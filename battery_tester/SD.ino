#include <SPI.h>
#include <SD.h>

// Set up for log file
#define cardSelect 4
File logfile;

// blink out an error code
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(200);
      digitalWrite(13, LOW);
      delay(200);
      
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
    Serial.print("Error "); 
  }
}
// This line is not needed if you have Adafruit SAMD board package 1.6.2+
//   #define Serial SerialUSB

void SetupSD() {
  
  Serial.println("\r\nAnalog logger test");
  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
 char filename[15];
  strcpy(filename, "ANALOG00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = '0' + i/10;
    filename[7] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }
  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    error(3);
  }
  Serial.print("Writing to "); 
  Serial.println(filename);
  Serial.println("Ready!");
  logfile.println("  Voltage    Current   Temp   Time  ");
  logfile.flush();
   //logfile.println("  16.85      40.2      25.5   0000  ");
}
void WirteToSd(float Voltage, float Current, float Temp, long Time){
  logfile.print("   "); 
  logfile.print(Voltage);
  logfile.print("      ");
  logfile.print(Current);
  logfile.print("      ");
  logfile.print(Temp);
  logfile.print("   ");
  logfile.println(Time);        
  logfile.flush();
}

