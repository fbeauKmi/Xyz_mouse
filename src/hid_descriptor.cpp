// 3DmouseHID library version: This project uses the 3DmouseHID library version 1.0.0.
//
//

#include "hid_descriptor.h"

mouseHID& XYZmouse()
{
    static mouseHID obj;
    return obj;
}

mouseHID::mouseHID()
{
}

int mouseHID::begin(void) {
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    HID().AppendDescriptor(&node);
    return HID().begin();

}

/// @brief  transform Axes before send report
/// @param mode 1|2 translation or rotation
/// @param x 
/// @param y 
/// @param z 
void mouseHID::send_command(uint8_t mode, Axes axes) {

    if (mode == 1)
    {
        // Revert axes for translation
        trans = axes;
        trans.y = axes.x;
        trans.x = axes.y * -1;
        rot = ZERO_AXES;
    } else {
        trans = ZERO_AXES;
        rot = axes;
    }
    send_report(); 
}

/// @brief Convert and report button States
/// @param L_state left button
/// @param R_state right button
void mouseHID::send_buttons(uint8_t L_state, uint8_t R_state) {
    button_state = L_state << 0 | R_state << 1;
    send_report();
}

/// @brief plan and report HID positions
void mouseHID::send_report() {
    static uint32_t _last_sent = 0;

    static uint8_t report_id = 1;
    static uint8_t last_button_state = 0;
    static uint8_t report_zero = 0;
    
    Axes* target = (report_id==1 ? &trans : &rot );
    uint8_t val[6];
    splitvalues(val, *target);

    if (isTimeout(&_last_sent, 8)) {
        
        // Skip report if zero values are reported more than 3 times 
        report_zero = (rot.x || rot.y || rot.z || trans.x || trans.y || trans.z) ? 0 : min(3 , report_zero) + 1 ;

        switch (report_id)
        {
        // Translation
        case 1:
        // Rotation
        case 2:
            if (report_zero < 3)
            {
                HID().SendReport(report_id, &val, 6);
            }
            report_id++;
            break;
        // buttons
        case 3:
            if(button_state != last_button_state) {
                // Send button state report
                last_button_state = button_state;
                HID().SendReport(3, &button_state, 1);
            }
            report_id = 1;
            break;
        }
    }
}

/// @brief Convert Axes struc into Axes array for HID
/// @param splitAxes 
/// @param axes 
/// @return 
bool mouseHID::splitvalues(uint8_t* splitAxes, Axes axes){
    
    splitAxes[0] = axes.x & 0xFF;
    splitAxes[1] = axes.x >> 8;
    splitAxes[2] = axes.y & 0xFF;
    splitAxes[3] = axes.y >> 8;
    splitAxes[4] = axes.z & 0xFF;
    splitAxes[5] = axes.z >> 8;
    return true;
    
}
