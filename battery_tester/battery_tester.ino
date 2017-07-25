#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>
//#define OLED_RESET 4
Adafruit_SSD1306 display;

/**************Run options***************************/
#define DEBUG


#define VoltageSensePin        A0
#define CurrentSensePin        A1
#define VoltageReference       A2

/********************Button & Menu Variable*****************/

Button ButtonBottomLeft  = Button(6,PULLUP);     //6
Button ButtonBottomRight = Button(7,PULLUP);    //7

Button ButtonTopLeft     = Button(8,PULLUP);        //8
Button ButtonTopRight    = Button(9,PULLUP);       //9
int MenuPage = 0;                             // 0 Mainpage   1 TestPage
int CurrentStep = 100;                         //5A
int VoltageChgStep = 10;                      // 0.1v
/*Sensor variable************************/

#define CurrnetSensorOffset    (1024*(0.5/5.0))  //default 0.5v
float CutOffVoltage = 1400;       //14.00V
float CurrentSetPoint = 6000;     //60.00A
float Voltage = 0.0;
float Current = 0.0;
float RefVoltage = 0.0;
float   Temp    = 0;

void setup()   {                
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.display();
  delay(200);
  display.clearDisplay();
  delay(500);
  GetSensor();
  StartupScreen();
}
void loop() {
  UpdateScreen();
  ChangeSetPoint();
}
void ChangeSetPoint(){
  if ( MenuPage ==0 ){
    int CurrentChangeReq = CurrentSetPoint;
    int VoltageSetPointReq = CutOffVoltage;
    //voltage change
    if ( ButtonTopLeft.isPressed()){
      VoltageSetPointReq = VoltageSetPointReq - VoltageChgStep;
    }
    if ( ButtonTopRight.isPressed()  ){
      VoltageSetPointReq = VoltageSetPointReq + VoltageChgStep;
    }
    CutOffVoltage  = VoltageSetPointReq;
    // current change
    if ( ButtonBottomLeft.isPressed() ){
      CurrentChangeReq = CurrentChangeReq - CurrentStep;
    }
    if ( ButtonBottomRight.isPressed() ){
      CurrentChangeReq = CurrentChangeReq + CurrentStep;
    }
      CurrentSetPoint  = CurrentChangeReq;
  }
}
void UpdateScreen(){
  display.clearDisplay();
  StartupScreen();
  display.display();
}
void TestScreen(){
    MenuPage = 1;                                    // in test page;
    displayChar(10,0,"BAT   SET",2);                //5x8*2  finish at col 16
    displayNumber(0,16,Voltage,2);                    //Bat Voltage
    displayChar(61,24,"V",1);                    //Bat Voltage
    displayNumber(68,16,(CutOffVoltage/100),2);   //Set voltage
    // display Current
    displayNumber(0,32,Current,2);                    //Bat Voltage
    displayChar(61,36,"A",1);                    //Bat Voltage/
    displayNumber(68,32,(CurrentSetPoint/100),2);   //Set voltage
    // display the temp
    display.setCursor(0,48);
    display.println(Temp,1);
    displayChar(45,48,"c",2);
    
}
void StartupScreen(){
/*  -BAT---SET- *    
 *  V12.000v    
    A100.00A
    **Temp :12.00C
         START   
*/  
    MenuPage = 0;                                    // in main page;
  //  display.clearDisplay();
    displayChar(10,0,"BAT   SET",2);                //5x8*2  finish at col 16
    // display Voltage
    displayNumber(0,16,Voltage,2);                    //Bat Voltage
    displayChar(61,24,"V",1);                    //Bat Voltage
    displayNumber(68,16,(CutOffVoltage/100),2);   //Set voltage
    // display Current
    displayNumber(0,32,Current,2);                    //Bat Voltage
    displayChar(61,36,"A",1);                    //Bat Voltage/
    displayNumber(68,32,(CurrentSetPoint/100),2);   //Set voltage
    // display the temp
    display.setCursor(0,48);
    display.println(Temp,1);
    displayChar(45,48,"c",2);
#ifdef DEBUG
    displayNumber(80,56,RefVoltage,1);
#endif
  //  display.display();
}
//Size 2 can diplay 4 line . each can display 10 numbers
//
void displayChar( int x, int y,char *Char,int Size ){
  display.setTextSize(Size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println(Char);
  //display.display();
}
void displayNumber(int x, int y, float Number,int Size){
 // String temp;
  display.setTextSize(Size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.print(Number);
  //display.display();
}
void GetSensor(){
  long V = readADC(VoltageSensePin);  // get voltage
  V = map(V,0,1023,0,32000);          //0-32v
  Voltage = V/1000;
  long A = readADC(CurrentSensePin);  // get current
  if ( A < CurrnetSensorOffset){
    A = CurrnetSensorOffset;
  }
  A = map(A,CurrnetSensorOffset,1023,0,10000);          //0-100A
  Current = A/100;
  long X = readADC(VoltageReference);
  RefVoltage = map(X,0,1023,0,4740);          //0-100A
  printDebugMsg("Refence: ",RefVoltage/1000);
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

