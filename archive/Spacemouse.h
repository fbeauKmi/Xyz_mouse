// Credits jfedor (github)
// This Mimic the Hid descriptor of 3Dconnexion SpaceMouse compact
// setUSB.py at root allows to set Vip:Pid required to let the drivers recognize the Mouse

#ifndef SPACEMOUSE_H
#define SPACEMOUSE_H

#include "HID.h"

#if !defined(_USING_HID)

#warning "Using legacy HID core (non pluggable)"

#else

#ifndef SPACEMOUSE_ENTERPRISE
  #define SPACEMOUSE_COMPACT
  #define KEY_LEFT     0x1
  #define KEY_RIGHT    0x2
#endif

#ifdef SPACEMOUSE_ENTERPRISE
  #define KEY_MENU     0x1
  #define KEY_ALT      0x80000
  #define KEY_CTRL     0x200000
  #define KEY_SHIFT    0x100000
  #define KEY_ESC      0x40000
  #define KEY_1        0x1000
  #define KEY_2        0x2000
  #define KEY_3        0x3000
  #define KEY_4        0x4000
  #define KEY_ROLL_CLOCKWISE  0x100
  #define KEY_TOP      0x4
  #define KEY_ROTATION 0x400000
  #define KEY_FRONT    0x20
  #define KEY_REAR     0x10
  #define KEY_FIT      0x2
#endif

#define HID_ROTATE     2
#define HID_TRANSLATE  1

class Spacemouse_ 
{
  public:
    Spacemouse_(void);
    int begin(void);
    int end(void);
    void rotate(int16_t rx, int16_t ry, int16_t rz);
    void translate(int16_t x, int16_t y, int16_t z);
    void send_axis(uint8_t hid_report_id, int16_t x, int16_t y, int16_t z);
    void press(uint32_t key);
    void release(uint32_t key);
  
  private:
    uint32_t _buttons;
    void buttons(uint32_t b);


};
extern Spacemouse_ Spacemouse;

// /// @brief send Motion Hid report
// /// @param rx Rotation   X
// /// @param ry            Y
// /// @param rz            Z
// /// @param x translation X
// /// @param y             Y
// /// @param z             Z
// void send_command(int16_t rx, int16_t ry, int16_t rz, int16_t x, int16_t y, int16_t z) {
//   uint8_t trans[6] = {(uint8_t)(x & 0xFF), (uint8_t)(x >> 8), (uint8_t)(y & 0xFF), (uint8_t)(y >> 8), (uint8_t)(z & 0xFF), (uint8_t)(z >> 8)};
//   HID().SendReport(1, trans, 6);
//   uint8_t rot[6] = {(uint8_t)(rx & 0xFF), (uint8_t)(rx >> 8), (uint8_t)(ry & 0xFF), (uint8_t)(ry >> 8), (uint8_t)(rz & 0xFF), (uint8_t)(rz >> 8)};
//   HID().SendReport(2, rot, 6);
// }

// /// @brief send Buttons Hid report
// /// @param L_state 
// /// @param R_state 
// void send_buttons(uint8_t L_state, uint8_t R_state){
//   static uint8_t laststate = 0;
//   uint8_t  buttons_state = L_state << 0 | R_state << 1;
//   if(buttons_state !=laststate){
//        HID().SendReport(3,&buttons_state,1);
//        laststate = buttons_state;
//   }
// }
#endif
#endif