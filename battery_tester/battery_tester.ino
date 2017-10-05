
/**************Run options***************************/
//#define DEBUG

#define RelayOutput            11
#define BatteryRecoveryTime  10000     //unit in ms
#define MinTestTime          1000      //unit in ms
//void WirteToSd(float Voltage, float Current, float Temp, long Time);

int ChangeStep = 10;                   
/*Sensor variable************************/
float CutOffVoltage = 1400;       //14.00V
float CurrentSetPoint = 100;      //10.00A
float TempsetPoint    =650;       //temperature setpoint
float CurrnetSensorOffset = 400;
float Voltage = 0.0;              //voltage
float Currents  = 0.0;              //current
float RefVoltage = 0.0;           //reference voltage
float Temp    = 0;                //Temperature

float BurstTime[20];
int bursts = 0;
float mah = 1300;                  //mAH
long SaveMillis = 0;               //time
long CurrentMillis = 0;
boolean TestStart = 0;            //Test Start; 0 stop or test done ,1 test start;
boolean TestPause = 0;
long SaveTime = 0;                // temperary Time save
float CurrentDisplay;
float TestTime;
float TotalTime;
boolean RelayStatus;            // check relay status
// Menu Items.
#define setVoltageItem 0    
//#define setCurrentItem 1
#define setTempItem    1 
int SettingItem = 0;   // 0 set voltage, 1 set current, 2 setTemp

void setup(){
 Serial.begin(115200);
 initDisplay();
 initSensor();
 //CurrnetSensorOffset = readADC(CurrentSensePin);  // get current offset every time power up the system
 SetupSD();                                       // setupSD card and and create a new file on the SD CARD
 pinMode(8, OUTPUT);
 pinMode(13, OUTPUT);
 pinMode(RelayOutput,OUTPUT);
}
void loop() {
   int i;
   digitalWrite(8, HIGH);
   GetSensor();
   GetSetPoint();
   TestScreen();
   GetStartKey();
   
   // if test key press start the test cycle
   if( TestStart == 1 ){
    testBattery();
   }
  // WirteToSd(Voltage,Currents,Temp,timeStamp/100000);
}
void CheckBatteryStutas(){
  if ( Voltage < CutOffVoltage ){               // Voltage < cut off voltage
      Relay(0);                                  // when Voltage less than cut off voltage turn off relay
      if ( TestTime >= MinTestTime){
        bursts = bursts+1;                      // increase the test times.
        BurstTime[ bursts ] = TestTime;         // rerocrd current test time
        TestTime = 0;                           // reset the test time for next test
        long RevocerTime=0;                     // start count down the battery recover time
        SaveMillis = millis();
        while(RevocerTime < BatteryRecoveryTime){
          RevocerTime = RevocerTime + ( millis() - SaveMillis);
          TotalTime = TotalTime +  ( millis() - SaveMillis)/1000;
          SaveMillis = millis();
          GetSensor();                         // get sensor data
          TestScreen();                       // Show on the display
          GetStartKey();                      // check the start key status
        }
      }else{
        //Test done
        Relay(0);
        TestStart = 0;
      }
  }
  if ( Temp > TempsetPoint ){                        // over temp
     Relay(0);
     TestStart = 0;                                 //Stop the test right away for safety 
  }
}
void testBattery()
{
  int i = 0;
  Relay(1);
  SaveMillis = millis();
  while( TestStart == 1 ){
      CurrentMillis = millis() - SaveMillis;            // get the time stamp on the micro second.
      TestTime = TestTime + (CurrentMillis/1000);
      TotalTime = TotalTime +  ( millis() - SaveMillis)/1000;
      mah = mah + ( CurrentMillis/3600 ) * Currents;  // calculate the mAH   
      SaveMillis = millis();
      GetSensor();                                    // get sensor data
      TestScreen();                                   // Show on the display
      GetStartKey();                                  // check the start key status
      CheckBatteryStutas();                           // check the battery condition
  }
}
void Relay(int State){
  if ( State == 1 ){
    digitalWrite(RelayOutput, HIGH);            // RELAY ON
    RelayStatus = true;
  }else if( State ==0 ){
    digitalWrite(RelayOutput, LOW);         // RELAY OFF
    RelayStatus = false;
  }
}


