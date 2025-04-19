#include <Adafruit_GFX.h>    // library for lcd
#include <Adafruit_ST7735.h> // library for ST7735
#include <Adafruit_ST7789.h> // library for ST7789
#include <SPI.h>

#define TFT_TCS        10
#define TFT_RST        8 // -1 will also work 
#define TFT_DC         9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_TCS, TFT_DC, TFT_RST);

// pin definitions
const int CLK_PIN = 3;  // pin 3 -> CLK
const int DOUT_PIN = 2; // pin 2 -> OUT
const int LED_PIN = 13; // led pin

// variables for ADC result
unsigned long hi_byte = 0;
unsigned long low_word = 0;
long highestValue = 0; // variable for highest pressure value

void initGPIO() {
  pinMode(LED_PIN, OUTPUT); 
  pinMode(DOUT_PIN, INPUT); 
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(CLK_PIN, LOW);
}

void waitForDOUTLow() {
  while (digitalRead(DOUT_PIN) != LOW) {
  }
}

void pulseCLK() {
  digitalWrite(CLK_PIN, LOW);
  digitalWrite(CLK_PIN, HIGH);
  digitalWrite(CLK_PIN, LOW);
}

void readHiByte() {
  hi_byte = 0;
  waitForDOUTLow();
  
  for (int i = 0; i < 8; i++) {
    pulseCLK();
    hi_byte <<= 1; 
    if (digitalRead(DOUT_PIN)) {
      hi_byte |= 1; 
    }
  }
}


void readLowWord() {
  low_word = 0; 
  
  for (int i = 0; i < 16; i++) { 
    pulseCLK(); 
    low_word <<= 1; 
    if (digitalRead(DOUT_PIN)) {
      low_word |= 1; 
    }
  }
  pulseCLK(); 
}


long getSignedValue() {
  unsigned long rawValue = (hi_byte << 16) | low_word; 
  if (rawValue & 0x800000) { 
    rawValue |= 0xFF000000; 
  }
  return (long)rawValue; 
}

void printCentered(String text) {
  int16_t x, y;
  uint16_t w, h;


  tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);


  int centerX = (tft.width() - w) / 2;


  tft.setCursor(centerX, (tft.height() / 2) - 9);
  tft.print(text);
}


void airMonitorWithDisplay() {
  initGPIO();
  tft.fillScreen(ST77XX_BLACK); 
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  //tft.print("  Ready");
  printCentered("Ready");

  while (true) {
    readHiByte();
    readLowWord();
    long adcValue = getSignedValue();


    if (adcValue > highestValue) {
      highestValue = adcValue;
  
      tft.fillScreen(ST77XX_BLACK); 
      tft.setCursor(0, 0);
      tft.print("Max: ");
      tft.print(((highestValue + 275000) / 1000) / 1493.6 ); 
      tft.print(" PSI");
    }

    Serial.println(((adcValue + 275000) / 1000) / 1493.6 ); 
    delay(100); 
  }
}

void setup() {
  Serial.begin(9600); 
  tft.initR(INITR_144GREENTAB); 
  airMonitorWithDisplay(); 
}

void loop() {
  
}
