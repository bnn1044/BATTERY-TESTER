#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Button.h>
//#define OLED_RESET 4
Adafruit_SSD1306 display;

#define VoltageSensePin        A0
#define CurrentSensePin        A1
#define TempSensePin           A2

/*#define ButtonBottomleft       6
#define ButtonBottomRight      7
#define ButtonTopLeft          8
#define ButtonTopRight         9*/

Button ButtonBottomleft = Button(6,PULLUP);
Button ButtonBottomRight = Button(7,PULLUP);
Button ButtonTopLeft = Button(8,PULLUP);
Button ButtonTopRight = Button(9,PULLUP);

#define ADCreference           5
#define CurrnetSensorOffset    (1024*(0.5/5.0))  //default 0.5v
int CutOffVoltage = 1400;       //14.00V
int CurrentSetPoint = 4000;     //40.00A
float Voltage = 0.0;
float Current = 0.0;
int Temp    = 0;

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
  StartupScreen();
  delay(10);
}
void StartupScreen(){
/*  -BAT---SET- *    
 *  V12.000v    
    A100.00A
    **Temp :12.00C
         START   
*/
    display.clearDisplay();
    GetSensor();
    displayChar(10,0,"BAT   SET",2);                //5x8*2  finish at col 16
    // display Voltage
    displayNumber(0,16,Voltage,2);                    //Bat Voltage
    displayChar(61,24,"V",1);                    //Bat Voltage
    displayNumber(68,16,(CutOffVoltage/100),2);   //Set voltage
    // display Current
    displayNumber(0,32,Current,2);                    //Bat Voltage
    displayChar(61,36,"A",1);                    //Bat Voltage
    displayNumber(68,32,(CurrentSetPoint/100),2);   //Set voltage
    // display the temp
    displayNumber(0,56,Temp,1);                    //Bat Voltage
    displayChar(32,56,"C",1);
    displayChar(65,48,"START",2);
    display.display();
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
  //temp = Number + unit;
  display.setTextSize(Size);
  display.setTextColor(WHITE);
  display.setCursor(x,y);
  display.println( Number );
  //display.display();
}
void GetSensor(){
  // analogRead(). It defaults to 10 bits (returns values between 0-1023)
  
  long V = readADC(VoltageSensePin);  // get voltage
  printDebugMsg("Voltage: ",V );
  V = map(V,0,1023,0,32000);          //0-32v
  Voltage = V/1000;
  
  long A = readADC(CurrentSensePin);  // get current
  printDebugMsg("Current: ",A);
  if ( A < CurrnetSensorOffset){
    A = CurrnetSensorOffset;
  }
  A = map(A,CurrnetSensorOffset,1023,0,10000);          //0-100A
  Current = A/100;
  
  //Serial.println(Voltage);
}

void printDebugMsg(String Temp, int number){
  String temp1;
  temp1 = Temp + number;
  Serial.println(temp1);
}


// read values
// to average
// to normisation
// do statistic

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

