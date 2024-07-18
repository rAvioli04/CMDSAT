#include <I2C_DMAC.h>
#include <wiring_private.h>

#define MPU6050_ADDRESS     0x68                                // Device address when ADO = 0
#define PWR_MGMT_1          0x6B
#define SAMPLE_RATE_DIVIDER 0x19
#define CONFIG              0x1A
#define GYRO_CONFIG         0x1B
#define ACCEL_CONFIG        0x1C
#define ACCEL_XOUT_H        0x3B
#define GYRO_XOUT_H         0x43
#define WHO_AM_I            0x75

uint32_t ti;
uint8_t data0[18];
uint8_t data1[18];
uint8_t data[36];
volatile boolean sercomError = false;

I2C_DMAC I2C1(&sercom1, 11, 13);                              // Create (instantiate) a second I2C1 object with sercom2, on pins D3 and D4

void setup() 
{
  pinPeripheral(11, PIO_SERCOM);                         // Assign D3 to SERCOM2 I2C SDA
  pinPeripheral(13, PIO_SERCOM); 
  
  Serial.begin(115200);                                  // Activate the native USB port
while(!Serial);                                        // Wait for the native USB to be ready
//  Serial.println("MPU6050 0");

  
  I2C.begin(400000);                                        // Start I2C bus at 400kHz          
  I2C.dmacInterruptsOn();
  I2C.readByte(MPU6050_ADDRESS, WHO_AM_I);           // Read the WHO_AM_I register 
//  while(I2C.readBusy);    
  I2C.writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x07);         // Wake up the MPU6050 device
  delay(100);                                               // Wait for the MPU6050 to settle   
  I2C.writeByte(MPU6050_ADDRESS,PWR_MGMT_1,0x80);
  delay(100); 
  I2C.writeByte(MPU6050_ADDRESS,PWR_MGMT_1,0x01);
  delay(100); 
  I2C.writeByte(MPU6050_ADDRESS, CONFIG, 0x00);              // Set the gyro/accel filter to 20Hz  
  I2C.writeByte(MPU6050_ADDRESS, SAMPLE_RATE_DIVIDER, 0x00);
  I2C.writeByte(MPU6050_ADDRESS, GYRO_CONFIG, 0x1 << 3);    // Set full scale range to +/-500 degrees/s 
  I2C.writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x1 << 3);   // Set full scale range to +/-4g

  I2C.initWriteRegAddr(MPU6050_ADDRESS, ACCEL_XOUT_H);       // Set-up DMAC to write to MPU6050 register pointer
  I2C.initReadBytes(MPU6050_ADDRESS, data, 14);   // Set DMAC to read the data
//  I2C.attachReadCallback(mpu0Callback);                    // Attach a read callback function to I2C
//  Serial.println("MPU6050 0 Done");
  
//  Serial.println("MPU6050 1");
  I2C1.begin(400000);                                       // Start I2C bus at 400kHz 
  I2C1.dmacInterruptsOn();
//  pinPeripheral(3, PIO_SERCOM_ALT);                         // Assign D3 to SERCOM2 I2C SDA
//  pinPeripheral(4, PIO_SERCOM_ALT);                         // Assign D4 to SERCOM2 I2C SCL
  I2C1.setWriteChannel(5);                                  // Set the I2C1 DMAC write channel to 2
  I2C1.setReadChannel(6);                                   // Set the I2C1 DMAC read channel to 3
  I2C1.writeByte(MPU6050_ADDRESS, PWR_MGMT_1, 0x07);         // Wake up the MPU6050 device
  delay(100);                                               // Wait for the MPU6050 to settle   
  I2C1.writeByte(MPU6050_ADDRESS,PWR_MGMT_1,0x80);
  delay(100); 
  I2C1.writeByte(MPU6050_ADDRESS,PWR_MGMT_1,0x01);
  delay(100); 
  I2C1.writeByte(MPU6050_ADDRESS, CONFIG, 0x00);              // Set the gyro/accel filter to 20Hz  
  I2C1.writeByte(MPU6050_ADDRESS, SAMPLE_RATE_DIVIDER, 0x00);
  I2C1.writeByte(MPU6050_ADDRESS, GYRO_CONFIG, 0x1 << 3);    // Set full scale range to +/-500 degrees/s 
  I2C1.writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x1 << 3);   // Set full scale range to +/-4g  // Set full scale range to +/-4g
  I2C1.initWriteRegAddr(MPU6050_ADDRESS, ACCEL_XOUT_H);      // Set-up DMAC to write to MPU6050 register pointer
  I2C1.initReadBytes(MPU6050_ADDRESS, data+18, 14);// Set DMAC to read the data
//  I2C1.attachReadCallback(mpu1Callback);                   // Attach a read callback function to I2C1
//  Serial.println("MPU6050 1 Done");
}

void loop() 
{ 
  I2C.readBytes(MPU6050_ADDRESS,ACCEL_XOUT_H,data,14);
  ti = micros();
  data[14] = ti >> 8;
  data[15] = ti >> 0;
  data[16] = '0';
  data[17] = '\n';

  I2C1.readBytes(MPU6050_ADDRESS,ACCEL_XOUT_H,data + 18,14);
  (data+18)[14] = ti >> 8;
  (data+18)[15] = ti >> 0;
  (data+18)[16] = '1';
  (data+18)[17] = '\n';
//  // Add your concurrent code here...
  while(I2C.readBusy) {
//    Serial.println("I2C is stuck");                   
  }
  while(I2C1.readBusy) {
//    Serial.println("I2C1 is stuck");
  }

  Serial.write(data,36);
}
