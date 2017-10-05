#include <Wire.h>

#include <DallasTemperature.h>

//Set up for Temp sensor
#define ONE_WIRE_BUS A1
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature TempSensor(&oneWire);



#define VoltageSensePin        A3
#define CurrentSensePin        A2

void initSensor()
{
 TempSensor.begin();
 TempSensor.setResolution(9);
 GetSensor();
 analogReference(AR_EXTERNAL);
 analogReadResolution(12);
 }

void GetSensor(){
  Voltage = readADC(VoltageSensePin);  // get voltage
  printDebugMsg("Voltage: ",Voltage);
  Voltage = map(Voltage,0,4096,0,29080);          //0-32v
  Voltage = Voltage/1000.0;  
  Currents = readADC(CurrentSensePin);  // get current
  CurrentDisplay = Currents;
  
  if ( Currents < CurrnetSensorOffset){
    Currents = CurrnetSensorOffset;
  }
  Currents = map(Currents,CurrnetSensorOffset,4096,0,10000);          //0-100A
  Currents = Currents/100.0;
  printDebugMsg("Current: ",Currents);
 
  TempSensor.setWaitForConversion(false);  // makes it async
  TempSensor.requestTemperatures();
  TempSensor.setWaitForConversion(true);
  Temp = TempSensor.getTempCByIndex(0);      // get Temperature

  if (TestStart){
   //void WirteToSd(float Voltage, float Current, float Temp, long Time)
    WirteToSd( Voltage,Currents,Temp,TotalTime );       // write to the SD card.
  }

}

// read values
// to average
float readADC(int adcPin) 
{
    long sampleTime = 50;
    float ADCraw;
    for (int i = 0; i < sampleTime; i++) 
    {
        ADCraw += analogRead(adcPin);
        delay(1);
    }
    ADCraw = ADCraw/sampleTime; // average  

    return ADCraw;

}
