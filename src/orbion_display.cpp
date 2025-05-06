#include "orbion_display.h"
#include <EEPROM.h>

#ifdef SSD1306
Orbion_display::Orbion_display():Adafruit_SSD1306(128, 64, &Wire, 4)
#else
Orbion_display::Orbion_display():Adafruit_SH1106G(128, 64, &Wire, 4)
#endif
{
    strcpy_P(list, (char*)pgm_read_word(&(datas[0])));
    strcpy_P(actions, (char*)pgm_read_word(&(datas[1])));
    strcpy_P(itemPos, (char*)pgm_read_word(&(datas[2])));
    strcpy_P(ee_address, (char*)pgm_read_word(&(datas[3])));
    strcpy_P(action_strings, (char*)pgm_read_word(&(datas[4])));
    memcpy_P(defaultVal,pgm_read_word(&datas[5]),12);
    ee_reset = defaultVal[0];
};     

void Orbion_display::init()
{
#ifdef SSD1306
    begin(SSD1306_SWITCHCAPVCC, 0x3C);
#else    
    begin(0x3C, true); // Address 0x3C default
#endif
    setRotation(2);   // define screen orientation
    clear();          // Clear screen at startup
};

#ifdef SSD1306
void Orbion_display::setContrast(uint8_t contrast)
{

}
#endif


void Orbion_display::update()
{
    static long timer=0;
    _currentMillis=millis();

    if(_currentMillis-timer>150){
        timer=_currentMillis;
        
        if(toupdate){
            toupdate= !toupdate;
            
            clearDisplay();
            setContrast(conf.contrast << 5);
            setTextColor(SH110X_WHITE);
            setTextSize(1);

            if(show){

                startpos = ((list[current_menu+1] >> 4) & 0xF) + list[0];
                nbitem= list[current_menu+1] & 0x0F;    

                drawLine(15,19,width()-15,19,SH110X_WHITE),
                setCursor( 0 , 9);
                strcpy(buf,"- ");
                strcat_P(buf,(char*)pgm_read_word(&(datas[current_menu+list[0]-1])));
                strcat(buf," -");
                buf[sizeof(buf) - 1] = '\0';
                print_center(buf);

                if(!actionmode){
                    select();
                }else{
                    action();
                }
            } else {

                drawBitmap(44,6,logo,logo_BMPWIDTH,logo_BMPHEIGHT,SH110X_WHITE);
                setCursor(12,45);
                print_center(FIRMWARE_NAME);
                setCursor(8,5);
                print(pantilt_mode ? "Pan" : "Rot");
            }
            display();
        }
    }
}

/// @brief Allow display to be refresh()
void Orbion_display::refresh(){
    toupdate = true;
    return;
}

/// @brief Allow display to be refresh() with a mode
void Orbion_display::refresh(bool mode){
    if( mode != pantilt_mode ) {
        pantilt_mode = mode;
        toupdate = true;
    }
    return;
}


/// @brief Clear display and update
void Orbion_display::clear(){
    clearDisplay();
    display();
}

/// @brief Exit Settings Mode
void Orbion_display::exitSettings(){
    show=false;
}

/// @brief Enter Settings Mode
void Orbion_display::startSettings()
{
    show = true;
    toupdate = true;
    current_menu = _ENTRY_MODE;
    current_item = 0;
    actionmode = 0;
}

/// @brief 
void Orbion_display::select()
{
        uint8_t _textcolor;
        
        // Only 3 item can be displayed  // Choose first item to display  // scroll trick
        if(current_item > 2 + startm){
            startm = current_item - 2;
        }else if(current_item < startm){
            startm = current_item;
        }

        // display menu items    
        for(uint8_t a = 0 ; a < min(3,nbitem) ; a++){
            setCursor( 31 , 24 + a *12);
            pgmString(startpos+a+startm);
            // Hightlight current item
            if(current_item == a + startm ){
                fillrect_center(buf);
                _textcolor = SH110X_BLACK;
            }else{
                _textcolor = SH110X_WHITE;
            }
            setTextColor(_textcolor);
            print_center(buf,0,width());
        }

}

void Orbion_display::action()
{
    if(actionmode && show ){
        setCursor(0 , 23);
        
        pgmString(startpos+current_item);
        print_center(buf);
        setCursor( 60 , 40);

        switch(actionmode){
            case PANTILT_AM:
            case KNOBDIR_AM:
            case MOTIONMOD_AM:
            case COLORMODE_AM:
            case RESET_AM:
                pgmString(action_strings[0]+str_pos+actionvalue);
                fillrect_center(buf);
                setTextColor(SH110X_BLACK);
                print_center(buf);
            break;
            case COLOR_AM:
                {
                uint32_t c = _leds->Color(actionvalue,jogxvalue,jogyvalue);
                setCursor( 20 , 35);

                print("HSV ");
                
                print(actionvalue);
                write(44);
                print(jogxvalue);
                write(44);
                print(jogyvalue);
                setCursor( 20 , 47);
                print("RGB #");
                print(_leds->getColor(),HEX);

                _leds->display(0,1,c,0);
                }
            break;
            case CONTRAST_AM:
                setContrast((actionvalue+1) << 5);
            default:
                setTextColor(SH110X_WHITE);
                print(actionvalue +1);
            break;
        }   
    }
}

/// @brief Way back in setting menu
void Orbion_display::back()
{
    toupdate=true;
    if(current_menu>_ENTRY_MODE || actionmode){
        if(!actionmode){
            current_item=current_menu-2;
            current_menu=1;
        }else{
            actionmode=0;
        }
        
    }else{
        exitSettings();
    }
    
}

void Orbion_display::scroll(int8_t inc)
{
   toupdate=true;
   if(!actionmode){
    current_item = max(0,min(nbitem-1,current_item-inc));
   }else{
    actionvalue = actionmode == COLOR_AM ? 
                    actionvalue + (inc==1 ? 8 : -8) : 
                    min(action_strings[actionmode],max(0,actionvalue+inc));
   }
}

void Orbion_display::jogx(int8_t inc)
{
    static uint64_t timer=0;
    _jog(inc, &jogxvalue, &timer);
}
void Orbion_display::jogy(int8_t inc)
{
    static uint64_t timer=0;  
    _jog(inc, &jogyvalue, &timer);
       
}

void Orbion_display::_jog(int8_t inc, uint8_t * axevalue, uint64_t * timer)
{
    if(actionmode == COLOR_AM){
        if(_currentMillis- *timer > 25){
            *timer = _currentMillis; 
            inc = (inc + 1) >> 2;
            *axevalue = min(0xFF,max(0,*axevalue - inc)); // = max(16,min(0,jogyvalue+inc));
            toupdate=true;
        }
    }
}

void Orbion_display::enter()
{
  
  if(current_menu==1){
    current_menu=current_item+2;
    current_item=0;
    startm=0;
    toupdate=true;
    select();
  }else{
    uint8_t id = startpos + current_item - list[0] - list[1] + 1;
    if(actionmode){ 
        if(actionmode==RESET_AM){
            ee_reset = (actionvalue==0 ? 0x01: 0xFF); // reset control byte to 0xFF
        }  else {        // Update EEPROM
            EE_write(id , actionvalue);
            if(actionmode==COLOR_AM){
                EE_write(id , jogxvalue, 1);
                EE_write(id , jogyvalue, 2);
            }
        }
        loadConfig();
        back();
    } else {                         // Read EEPROM
    actionmode = actions[id-2] & 0x0F;
    actionvalue =  EE_read(id) ;
    actionvalue = min(actionvalue,(uint8_t) action_strings[actionmode]);
    actionvalue =  (actionmode==RESET_AM?0:actionvalue);
    str_pos = itemPos[actionmode-1] - 1 ;
    
    if(actionmode == COLOR_AM){
        jogxvalue = EE_read(id,1);
        jogyvalue = EE_read(id,2);
    }
    toupdate=true;
    }
  }
}


////  Display functions

/// @brief print a single char centered
/// @param t 
void  Orbion_display::print_center(const char  &t)
{
    uint8_t x = width()/2 - 3;
    setCursor(x,getCursorY());
    print(t);
}

/// @brief Print a String centered
/// @param t 
void Orbion_display::print_center(const String &t)
{
    uint8_t x = width()/2 - 3 * t.length();
    setCursor(x,getCursorY());
    print(t);
}

/// @brief print a string centered between x and x+w
/// @param t 
/// @param x 
/// @param w 
void Orbion_display::print_center(const String &t,int8_t x,uint8_t w)
{
    uint8_t xs = x + w/2 - 3 * t.length();
    setCursor(xs,getCursorY());
    print(t);
}

/// @brief draw a filled rectangle on the string t
/// @param t 
void Orbion_display::fillrect_center(const String &t)
{
    uint8_t x1 = width()/2 - 3 * t.length() - 5;
    fillRect(x1,getCursorY()-2,t.length()*6+10,11,SH110X_WHITE);
}

/// @brief  read stting from PROGMEM store it to buffer
/// @param id 
void Orbion_display::pgmString(uint8_t id){
    strcpy_P(buf,(char *)pgm_read_word(&datas[id]));
}

/// @brief  
/// @param id 
/// @param nbval 
void Orbion_display::pgmUintArray(uint8_t id, uint8_t nbval){
   char cBuf[nbval];
   for(int a = 0; a<nbval; a++){
        strcpy_P(cBuf,(char *)pgm_read_word(&datas[id]));
   }
}


//// EEPROM Read/write functions   ////
uint8_t Orbion_display::EE_addr(uint8_t id)
{
    int ee_adr = (int)ee_address[id-1] - 1 ;
    return ee_adr;
}

void Orbion_display::EE_write(uint8_t id, byte value)
{
   EE_write(id,value,0U);
} 
void Orbion_display::EE_write(uint8_t id, byte value, uint8_t offset)
{
    int ee_adr = EE_addr(id) ;
    EEPROM.update(ee_adr + offset,value);
}

uint8_t Orbion_display::EE_read(uint8_t id)
{   
    return EE_read(id,0U);
}
uint8_t Orbion_display::EE_read(uint8_t id, uint8_t offset)
{
    int ee_adr = EE_addr(id) ;
    return EEPROM.read(ee_adr + offset);
}

//// Load config
void Orbion_display::loadConfig()
{
    if(EE_read(1) == ee_reset){ /// Check if EEPROM stores the config and read it
        conf.Mode = EE_read(3);
        conf.Encoder = EE_read(2) ? -1 : 1;

        conf.dz = min(19, EE_read(4)); /// limit DZ and sensitivity
        conf.sensitivity = min(4, EE_read(5));

        conf.color1 = _leds->Color(EE_read(6),EE_read(6,1),EE_read(6,2));
        conf.color2= _leds->Color(EE_read(7),EE_read(7,1),EE_read(7,2));;
        conf.led_mode= EE_read(8);
        conf.led_color_mode= EE_read(9);
        conf.contrast= EE_read(10);
        conf.timeout = EE_read(11) * 1000;
        
     }else{
        // EEPROM is empty or version changed
        // first byte is a control byte for version change
        // 0x01 = 1st version, 0x02 = 2nd version, etc..
        // 0xFF = no settings stored

        ee_reset = defaultVal[0]; // reset control byte
    
        for(uint8_t i=0; i<11; i++)
        {
        EE_write(i+1,defaultVal[i]);
        } 
        loadConfig();
     }
    
}

