#include "menu.h"

void Orbion_settings::update()
{
    static long timer=0;
    if(millis()-timer>150){
        timer=millis();
        
        if(toupdate){
            toupdate= !toupdate;
            
            display->clearDisplay();
            display->setTextColor(SH110X_WHITE);
            display->setTextSize(1);

            if(show){

                startpos = ((list[current_menu+1] >> 4) & 0xF) + list[0];
                nbitem= list[current_menu+1] & 0x0F;    

                display->drawLine(15,19,display->width()-15,19,SH110X_WHITE),
                display->setCursor( 0 , 9);
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
                display->drawRoundRect(10,10,108,28,6,SH110X_WHITE);
                display->setCursor(12,20);
                pgmString(23+conf.Mode);
                print_center(buf);
            }
            display->display();
        }
    }
}

/// @brief Allow display to be refresh()
void Orbion_settings::refresh(){
    toupdate = true;
    return;
}
/// @brief Exit Settings Mode
void Orbion_settings::exitSettings(){
    show=false;
}

/// @brief Enter Settings Mode
void Orbion_settings::startSettings()
{
    show = true;
    toupdate= true;
    current_menu=1;
    current_item=0;
    actionmode=0;
}

// void Orbion_settings::startSettings(uint8_t menu_id)
// {
//     show = true;
//     toupdate= true;
//     current_menu=menu_id;
//     current_item=0;
//     actionmode=0;
//     enter();
// }
void Orbion_settings::select()
{
        // Choose first item to display 
        if(current_item > 2 + startm){
            startm = current_item - 2;
        }else if(current_item < startm){
            startm = current_item;
        }

        for(uint8_t a = 0 ; a < min(3,nbitem) ; a++){
            display->setCursor( 31 , 24 + a *12);
            //strcpy_P(buf,(char*)pgm_read_word(&(datas[startpos+a+startm])));
            pgmString(startpos+a+startm);
           uint8_t TEXTCOLOR =0;
           if(current_item == a + startm ){
                fillrect_center(buf);
                TEXTCOLOR = SH110X_BLACK;
            }else{
                TEXTCOLOR = SH110X_WHITE;
            }
            display->setTextColor(TEXTCOLOR);
            print_center(buf,0,display->width());
        }

}

void Orbion_settings::action()
{
    if(actionmode && show ){

        display->setCursor(0 , 23);
        //strcpy_P(buf,(char*)pgm_read_word(&(datas[startpos+current_item])));
        pgmString(startpos+current_item);
        print_center(buf);
        display->setCursor( 60 , 40);

        switch(actionmode){
            case 1:
            case 3:
            case 6:
            case 7:
                //strcpy_P(buf,(char*)pgm_read_word(&(datas[action_strings[0]+str_pos+actionvalue])));
                pgmString(action_strings[0]+str_pos+actionvalue);
                fillrect_center(buf);
                display->setTextColor(SH110X_BLACK);
                print_center(buf);
                //display->print(actionvalue);
            break;
            case 4:
                //strcpy_P(buf,(char*)pgm_read_word(&(datas[action_strings[0] + str_pos + (actionvalue < 36 ? 0 : actionvalue-35)]) ));
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
                display->setCursor( 20 , 35);

                display->print("HSV ");
                
                display->print(actionvalue);
                display->write(44);
                display->print(jogxvalue);
                display->write(44);
                display->print(jogyvalue);
                display->setCursor( 20 , 47);
                display->print("RGB #");
                display->print(_leds->getColor(),HEX);

                _leds->display(0,1,c,0);
                }
            break;
            default:
                display->setTextColor(SH110X_WHITE);
                display->print(actionvalue +1);
            break;        }   
    }
}

/// @brief Way back in setting menu
void Orbion_settings::back()
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

void Orbion_settings::scroll(int8_t inc)
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

void Orbion_settings::jogx(int8_t inc)
{
    static uint64_t timer=millis();
    _jog(inc, &jogxvalue, &timer);
}
void Orbion_settings::jogy(int8_t inc)
{
    static uint64_t timer=millis();  
    _jog(inc, &jogyvalue, &timer);
       
}

void Orbion_settings::_jog(int8_t inc, uint8_t * axevalue, uint64_t * timer)
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

void Orbion_settings::enter()
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
void  Orbion_settings::print_center(const char  &t)
{
    uint8_t x = width/2 - 3;
    display->setCursor(x,display->getCursorY());
    display->print(t);
}

/// @brief Print a String centered
/// @param t 
void Orbion_settings::print_center(const String &t)
{
    uint8_t x = width/2 - 3 * t.length();
    display->setCursor(x,display->getCursorY());
    display->print(t);
}

/// @brief print a string centerd from x to x+w
/// @param t 
/// @param x 
/// @param w 
void Orbion_settings::print_center(const String &t,int8_t x,uint8_t w)
{
    uint8_t xs = x + w/2 - 3 * t.length();
    display->setCursor(xs,display->getCursorY());
    display->print(t);
}

void Orbion_settings::fillrect_center(const String &t)
{
    uint8_t x1 = width/2 - 3 * t.length() - 5;
    display->fillRect(x1,display->getCursorY()-2,t.length()*6+10,11,SH110X_WHITE);
}

/// @brief  Optimize string read sore it to buffer
/// @param id 
void Orbion_settings::pgmString(uint8_t id){
   
    strcpy_P(buf,(char *)pgm_read_word(&datas[id]));

}


//// EEPROM Read/write functions   ////


uint8_t Orbion_settings::EE_addr(uint8_t id)
{
    int ee_adr = (int)ee_address[id-1] - 1 ;
    return ee_adr;
}

void Orbion_settings::EE_write(uint8_t id, byte value)
{
   EE_write(id,value,0U);
} 
void Orbion_settings::EE_write(uint8_t id, byte value, uint8_t offset)
{
    int ee_adr = EE_addr(id) ;
    EEPROM.update(ee_adr + offset,value);
}

uint8_t Orbion_settings::EE_read(uint8_t id)
{   
    return EE_read(id,0U);
}
uint8_t Orbion_settings::EE_read(uint8_t id, uint8_t offset)
{
    int ee_adr = EE_addr(id) ;
    return EEPROM.read(ee_adr + offset);
}

//// Load config

void Orbion_settings::loadConfig()
{
    if(EE_read(13) == 1){
        conf.Mode = EE_read(1);
        conf.sensibility = EE_read(2);
        conf.Encoder = EE_read(3) ? -1 : 1;

        conf.color1 = _leds->Color(EE_read(8),EE_read(8,1),EE_read(8,2));
        conf.color2= _leds->Color(EE_read(9),EE_read(9,1),EE_read(9,2));;
        conf.led_mode= EE_read(10);
        conf.led_color_mode= EE_read(11);
        conf.timeout = EE_read(12) * 1000;

        buttonKey(&conf.b0, EE_read(4));
        buttonKey(&conf.b1, EE_read(5));
        buttonKey(&conf.b2, EE_read(6));
        buttonKey(&conf.b3, EE_read(7));
     }else{

// first run Write default settings on EEPROM
        EE_write(13,1);
        byte var[11];
        memcpy_P(var,pgm_read_word(&datas[5]),11);
        for(uint8_t i=0; i<11; i++)
        {
        EE_write(i+1,var[i]);
        } 
        loadConfig();
     }
    
}

void Orbion_settings::buttonKey(button_action * b, uint8_t id)
{
    b->M = id < 42 ? 4 : 0;
    if(id < 36){
        //strcpy_P(buf,(char*)pgm_read_word(&(datas[32])));
        pgmString(33);
        b->K = buf[id];
    }else{
        //strcpy_P(buf,(char*)pgm_read_word(&(datas[4])));
        pgmString(4);
        b->K = buf[id-36];
    }
    return;
}
