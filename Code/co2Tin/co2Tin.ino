/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
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


#include <Arduino.h>
#include <SensirionI2CScd4x.h>

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define lowBatteryPin A2


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

uint16_t ppm = 0;
float temp = 0.0f;
float hum = 0.0f;

int pixTic = 0;
long ticInit = 0;
bool barState = false;
int sensorInit;
bool battOn = false;

SensirionI2CScd4x scd4x;

void printUint16Hex(uint16_t value) {
    Serial.print(value < 4096 ? "0" : "");
    Serial.print(value < 256 ? "0" : "");
    Serial.print(value < 16 ? "0" : "");
    Serial.print(value, HEX);
}


void setup() {

Serial.begin(115200);

  Wire.begin();

  uint16_t error;
  char errorMessage[256];

  pinMode(lowBatteryPin, INPUT_PULLUP);
  

  scd4x.begin(Wire);

  // stop potentially previously started measurement
  error = scd4x.stopPeriodicMeasurement();
  if (error) {
      Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
      errorToString(error, errorMessage, 256);
      Serial.println(errorMessage);
  }

    uint16_t serial0;
    uint16_t serial1;
    uint16_t serial2;
    error = scd4x.getSerialNumber(serial0, serial1, serial2);
    if (error) {
        Serial.print("Error trying to execute getSerialNumber(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        //printSerialNumber(serial0, serial1, serial2);
    }

      // Start Measurement
    error = scd4x.startPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
  
  ;

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds


  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  display.setTextColor(SSD1306_WHITE);

  


  ticInit = millis();


}



void loop() {

  if ((millis() - ticInit) > 1000){

    pixTic += 1;
    ticInit = millis();
    
    if (pixTic >= 127){
      pixTic -= 127;
      barState = not(barState);
    }

    if ((pixTic % 2) == 0){
      battOn = not(battOn);
    }
    
  }

  drawMainDisplay();

  measureCo2();
  
}

void measureCo2(){

    uint16_t error;
    char errorMessage[256];
    bool doMeasurement = false;

    if ((millis() - sensorInit) > 5000){
      doMeasurement = true;
      sensorInit = millis();
    }
  
    // Read Measurement

    if (doMeasurement){
      error = scd4x.readMeasurement(ppm, temp, hum);
      if (error) {
          Serial.print("Error trying to execute readMeasurement(): ");
          errorToString(error, errorMessage, 256);
          Serial.println(errorMessage);
      } else if (ppm == 0) {
          Serial.println("Invalid sample detected, skipping.");
      } else {
          Serial.print("Co2:");
          Serial.print(ppm);
          Serial.print("\t");
          Serial.print("Temperature:");
          Serial.print(temp);
          Serial.print("\t");
          Serial.print("Humidity:");
          Serial.println(hum);
      }
    }
}

void drawMainDisplay(){

  display.clearDisplay();

  display.setFont();
  display.setTextSize(1); 
  display.setCursor(58, 7);
  display.println(F("ppm"));
  display.setCursor(58, 17);
  display.println(F("CO2"));

  display.setCursor(122, 16);
  display.println(F("%"));

  display.drawCircle(123, 1, 1, SSD1306_WHITE);

  display.setCursor(91, 2);
  display.println(temp);

  display.setCursor(91, 18);
  display.println(hum);

  display.setFont(&FreeSans12pt7b);
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(ppm);

  if (pixTic < 127) {
    if (barState) {
      display.fillRect(pixTic, 31, display.width(), 31, SSD1306_WHITE);
    }
    else{
      display.fillRect(0, 31, pixTic, 31, SSD1306_WHITE);
    }
  }
   else{
    if (barState){
      display.fillRect(0, 31, display.width(), 31, SSD1306_WHITE);
   
    }
    
  }


  if (lowBatteryCheck()){
      lowBatteryDisplay();
  }
  
  display.display();
  
}


bool lowBatteryCheck(){

  bool lowBatt = false;
  if (digitalRead(lowBatteryPin) == LOW){
    lowBatt = battOn;
  }


  
  return lowBatt;
}

void lowBatteryDisplay(){
  display.drawRect(60, 0, 12, 7, SSD1306_WHITE);
  display.fillRect(72, 2, 1, 3, SSD1306_WHITE);
  display.fillRect(62, 2, 1, 3, SSD1306_WHITE);
}




void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}

