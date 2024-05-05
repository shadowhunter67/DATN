// OLED cho ESP8266
/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// OLED (SDA) -> ESP8266 (D2-SDA)
// OLED (SCK) -> ESP8266 (D1-SCL)
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };
int a=2;
void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Living Room"));
  
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,8);             // Start at top-left corner
  display.println(F("T: 33.9*C"));

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(70,8);             // Start at top-left corner
  display.println(F("H: 55%"));

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,16);             // Start at top-left corner
  display.println(F("Light: 1"));

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(70,16);             // Start at top-left corner
  display.println(F("Pan: 1"));
  
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,24);             // Start at top-left corner
  display.println(F("Kitchen Room"));
  display.display();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,32);             // Start at top-left corner
  display.println(F("Gas: 50"));
  display.display();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(70,32);             // Start at top-left corner
  display.println(F("Buzzer: 1"));
  display.display();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,40);             // Start at top-left corner
  display.println(F("Bath Room"));
  display.display();
  
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,48);             // Start at top-left corner
  display.println(F("PIR: 1"));
  display.display();
  
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(70,48);             // Start at top-left corner
  display.println(F("Pir: 1"));
  display.display();

}

void loop() {
//  display.setTextSize(1);             // Normal 1:1 pixel scale
//  display.setTextColor(SSD1306_WHITE);        // Draw white text
//  display.setCursor(0,0);             // Start at top-left corner
//  display.println(F("Living Room"));
//  display.display();
//  
//  display.setTextSize(1);             // Normal 1:1 pixel scale
//  display.setTextColor(SSD1306_WHITE);        // Draw white text
//  display.setCursor(0,8);             // Start at top-left corner
//  display.println(F("T: 33.9*C"));
//  display.display();
//  
//  display.setTextSize(1);             // Normal 1:1 pixel scale
//  display.setTextColor(SSD1306_WHITE);        // Draw white text
//  display.setCursor(70,8);             // Start at top-left corner
//  display.print(F("H: "));
//  display.print(a);
//  display.println(F("%"));
//  display.display();
}



// OLED cho ESP32
///**************************************************************************
// This is an example for our Monochrome OLEDs based on SSD1306 drivers
//
// Pick one up today in the adafruit shop!
// ------> http://www.adafruit.com/category/63_98
//
// This example is for a 128x64 pixel display using I2C to communicate
// 3 pins are required to interface (two I2C and one reset).
//
// Adafruit invests time and resources providing this open
// source code, please support Adafruit and open-source
// hardware by purchasing products from Adafruit!
//
// Written by Limor Fried/Ladyada for Adafruit Industries,
// with contributions from the open source community.
// BSD license, check license.txt for more information
// All text above, and the splash screen below must be
// included in any redistribution.
// **************************************************************************/
//
//#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//// OLED (SDA) -> ESP32 (22-SDA)
//// OLED (SCK) -> ESP32 (21-SCL)
//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels
//
//// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//// The pins for I2C are defined by the Wire-library. 
//// On an arduino UNO:       A4(SDA), A5(SCL)
//// On an arduino MEGA 2560: 20(SDA), 21(SCL)
//// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
//#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
//#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//
//#define NUMFLAKES     10 // Number of snowflakes in the animation example
//
//#define LOGO_HEIGHT   16
//#define LOGO_WIDTH    16
//static const unsigned char PROGMEM logo_bmp[] =
//{ 0b00000000, 0b11000000,
//  0b00000001, 0b11000000,
//  0b00000001, 0b11000000,
//  0b00000011, 0b11100000,
//  0b11110011, 0b11100000,
//  0b11111110, 0b11111000,
//  0b01111110, 0b11111111,
//  0b00110011, 0b10011111,
//  0b00011111, 0b11111100,
//  0b00001101, 0b01110000,
//  0b00011011, 0b10100000,
//  0b00111111, 0b11100000,
//  0b00111111, 0b11110000,
//  0b01111100, 0b11110000,
//  0b01110000, 0b01110000,
//  0b00000000, 0b00110000 };
//int a=2;
//void setup() {
//  Serial.begin(9600);
//
//  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
//  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
//    Serial.println(F("SSD1306 allocation failed"));
//    for(;;); // Don't proceed, loop forever
//  }
//
//  // Show initial display buffer contents on the screen --
//  // the library initializes this with an Adafruit splash screen.
//  display.display();
//  delay(2000); // Pause for 2 seconds
//
//  // Clear the buffer
//  display.clearDisplay();
//
//  // Draw a single pixel in white
//  display.drawPixel(10, 10, SSD1306_WHITE);
//
//  // Show the display buffer on the screen. You MUST call display() after
//  // drawing commands to make them visible on screen!
//  display.display();
//  delay(2000);
//  // display.display() is NOT necessary after every single drawing command,
//  // unless that's what you want...rather, you can batch up a bunch of
//  // drawing operations and then update the screen all at once by calling
//  // display.display(). These examples demonstrate both approaches...
//
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(0,0);             // Start at top-left corner
////  display.println(F("Living Room"));
////  
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(0,8);             // Start at top-left corner
////  display.println(F("T: 33.9*C"));
////
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(70,8);             // Start at top-left corner
////  display.println(F("H: 55%"));
////
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(0,16);             // Start at top-left corner
////  display.println(F("Light: 1"));
////
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(70,16);             // Start at top-left corner
////  display.println(F("Pan: 1"));
////  
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(0,24);             // Start at top-left corner
////  display.println(F("Kitchen Room"));
////  display.display();
////
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(0,32);             // Start at top-left corner
////  display.println(F("Gas: 50"));
////  display.display();
////
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(70,32);             // Start at top-left corner
////  display.println(F("Buzzer: 1"));
////  display.display();
////
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(0,40);             // Start at top-left corner
////  display.println(F("Bath Room"));
////  display.display();
////  
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(0,48);             // Start at top-left corner
////  display.println(F("PIR: 1"));
////  display.display();
////  
////  display.setTextSize(1);             // Normal 1:1 pixel scale
////  display.setTextColor(SSD1306_WHITE);        // Draw white text
////  display.setCursor(70,48);             // Start at top-left corner
////  display.println(F("Pir: 1"));
////  display.display();
//
//}
//
//void loop() {
//  display.setTextSize(1);             // Normal 1:1 pixel scale
//  display.setTextColor(SSD1306_WHITE);        // Draw white text
//  display.setCursor(0,0);             // Start at top-left corner
//  display.println(F("Living Room"));
//  display.display();
//  
//  display.setTextSize(1);             // Normal 1:1 pixel scale
//  display.setTextColor(SSD1306_WHITE);        // Draw white text
//  display.setCursor(0,8);             // Start at top-left corner
//  display.println(F("T: 33.9*C"));
//  display.display();
//  
//  display.setTextSize(1);             // Normal 1:1 pixel scale
//  display.setTextColor(SSD1306_WHITE);        // Draw white text
//  display.setCursor(70,8);             // Start at top-left corner
//  display.print(F("H: "));
//  display.print(a);
//  display.println(F("%"));
//  display.display();
//}
