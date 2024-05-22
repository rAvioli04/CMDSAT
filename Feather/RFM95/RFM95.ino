#include <Wire.h>
#include "wiring_private.h"

TwoWire sercom0Wire(&sercom3,20,21);
TwoWire sercom4Wire(&sercom4,A1,A2);


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  Serial.flush();
  while (!Serial) {}  
  Serial.println("Serial Ready");


  sercom0Wire.begin();
  sercom4Wire.begin();
  
  pinPeripheral(20,PIO_SERCOM);
  pinPeripheral(21,PIO_SERCOM);
  pinPeripheral(A1,PIO_SERCOM_ALT );
  pinPeripheral(A2,PIO_SERCOM_ALT );

  sercom0Wire.setClock(400000);
  sercom4Wire.setClock(400000);
  
  Serial.println("SERCOM0 Ready");

//  sercom0Wire.beginTransmission(0x68);
//  uint8_t whoAmI[] = {0x75};
//  sercom0Wire.write(whoAmI,1);
//  Serial.println(sercom0Wire.endTransmission(false));
//  delay(1000);
//  Serial.println("SERCOM0 Reset");

  uint8_t deviceReset[] = {0x6B, 0x07};
  uint8_t sensorReset[] = {0x68, 0x80};  
  uint8_t sampleRateDivider[] = {0x19, 0x00};
  uint8_t filterConfig[] = {0x1A, 0x00}; 
  uint8_t gyroConfig[] = {0x1B, 0x08}; 
  uint8_t accelerometerConfig[] = {0x1C, 0x00}; 
  uint8_t powerManagement[] = {0x6B, 0x01};


  // SERCOM0
  sercom0Wire.beginTransmission(0x68);
  sercom0Wire.write(deviceReset,2);
  Serial.println(sercom0Wire.endTransmission());
  Serial.println("SERCOM0 Reset");
  
  sercom0Wire.beginTransmission(0x68); 
  sercom0Wire.write(sensorReset,2);
  sercom0Wire.endTransmission();
  Serial.println("SERCOM0 Sensors Reset");

  sercom0Wire.beginTransmission(0x68);         
  sercom0Wire.write(sampleRateDivider,2);
  sercom0Wire.endTransmission();
  Serial.println("SERCOM0 Sample Rate Divider Set");

  sercom0Wire.beginTransmission(0x68);      
  sercom0Wire.write(filterConfig,2);
  sercom0Wire.endTransmission();
  Serial.println("SERCOM0 Filter Set");  

  sercom0Wire.beginTransmission(0x68); 
  sercom0Wire.write(gyroConfig,2);
  sercom0Wire.endTransmission();
  Serial.println("SERCOM0 Gyro Configured");

  sercom0Wire.beginTransmission(0x68);   
  sercom0Wire.write(accelerometerConfig,2);
  sercom0Wire.endTransmission();
  Serial.println("SERCOM0 Accelerometer Configured");  

  sercom0Wire.beginTransmission(0x68);
  sercom0Wire.write(powerManagement,2);
  sercom0Wire.endTransmission();
  Serial.println("SERCOM0 Power Management Configured");  



  // SERCOM4   
  sercom4Wire.beginTransmission(0x68);
  sercom4Wire.write(deviceReset,2);
  Serial.println(sercom4Wire.endTransmission());
  Serial.println("SERCOM4 Reset");
  
  sercom4Wire.beginTransmission(0x68); 
  sercom4Wire.write(sensorReset,2);
  sercom4Wire.endTransmission();
  Serial.println("SERCOM4 Sensors Reset");

  sercom4Wire.beginTransmission(0x68);         
  sercom4Wire.write(sampleRateDivider,2);
  sercom4Wire.endTransmission();
  Serial.println("SERCOM4 Sample Rate Divider Set");

  sercom4Wire.beginTransmission(0x68);      
  sercom4Wire.write(filterConfig,2);
  sercom4Wire.endTransmission();
  Serial.println("SERCOM4 Filter Set");  

  sercom4Wire.beginTransmission(0x68); 
  sercom4Wire.write(gyroConfig,2);
  sercom4Wire.endTransmission();
  Serial.println("SERCOM4 Gyro Configured");

  sercom4Wire.beginTransmission(0x68);   
  sercom4Wire.write(accelerometerConfig,2);
  sercom4Wire.endTransmission();
  Serial.println("SERCOM4 Accelerometer Configured");  

  sercom4Wire.beginTransmission(0x68);
  sercom4Wire.write(powerManagement,2);
  sercom4Wire.endTransmission();
  Serial.println("SERCOM4 Power Management Configured");  
}


void loop() {
  int receivedBytes = 0;
  // SERCOM0
  sercom0Wire.beginTransmission(0x68);
  sercom0Wire.write(0x3B);
  sercom0Wire.endTransmission(false);
  sercom0Wire.requestFrom(0x68, 6, true);

  sercom0Wire.beginTransmission(0x68);
  sercom0Wire.write(0x3B);
  sercom0Wire.endTransmission(false);
  receivedBytes = sercom0Wire.requestFrom(0x68, 6, true);

  uint16_t msB0 = sercom0Wire.read();
  uint16_t lsB0 = sercom0Wire.read();

  // SERCOM4
  sercom4Wire.beginTransmission(0x68);
  sercom4Wire.write(0x3B);
  sercom4Wire.endTransmission(false);
  sercom4Wire.requestFrom(0x68, 6, true);

  sercom4Wire.beginTransmission(0x68);
  sercom4Wire.write(0x43);
  sercom4Wire.endTransmission(false);
  receivedBytes = sercom4Wire.requestFrom(0x68, 6, true);

  uint16_t msB4 = sercom4Wire.read();
  uint16_t lsB4 = sercom4Wire.read();
  
  //Serial.println((((double)(int16_t)((msB<<8) | lsB)))/16384.);
}
