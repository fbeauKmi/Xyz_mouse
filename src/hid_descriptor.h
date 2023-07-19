// Credits jfedor (github)
// This Mimic the Hid descriptor of 3Dconnexion SpaceMouse compact
// setUSB.py at root allows to set Vip:Pid required to let the drivers recognize the Mouse
#pragma once
#ifndef HIDDESCRIPTOR_H
#define HIDDESCRIPTOR_H

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  0x05, 0x01,           //  Usage Page (Generic Desktop)
  0x09, 0x08,           //  0x08: Usage (Multi-Axis)
  0xa1, 0x01,           //  Collection (Application)
  0xa1, 0x00,           // Collection (Physical)
  0x85, 0x01,           //  Report ID
  0x16, 0x00, 0x80,     //logical minimum (-500)
  0x26, 0xff, 0x7f,     //logical maximum (500)
  0x36, 0x00, 0x80,     //Physical Minimum (-32768)
  0x46, 0xff, 0x7f,     //Physical Maximum (32767)
  0x09, 0x30,           //    Usage (X)
  0x09, 0x31,           //    Usage (Y)
  0x09, 0x32,           //    Usage (Z)
  0x75, 0x10,           //    Report Size (16)
  0x95, 0x03,           //    Report Count (3)
  0x81, 0x02,           //    Input (variable,absolute)
  0xC0,                 //  End Collection
  0xa1, 0x00,           // Collection (Physical)
  0x85, 0x02,           //  Report ID
  0x16, 0x00, 0x80,     //logical minimum (-500)
  0x26, 0xff, 0x7f,     //logical maximum (500)
  0x36, 0x00, 0x80,     //Physical Minimum (-32768)
  0x46, 0xff, 0x7f,     //Physical Maximum (32767)
  0x09, 0x33,           //    Usage (RX)
  0x09, 0x34,           //    Usage (RY)
  0x09, 0x35,           //    Usage (RZ)
  0x75, 0x10,           //    Report Size (16)
  0x95, 0x03,           //    Report Count (3)
  0x81, 0x02,           //    Input (variable,absolute)
  0xC0,                 //  End Collection
 
  0xa1, 0x00,           // Collection (Physical)
  0x85, 0x03,           //  Report ID
  0x15, 0x00,           //   Logical Minimum (0)
  0x25, 0x01,           //    Logical Maximum (1)
  0x75, 0x01,           //    Report Size (1)
  0x95, 0x08,           //    Report Count (24)
  0x05, 0x09,           //    Usage Page (Button)
  0x19, 0x01,           //    Usage Minimum (Button #1)
  0x29, 0x08,           //    Usage Maximum (Button #24)
  0x81, 0x02,           //    Input (variable,absolute)
  0xC0,
  0xC0
};
 
/// @brief send Motion Hid report
/// @param rx Rotation   X
/// @param ry            Y
/// @param rz            Z
/// @param x translation X
/// @param y             Y
/// @param z             Z
void send_command(int16_t rx, int16_t ry, int16_t rz, int16_t x, int16_t y, int16_t z) {
  uint8_t trans[6] = {(uint8_t)(x & 0xFF), (uint8_t)(x >> 8), (uint8_t)(y & 0xFF), (uint8_t)(y >> 8), (uint8_t)(z & 0xFF), (uint8_t)(z >> 8)};
  HID().SendReport(1, trans, 6);
  uint8_t rot[6] = {(uint8_t)(rx & 0xFF), (uint8_t)(rx >> 8), (uint8_t)(ry & 0xFF), (uint8_t)(ry >> 8), (uint8_t)(rz & 0xFF), (uint8_t)(rz >> 8)};
  HID().SendReport(2, rot, 6);
}

/// @brief send Buttons Hid report
/// @param L_state 
/// @param R_state 
void send_buttons(uint8_t L_state, uint8_t R_state){
  static uint8_t laststate = 0;
  uint8_t  buttons_state = L_state << 0 | R_state << 1;
  if(buttons_state !=laststate){
       HID().SendReport(3,&buttons_state,1);
       laststate = buttons_state;
  }
}

#endif