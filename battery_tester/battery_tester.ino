#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>
#include <SoftwareSerial.h>
#include <PID_v1.h>
//#define OLED_RESET 4
Adafruit_SSD1306 display;

SoftwareSerial LogSerial(3, 2); // RX, TX


//Define Variables we'll be connecting to
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

/**************Run options***************************/
//#define DEBUG


#define VoltageSensePin        A0
#define CurrentSensePin        A1
#define VoltageReference       A2

/********************Button & Menu Variable*****************/

Button Select  = Button(6,PULLUP);     //6
Button Start   = Button(7,PULLUP);    //7
Button Minus   = Button(8,PULLUP);        //8
Button Plus    = Button(9,PULLUP);       //9

int ChangeStep = 10;                         //5A
/*Sensor variable************************/

#define CurrnetSensorOffset    (1023*(0.5/5.0))  //default 0.5v
float CutOffVoltage = 1400;       //14.00V
float CurrentSetPoint = 600;     //60.00A
float TempsetPoint    =650;       //  temperature setpoint
float Voltage = 0.0;              //voltage
float Current = 0.0;              //current
float RefVoltage = 0.0;           //reference voltage
float Temp    = 0;                //Temperature
long mah = 1300;                  //mAH
long timeStamp = 0;               //time
boolean TestStart = 0;            //Test Start; 0 stop or test done ,1 test start;
int minites,seconds;              // minites Second
long timetemp = 0;                // temperary Time save

// Menu Items.
#define setVoltage 0    
#define setCurrent 1
#define setTemp    2 
int SettingItem = 0;   // 0 set voltage, 1 set current, 2 setTemp

void setup()   {                
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.display();
  delay(200);
  display.clearDisplay();
  delay(500);
  GetSensor();
  myPID.SetMode(AUTOMATIC);
}

void doMath(){
  
  //calcultate the mah
  //calculate the minites
  //When start the time capture, need to do // timeStamp =millis(); first before calling 
  //compute PID
   Setpoint = CurrentSetPoint;
   Input = Current;
   myPID.Compute();
   analogWrite( 5,Output);
   printDebugMsg("Setpoint: ",Setpoint);
   printDebugMsg("Input: ",Input);
   printDebugMsg("OUTPUTPWM: ",Output);
}
void loop() {
  UpdateScreen();
  GetSensor();
  GetSetPoint();
  if ( GetStartKey()){             //test start
      timetemp = millis();
      timeStamp = 0;
      printDebugMsg("Test start ",TestStart);
      while( TestStart ){
          timeStamp =millis()- timetemp;
          minites = ( timeStamp/1000 ) /60;
          seconds = ( timeStamp/1000 ) % 60;    // get the uptime
          GetSensor();            //read the data
          UpdateScreen();         //Update screen
          doMath();
         // CheckBatteryStutas();
      }
  }
}
boolean CheckBatteryStutas(){
  if ( Voltage < CutOffVoltage ){               // Voltage < cut off voltage
      TestStart = 0;
  }
  if ( Temp > setTemp ){                        // over temp
      TestStart = 0;
  }
  return TestStart;
}
boolean GetStartKey(){
  if ( Start.uniquePress()){
    if ( !TestStart ){
      TestStart = 1;
    }else
      TestStart = 0;
  }
  return TestStart;
}
void GetSetPoint(){
  int CurrentChangeReq = CurrentSetPoint;
  int VoltageSetPointReq = CutOffVoltage;
  int TemperaterReq      = TempsetPoint ;
   if ( Select.uniquePress()){
        SettingItem = SettingItem+1;
        if ( SettingItem >= 3 ){
           SettingItem = 0;
        }
  }
    //voltage change
    if ( Minus.isPressed()){
      switch ( SettingItem ){
        case 0 : 
          VoltageSetPointReq = VoltageSetPointReq - ChangeStep;
          CutOffVoltage  = VoltageSetPointReq;
          break;
        case 1:
          CurrentChangeReq = CurrentChangeReq - ChangeStep;
          CurrentSetPoint = CurrentChangeReq;
          break;
        case 2:
          TemperaterReq = TemperaterReq - ChangeStep;
          TempsetPoint = TemperaterReq;
          break;
      }
    }
    if ( Plus.isPressed()){
       switch ( SettingItem ){
        case 0 : 
          VoltageSetPointReq = VoltageSetPointReq + ChangeStep;
          CutOffVoltage  = VoltageSetPointReq;
          break;
        case 1:
          CurrentChangeReq = CurrentChangeReq + ChangeStep;
          CurrentSetPoint = CurrentChangeReq;
          break;
        case 2:
          TemperaterReq = TemperaterReq + ChangeStep;
          TempsetPoint = TemperaterReq;
          break;
      }
    }
}
void UpdateScreen(){
  display.clearDisplay();
  TestScreen();
  display.display();
}
void TestScreen(){
  /*12.00 v 14.00
   *45.0  A 40.0
   *25.0  C 65.0
   *10000 mah 00:00
   */
    displayNumber(0,0,Voltage,2,2);                    //Bat Voltage
    displayChar(60,8,"V",1);                         //Bat Voltage
    displayNumber(68,0,(CutOffVoltage/100),2,2);      //Set voltage
    if ( SettingItem == setVoltage ){
      display.drawRect(68,0,60,16,1);
    }
    //display.setTextWrap(0);
    // display Current
    displayNumber(0,16,Current,2,1);                    //Bat current
    displayChar(60,23,"A",1);                           //Bat current
    displayNumber(72,16,(CurrentSetPoint/10),2,1);      //Set current
    if ( SettingItem == setCurrent ){
      display.drawRect(70,15,50,17,1);
    }
    // display the temp
    displayNumber(0,33,Temp,2,1);                       //temp
    displayChar(60,40,"C",1);                           //temp
    displayNumber(72,33,(TempsetPoint/10),2,1);         //Set set temp
    if ( SettingItem == setTemp ){
      display.drawRect(70,31,50,17,1);
    }
    // display mah
    displayNumber(0,50,mah,2,0);                         //mah
    //display time
    if ( minites>0 ){
      displayNumber(64,50,minites,2,0);                   //display mintues
      displayChar(85,50,":",2);
    }
    displayNumber(95,50,seconds,2,0);                   //display mintues
}
void displayChar( int x, int y,char *Char,int Size ){
  display.setTextSize(Size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(Char);
  //display.display();
}
void displayNumber(int x, int y, float Number,int Size,int decPoint){
 // String temp;
  display.setTextSize(Size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(Number,decPoint);
  //display.display();
}
void GetSensor(){
  long V = readADC(VoltageSensePin);  // get voltage
  V = map(V,0,1023,0,32000);          //0-32v
  Voltage = V/1000;
#ifdef DEBUG
  printDebugMsg("Voltage",Voltage);
#endif  
  long A = readADC(CurrentSensePin);  // get current
  if ( A < CurrnetSensorOffset){
    A = CurrnetSensorOffset;
  }
  A = map(A,CurrnetSensorOffset,1023,0,10000);          //0-100A
  Current = A/100;
  long X = readADC(VoltageReference);
  RefVoltage = map(X,0,1023,0,4740);          //0-100A
#ifdef DEBUG  
  printDebugMsg("Refence: ",RefVoltage/1000);
#endif  
}
void printDebugMsg(String Temp, uint16_t number){
  String temp1;
  temp1 = Temp + number;
  Serial.println(temp1);
}

// read values
// to average
uint16_t readADC(int adcPin) 
{
    uint16_t ADC;
    for (uint8_t i = 0; i < 10; i++) 
    {
        ADC += analogRead(adcPin);
    }
    ADC = ADC/10; // average  
    return (ADC);
}

