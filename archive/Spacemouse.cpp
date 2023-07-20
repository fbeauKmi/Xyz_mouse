#include "Spacemouse.h"

#if defined(_USING_HID)

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
  0x75, 0x04,           //    Report Size (1)
  0x95, 0x08,           //    Report Count (24)
  0x05, 0x09,           //    Usage Page (Button)
  0x19, 0x01,           //    Usage Minimum (Button #1)
  0x29, 0x08,           //    Usage Maximum (Button #24)
  0x81, 0x02,           //    Input (variable,absolute)
  0xC0,
  0xC0
};



Spacemouse_::Spacemouse_(void) : _buttons(0) {
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    HID().AppendDescriptor(&node);
}

int Spacemouse_::begin(){
    return HID().begin();
}

int Spacemouse_::end(){
    return 0;
}

void Spacemouse_::translate(int16_t x, int16_t y, int16_t z)
{
    send_axis(HID_TRANSLATE,x,y,z);
}

void Spacemouse_::rotate(int16_t rx, int16_t ry, int16_t rz)
{
    send_axis(HID_ROTATE,rx,ry,rz);
}

void Spacemouse_::send_axis(uint8_t hid_report_id, int16_t x, int16_t y, int16_t z)
{
    uint8_t transform[6] = {(uint8_t)(x & 0xFF), (uint8_t)(x >> 8), (uint8_t)(y & 0xFF), (uint8_t)(y >> 8), (uint8_t)(z & 0xFF), (uint8_t)(z >> 8)};
    HID().SendReport(hid_report_id, transform, sizeof(transform));
}

void Spacemouse_::buttons(uint32_t b){
    
    if (b != _buttons)
	{
		_buttons = b;
		HID().SendReport(3,&_buttons,sizeof(_buttons));
    }
}

void Spacemouse_::press(uint32_t b) 
{
	buttons(_buttons | b);
}

void Spacemouse_::release(uint32_t b)
{
	buttons(_buttons & ~b);
}


Spacemouse_ Spacemouse;

#endif