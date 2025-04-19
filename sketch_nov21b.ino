
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//#include "Adafruit_Sensor.h"
//#include "Adafruit_AM2320.h"
//Adafruit_AM2320 am2320 = Adafruit_AM2320();

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//SDA = A4
//SCL = A5

int timer = 0;
int IR1 = 2;
int IR2 = 3;
int Buzzer = 12;
int counter = 0;
char Flag1 = 'N';
char Flag2 = 'N';

void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  display.clearDisplay();
      display.setCursor(19, 0);
  display.println("Students");
   display.setCursor(8, 18);
   display.println("in the bus");
   display.setCursor(60, 36);
    display.println(counter);
 //  display.setCursor(30, 36);
 //   display.println(" ");
   display.display();
  
      

  pinMode(13,OUTPUT);
  pinMode(5,INPUT);
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:


  Serial.print(digitalRead (IR1));
  Serial.print("\t");
  Serial.print(Flag1);
  Serial.print("\t");
  Serial.print(digitalRead (IR2));
  Serial.print("\t");
  Serial.print(Flag2);
  Serial.print("\t");
  Serial.print(counter);
  Serial.println();

    
     // No one enters/exits
     if (digitalRead (IR1) == HIGH && digitalRead (IR2) == HIGH && Flag1 == 'N' && Flag2 == 'N')
    {
    //no action
    }
    
    // Someone just enters
    if (digitalRead (IR1) == LOW && digitalRead (IR2) == HIGH && Flag1 == 'N' && Flag2 == 'N')
      {
     Flag1 = 'F'; 
    }

      // Someone is halfway entered
    if (digitalRead (IR1) == HIGH && digitalRead (IR2) == HIGH && ((Flag1 == 'F' && Flag2 == 'N') || (Flag1 == 'N' && Flag2 == 'F')))
      {
        timer = timer + 1;
      if (timer >= 80)
       {
         Flag1 = 'N';
          Flag2 = 'N';
         timer = 0;
       }
      }
    
      // Someone is at 2nd sensor for entering
    if (digitalRead (IR1) == HIGH && digitalRead (IR2) == LOW && Flag1 == 'F' && Flag2 == 'N')
      {
     Flag2 = 'S'; 
    }
  
     // Someone enters bus completly
    if (digitalRead (IR1) == HIGH && digitalRead (IR2) == HIGH && Flag1 == 'F' && Flag2 == 'S')
      {
      counter = counter + 1;
      Flag1 = 'N';
      Flag2 = 'N';
      tone(Buzzer, 1200, 100);
      }
  //=======================================================================================================
        
        // Someone just exits
    if (digitalRead (IR1) == HIGH && digitalRead (IR2) == LOW && Flag1 == 'N' && Flag2 == 'N')
      {
     Flag2 = 'F'; 
    }

      // Someone is at 1st sensor during exit
    if (digitalRead (IR1) == LOW && digitalRead (IR2) == HIGH && Flag1 == 'N' && Flag2 == 'F')
      {
     Flag1 = 'S'; 
    }
  
     // Someone exits bus completly
    if (digitalRead (IR1) == HIGH && digitalRead (IR2) == HIGH && Flag1 == 'S' && Flag2 == 'F')
      {
      counter = counter - 1;
      Flag1 = 'N';
      Flag2 = 'N';
      tone(Buzzer, 2400, 100);
      }
    
    display.clearDisplay();
       display.setCursor(19, 0);
    display.println("Students");
   display.setCursor(8, 18);
   display.println("in the bus");
   display.setCursor(60, 36);
    display.println(counter);
   display.display();
    
  }
