//***************************************************/
//         Pin Defines  &  Other settings           */
//***************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H


#define LED 15      // Neopixel pin
#define NUMPIXELS 7 // Number of led on the strip (default 7)
#define LED_DIR -1  // 1 or -1 depends the way your Leds "rotate"
#define DT 4        // DT Encoder
#define CLK 5       // CLK Encoder
#define AxisY A2    // VRY Joystick
#define AxisX A3    // VRX Joystick
#define joyButt A1  // SW Joystick
#define button1 7   // Rear Button
#define button2 8   // Rear Button
#define button3 9   // Rear Button

// Experimental :

// Leave uncomment if you want to use encoder as Zoom in PAN mode instead of Y joystick
#define ZOOM_ON_ENCODER

#endif