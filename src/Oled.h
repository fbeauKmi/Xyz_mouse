//*******************************************************************************************//
//                                           Display Set                                     //
//*******************************************************************************************//

/////////////// SSD1306 ////////////////////////

// UNCOMMENT ALL IN THIS SECTION IF YOU CAN USE A SH11XX LIBRERY
// also uncomments the inherent initialization 

//#include <Adafruit_SSD1306.h>              
//Adafruit_SSD1306 display(4);      
//#define SSD1306 HIGH
//#define SH110X_WHITE WHITE

/////////////// SH110x ////////////////////////

// COMMENT ALL IN THIS SECTION IF YOU CAN USE A SSD1306 LIBRERY
// also uncomments the inherent initialization

#include <Adafruit_SH110X.h>
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, 4);
#define SSD1306 LOW
#define WHITE SH110X_WHITE