#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_FeatherOLED.h>

// define for dplay
Adafruit_FeatherOLED display = Adafruit_FeatherOLED();

void initDisplay(){
   display.init();
}

void TestScreen(){
  /*12.00 v 14.00
   *45.0  A 40.0
   *25.0  C 65.0
   *10000 mah 00:00
   */
    display.clearDisplay();
    displayNumber(0,0,Voltage,2);                    //Bat Voltage
    displayChar(38,0,"V",1);                         //Bat Voltage
    if ( SettingItem == setVoltageItem ){
      display.setCursor(46,0);
      display.print(">");
    }
    displayNumber(55,0,(CutOffVoltage/100.0),2);      //Set voltage
    // display Current
    displayNumber(0,8,Currents,1);                    //Bat current
    displayChar(38,8,"A",1);                           //Bat current
    /*if ( SettingItem == setCurrentItem ){
      display.setCursor(46,8);
      display.print(">");
    }*/
    displayNumber(55,8,(CurrentSetPoint/10.0),1);      //Set current
    displayNumber(100,8,CurrnetSensorOffset,0);      //Set current

    displayNumber(100,16,CurrentDisplay,0);      //Set current
    //displayNumber(70,25,timeStamp,0);
    
    // display the temp
    displayNumber(0,16,Temp,1);                       //temp
    displayChar(38,16,"C",1);                           //temp
     if ( SettingItem == setTempItem ){
      display.setCursor(46,16);
      display.print(">");
    }
    displayNumber(55,16,(TempsetPoint/10.0),1);         //Set set temp
    // display mah
    displayNumber(0,24,mah,0);                         //mah
    //display time
    display.display();
}
void displayChar( int x, int y,char *Char,int Size ){
  display.setCursor(x,y);
  display.println(Char);
}
void displayNumber(int x, int y, float Number,int decPoint){
  display.setCursor(x,y);
  display.println(Number,decPoint);
}
void printDebugMsg(String Temp, uint16_t number){
  String temp1;
  temp1 = Temp + number;
  Serial.println(temp1);
}
