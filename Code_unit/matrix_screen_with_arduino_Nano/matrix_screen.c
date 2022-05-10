

#include "U8g2lib.h"
#include "TimerOne.h"
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
 

U8G2_MAX7219_32X8_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 7, /* data=*/ 5, /* cs=*/ 6, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);
int Minute = 15;
int Second = 0;
int Hour = 11;
void timerIsr(){
  Second++;
  if (Second > 59)
  {
    Second = 0;
    Minute++;   
    if (Minute > 59)
      {Minute = 0;
        Hour++;
    if(Hour > 24)
      Hour=0;
      }
  } 

}
void setup(void) {
  u8g2.begin();
   Timer1.initialize(1000000);
  Timer1.attachInterrupt( timerIsr );

}
 
void loop(void) {
  

  u8g2.clearBuffer();  
  char Hou[3];
  char Min[3];
  itoa(Hour,Hou,10); 
  itoa(Minute,Min,10);
  u8g2.setFont(u8g2_font_victoriabold8_8r);
  u8g2.drawStr(0,7,Hou);
  u8g2.drawStr(16,7,Min); 
  u8g2.sendBuffer();

  delay(1000);  
  
}
