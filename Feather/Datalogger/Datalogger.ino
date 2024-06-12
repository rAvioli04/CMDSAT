//#include <SPI.h>
//#include "SdFat.h"
//#include "sdios.h"
//#define SD_FAT_TYPE 1
//#define SPI_SPEED SD_SCK_MHZ(4)
#define BUFF_LEN 1024
//String filename = "data0.txt";
//const int8_t DISABLE_CHIP_SELECT = 8;

//SdFat32 sd;
//File32 file;


#include <I2C_DMAC.h>
#include <wiring_private.h>
#define MPU6050_ADDRESS 0x68                                
#define PWR_MGMT_1      0x6B
#define CONFIG          0x1A
#define GYRO_CONFIG     0x1B
#define ACCEL_CONFIG    0x1C
#define GYRO_XOUT_H     0x3B
#define WHO_AM_I        0x75

uint8_t data[6];
uint8_t data1[6];
uint8_t buff[12*BUFF_LEN];
int i = 0;
int high = 0;
int waitCount = 0;

unsigned long tick;
I2C_DMAC I2C1(&sercom1, 11, 13); 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // Open serial communications and wait for port to open:
//  Serial.begin(115200);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  
//  Serial.println("Configuring I2C");
  I2C.begin(400000);                                 // Start I2C bus at 400kHz
  
  I2C.setWriteChannel(10);                                  // Set the I2C1 DMAC write channel to 2
  I2C.setReadChannel(11);                                   // Set the I2C1 DMAC read channel to 3   
  I2C.dmacInterruptsOn();
  I2C.readByte(MPU6050_ADDRESS, WHO_AM_I);           // Read the WHO_AM_I register 
  while(I2C.readBusy);                               // Wait for synchronization  
//  Serial.println(I2C.getData(), HEX);             // Output the result
//  Serial.println("Wake up the MPU6050 device");    
  I2C.writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);
  delay(100);
//  Serial.println("Wait for the MPU6050 to settle");
//  Serial.println("Set the gyro/accel filter to 20Hz");
  I2C.writeByte(MPU6050_ADDRESS, CONFIG, 0x4);
//  Serial.println("Set full scale range to +/-500 degrees/s");
  I2C.writeByte(MPU6050_ADDRESS, GYRO_CONFIG, 0x1 << 3); 
//  Serial.println("Set full scale range to +/-4g");  
  I2C.writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x1 << 3);
//  Serial.println("Set-up DMAC to write to MPU6050 register pointer");  
  I2C.initWriteRegAddr(MPU6050_ADDRESS, GYRO_XOUT_H);
//  Serial.println("Set DMAC to read the data");
  I2C.initReadBytes(MPU6050_ADDRESS, data, sizeof(data));

  I2C.attachReadCallback(mpu0Callback);
  I2C.attachSercomErrorCallback(sercomErrorCallback);
  I2C.attachDmacErrorCallback(dmacErrorCallback);
//  Serial.println("Finished Configuring I2C");

//  Serial.println("Configuring I2C1");
  I2C1.begin(400000);                                       // Start I2C bus at 400kHz 
  pinPeripheral(11, PIO_SERCOM);                         // Assign D3 to SERCOM2 I2C SDA
  pinPeripheral(13, PIO_SERCOM);                         // Assign D4 to SERCOM2 I2C SCL
  I2C1.setWriteChannel(2);                                  // Set the I2C1 DMAC write channel to 2
  I2C1.setReadChannel(3);                                   // Set the I2C1 DMAC read channel to 3
  I2C1.dmacInterruptsOn();

  I2C1.readByte(MPU6050_ADDRESS, WHO_AM_I);           // Read the WHO_AM_I register 
  while(I2C1.readBusy);                               // Wait for synchronization  
//  Serial.println(I2C1.getData(), HEX);             // Output the result
//  Serial.println("Wake up the MPU6050 device");  
  I2C1.writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x01);        // Wake up the MPU6050 device
  delay(100);                                               // Wait for the MPU6050 to settle   
  I2C1.writeByte(MPU6050_ADDRESS, CONFIG, 0x4);             // Set the gyro/accel filter to 20Hz  
  I2C1.writeByte(MPU6050_ADDRESS, GYRO_CONFIG, 0x1 << 3);   // Set full scale range to +/-500 degrees/s  
  I2C1.writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x1 << 3);  // Set full scale range to +/-4g
  I2C1.initWriteRegAddr(MPU6050_ADDRESS, GYRO_XOUT_H);      // Set-up DMAC to write to MPU6050 register pointer
  I2C1.initReadBytes(MPU6050_ADDRESS, data1, sizeof(data1));// Set DMAC to read the data
  I2C1.attachReadCallback(mpu1Callback);                   // Attach a read callback function to I2C1
  I2C1.attachSercomErrorCallback(sercomErrorCallback);
  I2C1.attachDmacErrorCallback(dmacErrorCallback);
//  Serial.println("Finished Configuring I2C1");
  
//  pinMode(SS,OUTPUT);
//  
////  Serial.println("Initializing SD card...");
//  pinMode(DISABLE_CHIP_SELECT, OUTPUT);
//  digitalWrite(DISABLE_CHIP_SELECT, HIGH);

//  while (!sd.begin(SS,SPI_SPEED)) {
////    Serial.println("Card failed, or not present");
//    if (sd.card()->errorCode()) {
//      Serial.println("hey!");
//    }
//    // don't do anything more:
//    delay(1000);
//  }
////  Serial.println("card initialized.");
//
//  //O_CREAT | O_WRITE
//  file.open(filename.c_str(), O_CREAT | O_WRITE);
//  byte header[] = {0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,'\n'};
//  file.write(header,12);
//  file.flush();

//  Serial.println(sizeof(unsigned long));
  digitalWrite(LED_BUILTIN, LOW);
  tick = micros();
}

void loop() {
  I2C.readBytes(MPU6050_ADDRESS, GYRO_XOUT_H, data, sizeof(data));
  //delay(50);

  I2C1.readBytes(MPU6050_ADDRESS, GYRO_XOUT_H, data1, sizeof(data1));
  //delay(50);

  
  while(I2C.readBusy || I2C1.readBusy) {
    if (waitCount < 10000) {
      waitCount++;
    } else {
      waitCount = 0;
//      Serial.println("waiting");
    }
    
  }
     

  if (i == BUFF_LEN) {
    unsigned long newTick = micros();
//    Serial.println(String(12*BUFF_LEN) + " Bytes in " + String(newTick - tick) + " us");
    tick = newTick;
  if (high) {
    digitalWrite(LED_BUILTIN, LOW);
    high = 0;
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    high = 1;
  }
//    if (file.isOpen()) {
//      file.write(buff,12*BUFF_LEN);
//      file.flush();
//      //Serial.println("flushed");
//    } else {
////      Serial.println("error opening file");
//    }
    i=0;
  
  }
}

void mpu0Callback()
{
  mpuCallback(0);
}
void mpu1Callback()
{
  mpuCallback(1);
}

void mpuCallback(int mpu)
{
//  Serial.println("mpu " + String(mpu) + ": " + String(i));
  if (i < BUFF_LEN) {
    unsigned long ti = micros();
    buff[0+12*i] = 0x30 + mpu;
    buff[1+12*i] = ti >> 24;
    buff[2+12*i] = ti >> 16;
    buff[3+12*i] = ti >> 8;
    buff[4+12*i] = ti;

    for (int j = 0; j < 6; j++) {
      if (mpu == 0) {
        buff[5+j+12*i] = data[j];
      } else {
        buff[5+j+12*i] = data1[j];
      }
    }
    buff[11+12*i] = '\n';
    i++;
  }
  return;
}

void sercomErrorCallback() {
//  Serial.println("Sercom Error!");
}
void dmacErrorCallback() {
//  Serial.println("Dmac Error!");
}
