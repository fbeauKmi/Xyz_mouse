#include "orbion_display.h"

Orbion_display::Orbion_display(const char* const* strings) : Adafruit_SH1106G(128, 64, &Wire, 4)
{
    datas = strings;
    strcpy_P(list, (char*)pgm_read_word(&(datas[0])));
    strcpy_P(actions, (char*)pgm_read_word(&(datas[1])));
    strcpy_P(ee_address, (char*)pgm_read_word(&(datas[2])));
    strcpy_P(action_strings, (char*)pgm_read_word(&(datas[3])));
}

void Orbion_display::init(){
    begin(0x3C, true); // Address 0x3C default     // comment if your oled is SSD1306
    setRotation(2);   // define screen orientation
    clearDisplay();
    display();
}

void Orbion_display::update()
{
    static long timer=0;
    if(millis()-timer>150){
        timer=millis();
        
        if(toupdate){
            toupdate= !toupdate;
            
            clearDisplay();
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
                drawRoundRect(10,10,108,28,6,SH110X_WHITE);
                setCursor(12,20);
                pgmString(25+conf.Mode);
                print_center(buf);
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
/// @brief Exit Settings Mode
void Orbion_display::exitSettings(){
    show=false;
}

/// @brief Enter Settings Mode
void Orbion_display::startSettings()
{
    show = true;
    toupdate= true;
    current_menu=1;
    current_item=0;
    actionmode=0;
}

// void Orbion_display::startSettings(uint8_t menu_id)
// {
//     show = true;
//     toupdate= true;
//     current_menu=menu_id;
//     current_item=0;
//     actionmode=0;
//     enter();
// }
void Orbion_display::select()
{
        // Choose first item to display 
        if(current_item > 2 + startm){
            startm = current_item - 2;
        }else if(current_item < startm){
            startm = current_item;
        }

        for(uint8_t a = 0 ; a < min(3,nbitem) ; a++){
            setCursor( 31 , 24 + a *12);
            //strcpy_P(buf,(char*)pgm_read_word(&(datas[startpos+a+startm])));
            pgmString(startpos+a+startm);
           uint8_t TEXTCOLOR =0;
           if(current_item == a + startm ){
                fillrect_center(buf);
                TEXTCOLOR = SH110X_BLACK;
            }else{
                TEXTCOLOR = SH110X_WHITE;
            }
            setTextColor(TEXTCOLOR);
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
            case 1:
            case 3:
            case 6:
            case 7:
                pgmString(action_strings[0]+str_pos+actionvalue);
                fillrect_center(buf);
                setTextColor(SH110X_BLACK);
                print_center(buf);
                //print(actionvalue);
            break;
            case 4:
                pgmString(action_strings[0] + str_pos + (actionvalue < 36 ? 0 : actionvalue-35));
                if(actionvalue < 36){
                    print_center(buf[actionvalue]);
                }else{
                    print_center(buf);
                }
            break;
            case 5:
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
            default:
                setTextColor(SH110X_WHITE);
                print(actionvalue +1);
            break;        }   
    }
}

/// @brief Way back in setting menu
void Orbion_display::back()
{
    toupdate=true;
    if(current_menu>1){
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
    actionvalue = actionmode == 5 ? 
                    actionvalue + (inc==1 ? 8 : -8) : 
                    min(action_strings[actionmode],max(0,actionvalue-inc));
   }
}

void Orbion_display::jogx(int8_t inc)
{
    static uint64_t timer=millis();
    _jog(inc, &jogxvalue, &timer);
}
void Orbion_display::jogy(int8_t inc)
{
    static uint64_t timer=millis();  
    _jog(inc, &jogyvalue, &timer);
       
}

void Orbion_display::_jog(int8_t inc, uint8_t * axevalue, uint64_t * timer)
{
    if(actionmode == 5){
        if(millis()- *timer > 25){
            *timer = millis(); 
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
    uint8_t id = startpos + current_item - list[0] - 4;
    if(actionmode){            // Update EEPROM
    EE_write(id , actionvalue);
    if(actionmode==5){
        EE_write(id , jogxvalue, 1);
        EE_write(id , jogyvalue, 2);
    }
    loadConfig();
    back();
    } else {                         // Read EEPROM
    actionmode = actions[id-1] & 0x0F;
    actionvalue =  EE_read(id) ;
    actionvalue = min(actionvalue,(uint8_t) action_strings[actionmode]);
    switch(actionmode){
        case 1:
            str_pos=0;
        break;
        case 3:
            str_pos= 21;
        break;
        case 4:
            str_pos = 10;
        break;
        case 6:
            str_pos = 23;
        break;
        case 7:
            str_pos = 26;    
        break;
        case 5:
            jogxvalue = EE_read(id,1);
            jogyvalue = EE_read(id,2);
        break;
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

/// @brief print a string centerd from x to x+w
/// @param t 
/// @param x 
/// @param w 
void Orbion_display::print_center(const String &t,int8_t x,uint8_t w)
{
    uint8_t xs = x + w/2 - 3 * t.length();
    setCursor(xs,getCursorY());
    print(t);
}

void Orbion_display::fillrect_center(const String &t)
{
    uint8_t x1 = width()/2 - 3 * t.length() - 5;
    fillRect(x1,getCursorY()-2,t.length()*6+10,11,SH110X_WHITE);
}

/// @brief  Optimize string read sore it to buffer
/// @param id 
void Orbion_display::pgmString(uint8_t id){
    strcpy_P(buf,(char *)pgm_read_word(&datas[id]));
}

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
    if(EE_read(15) == 1){
        conf.Mode = EE_read(1);
        conf.AltMode0 = conf.Mode;
        conf.AltMode1 = EE_read(2);
        conf.AltMode2 = EE_read(3);
        conf.sensibility = EE_read(4);
        conf.Encoder = EE_read(5) ? -1 : 1;

        conf.color1 = _leds->Color(EE_read(10),EE_read(10,1),EE_read(10,2));
        conf.color2= _leds->Color(EE_read(11),EE_read(11,1),EE_read(11,2));;
        conf.led_mode= EE_read(12);
        conf.led_color_mode= EE_read(13);
        conf.timeout = EE_read(14) * 1000;

        buttonKey(&conf.b0, EE_read(6));
        buttonKey(&conf.b1, EE_read(7));
        buttonKey(&conf.b2, EE_read(8));
        buttonKey(&conf.b3, EE_read(9));
     }else{

// first run Write default settings on EEPROM
        EE_write(15,1);
        byte var[13];
        memcpy_P(var,pgm_read_word(&datas[5]),11);
        for(uint8_t i=0; i<11; i++)
        {
        EE_write(i+1,var[i]);
        } 
        loadConfig();
     }
    
}

void Orbion_display::buttonKey(button_action * b, uint8_t id)
{
    b->M = id < 42 ? 4 : 0;
    if(id < 36){
        pgmString(35);
        b->K = buf[id];
    }else{
        pgmString(4);
        b->K = buf[id-36];
    }
    return;
}
