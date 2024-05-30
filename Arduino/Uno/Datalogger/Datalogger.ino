/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library.

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created  24 Nov 2010
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "wiring_private.h"
#include "SdFat.h"
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#define SD_FAT_TYPE 1
#define SPI_SPEED SD_SCK_MHZ(4)
#define BUFF_LEN 500
String filename = "data0.txt";
const int8_t DISABLE_CHIP_SELECT = 8;
//#define CHIP_SELECT 6
//#define SCK 24
//#define MOSI 23
//#define MISO 22
//#undef Sd2PinMap_h
Adafruit_MPU6050 mpu;
File dataFile;
byte buff[12*BUFF_LEN];
int i = 0;
SdFat32 sd;
File32 file;
ArduinoOutStream cout(Serial);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

//  if (!mpu.begin()) {
//    Serial.println("Failed to find MPU6050 chip");
//    while (1) {
//      delay(10);
//    }
//  }

  Wire.begin();  
  Wire.setClock(400000);
  
//  Serial.println("MPU6050 Found!");
//  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
//  Serial.print("Accelerometer range set to: ");
//  switch (mpu.getAccelerometerRange()) {
//  case MPU6050_RANGE_2_G:
//    Serial.println("+-2G");
//    break;
//  case MPU6050_RANGE_4_G:
//    Serial.println("+-4G");
//    break;
//  case MPU6050_RANGE_8_G:
//    Serial.println("+-8G");
//    break;
//  case MPU6050_RANGE_16_G:
//    Serial.println("+-16G");
//    break;
//  }
//
//  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
//  Serial.print("Filter bandwidth set to: ");
//  switch (mpu.getFilterBandwidth()) {
//  case MPU6050_BAND_260_HZ:
//    Serial.println("260 Hz");
//    break;
//  case MPU6050_BAND_184_HZ:
//    Serial.println("184 Hz");
//    break;
//  case MPU6050_BAND_94_HZ:
//    Serial.println("94 Hz");
//    break;
//  case MPU6050_BAND_44_HZ:
//    Serial.println("44 Hz");
//    break;
//  case MPU6050_BAND_21_HZ:
//    Serial.println("21 Hz");
//    break;
//  case MPU6050_BAND_10_HZ:
//    Serial.println("10 Hz");
//    break;
//  case MPU6050_BAND_5_HZ:
//    Serial.println("5 Hz");
//    break;
//  }

  uint8_t deviceReset[] = {0x6B, 0x07};
  uint8_t sensorReset[] = {0x68, 0x80};  
  uint8_t sampleRateDivider[] = {0x19, 0x00};
  uint8_t filterConfig[] = {0x1A, 0x00}; 
  uint8_t gyroConfig[] = {0x1B, 0x08}; 
  uint8_t accelerometerConfig[] = {0x1C, 0x00}; 
  uint8_t powerManagement[] = {0x6B, 0x01};


  // SERCOM0
  Wire.beginTransmission(0x68);
  Wire.write(deviceReset,2);
  Serial.println(Wire.endTransmission());
  Serial.println("SERCOM0 Reset");
  
  Wire.beginTransmission(0x68); 
  Wire.write(sensorReset,2);
  Wire.endTransmission();
  Serial.println("SERCOM0 Sensors Reset");

  Wire.beginTransmission(0x68);         
  Wire.write(sampleRateDivider,2);
  Wire.endTransmission();
  Serial.println("SERCOM0 Sample Rate Divider Set");

  Wire.beginTransmission(0x68);      
  Wire.write(filterConfig,2);
  Wire.endTransmission();
  Serial.println("SERCOM0 Filter Set");  

  Wire.beginTransmission(0x68); 
  Wire.write(gyroConfig,2);
  Wire.endTransmission();
  Serial.println("SERCOM0 Gyro Configured");

  Wire.beginTransmission(0x68);   
  Wire.write(accelerometerConfig,2);
  Wire.endTransmission();
  Serial.println("SERCOM0 Accelerometer Configured");  

  Wire.beginTransmission(0x68);
  Wire.write(powerManagement,2);
  Wire.endTransmission();
  Serial.println("SERCOM0 Power Management Configured");  

  pinMode(SS,OUTPUT);
  //pinPeripheral(SCK,PIO_SERCOM_ALT );
  //pinPeripheral(MISO,PIO_SERCOM_ALT );
  //pinPeripheral(MOSI,PIO_SERCOM_ALT );
  //pinMode(27,OUTPUT);
  
  Serial.println("Initializing SD card...");
  pinMode(DISABLE_CHIP_SELECT, OUTPUT);
  digitalWrite(DISABLE_CHIP_SELECT, HIGH);
  Serial.print("SS: ");
  Serial.println(SS);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
    // see if the card is present and can be initialized:
  while (!sd.begin(SS,SPI_SPEED)) {
    Serial.println("Card failed, or not present");
    if (sd.card()->errorCode()) {
            cout << F(
             "\nSD initialization failed.\n"
             "Is the card correctly inserted?\n"
             "Is chipSelect set to the correct value?\n"
             "Does another SPI device need to be disabled?\n"
             "Is there a wiring/soldering problem?\n");
      cout << F("\nerrorCode: ") << hex << showbase;
      cout << int(sd.card()->errorCode());
      cout << F(", errorData: ") << int(sd.card()->errorData());
      cout << dec << noshowbase << endl;
    }

    // don't do anything more:
    delay(1000);
  }
  Serial.println("card initialized.");
  int j = 0;

//  while(1) {
//    Serial.println(filename);
//    Serial.println(j);
//    bool fileExists = file.exists(filename.c_str());
//    Serial.println(fileExists);
//    if (!fileExists) {
//      break;
//    }
//    j++;
//    filename = "data" + String(j) + ".txt";
//  }
  //O_CREAT | O_WRITE
  file.open(filename.c_str(), O_CREAT | O_WRITE);
  byte header[] = {0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,'\n'};
  file.write(header,12);
  file.flush();

  Serial.println(sizeof(unsigned long));
}

void loop() {
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 6, true);

  unsigned long ti = micros();
  buff[0+12*i] = 0x23;
  buff[1+12*i] = ti >> 24;
  buff[2+12*i] = ti >> 16;
  buff[3+12*i] = ti >> 8;
  buff[4+12*i] = ti;
  for (int j = 0; j < 6; j++) {
    buff[5+j+12*i] = Wire.read();
  }
  buff[11+12*i] = '\n';
  i++;
  if (i == BUFF_LEN) {

    if (file.isOpen()) {
      file.write(buff,12*BUFF_LEN);
      file.flush();
      //Serial.println("flushed");
    } else {
      Serial.println("error opening file");
    }
    i=0;
  }
}
