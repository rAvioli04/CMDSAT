#include <SPI.h>
#include <SD.h>

#define BUFF_LEN 1000

#include <Arduino.h>
#include <I2C_DMAC.h>
#include <wiring_private.h>
#define MPU6050_ADDRESS0 0x68        
#define MPU6050_ADDRESS1 0x69                             
#define PWR_MGMT_1      0x6B
#define CONFIG          0x1A
#define GYRO_CONFIG     0x1B
#define ACCEL_CONFIG    0x1C
#define GYRO_XOUT_H     0x3B
#define WHO_AM_I        0x75

String filename = "data0.txt";
uint8_t data0A[6*BUFF_LEN];
uint8_t data0B[6*BUFF_LEN];
uint8_t data1A[6*BUFF_LEN];
uint8_t data1B[6*BUFF_LEN];
volatile int i0 = 0;
volatile int i1 = 0;
volatile int alt0 = false;
volatile int write0 = true;
volatile int alt1 = false;
volatile int write1 = false;

volatile int count0 = 0;
volatile int count1 = 0;

int high0 = 0;
int high1 = 0;
Sd2Card card;
File file;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(8, HIGH);
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    Serial.println("wait");
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initializing SD card...");
  pinMode(SS,OUTPUT); 
  if (!SD.begin(4)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  file = SD.open(filename, FILE_WRITE);
  if (file) {
    Serial.println("File opened");
  }
  else {
    Serial.println("File did not open");
  }

  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Configuring I2C");
  I2C.begin(100000);                                 // Start I2C bus at 400kHz
  Serial.println("Setting DMA Channels");
  I2C.setWriteChannel(2);                                  // Set the I2C1 DMAC write channel to 2
  I2C.setReadChannel(3);                                   // Set the I2C1 DMAC read channel to 3 
  Serial.println("DMAC Interrupts Enabled");  
//  I2C.dmacInterruptsOn();
  I2C.setPriority(0); 
  Serial.println("Reading WHO_AM_I");
  NVIC_ClearPendingIRQ(DMAC_IRQn); // Clear any pending DMAC interrupts
  NVIC_SetPriority(DMAC_IRQn, 0);  // Set the Nested Vector Interrupt Controller (NVIC) priority for the DMAC to 0 (highest)
  NVIC_EnableIRQ(DMAC_IRQn);  
  delay(500);
  I2C.readByte(MPU6050_ADDRESS0, WHO_AM_I);           // Read the WHO_AM_I register 
  while(I2C.isReadBusy()) {
    Serial.println("e");// Wait for synchronization  
  }
  Serial.println(I2C.getData(), HEX);             // Output the result
  Serial.println("Wake up the MPU6050 device");    
  I2C.writeByte(MPU6050_ADDRESS0, PWR_MGMT_1, 0x01);
  delay(100);
  Serial.println("Wait for the MPU6050 to settle");
  Serial.println("Set the gyro/accel filter to 20Hz");
  I2C.writeByte(MPU6050_ADDRESS0, CONFIG, 0x4);
  Serial.println("Set full scale range to +/-500 degrees/s");
  I2C.writeByte(MPU6050_ADDRESS0, GYRO_CONFIG, 0x1 << 3); 
  Serial.println("Set full scale range to +/-4g");  
  I2C.writeByte(MPU6050_ADDRESS0, ACCEL_CONFIG, 0x1 << 3);
  Serial.println("Set-up DMAC to write to MPU6050 register pointer");  
  I2C.initWriteRegAddr(MPU6050_ADDRESS0, GYRO_XOUT_H);
  Serial.println("Set DMAC to read the data");
//  I2C.initReadBytes(MPU6050_ADDRESS0, data0A + i0, 6);

//  I2C.attachReadCallback(mpu0Callback);
  I2C.attachSercomErrorCallback(sercomErrorCallback);
  I2C.attachDmacErrorCallback(dmacErrorCallback);
  Serial.println("Finished Configuring I2C");
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Configuring I2C1");
//  I2C1.begin(100000);                                       // Start I2C bus at 400kHz 
//  pinPeripheral(11, PIO_SERCOM);                         // Assign D3 to SERCOM2 I2C SDA
//  pinPeripheral(13, PIO_SERCOM);                         // Assign D4 to SERCOM2 I2C SCL
//  I2C1.setWriteChannel(10);                                  // Set the I2C1 DMAC write channel to 2
//  I2C1.setReadChannel(11);                                   // Set the I2C1 DMAC read channel to 3
//  I2C1.dmacInterruptsOn();
//  I2C1.setPriority(3);    
  I2C.readByte(MPU6050_ADDRESS1, WHO_AM_I);           // Read the WHO_AM_I register 
  while(I2C.readBusy);                               // Wait for synchronization  
  Serial.println(I2C.getData(), HEX);             // Output the result
  Serial.println("Wake up the MPU6050 device");  
  I2C.writeByte(MPU6050_ADDRESS1, PWR_MGMT_1, 0x01);        // Wake up the MPU6050 device
  delay(100);                                               // Wait for the MPU6050 to settle   
  I2C.writeByte(MPU6050_ADDRESS1, CONFIG, 0x4);             // Set the gyro/accel filter to 20Hz  
  I2C.writeByte(MPU6050_ADDRESS1, GYRO_CONFIG, 0x1 << 3);   // Set full scale range to +/-500 degrees/s  
  I2C.writeByte(MPU6050_ADDRESS1, ACCEL_CONFIG, 0x1 << 3);  // Set full scale range to +/-4g
  I2C.initWriteRegAddr(MPU6050_ADDRESS1, GYRO_XOUT_H);      // Set-up DMAC to write to MPU6050 register pointer
//  I2C.initReadBytes(MPU6050_ADDRESS1, data1A + i0, 6);// Set DMAC to read the data
//  I2C.attachReadCallback(mpu1Callback);                   // Attach a read callback function to I2C1
//  I2C.attachSercomErrorCallback(sercomErrorCallback);
//  I2C.attachDmacErrorCallback(dmacErrorCallback);
  Serial.println("Finished Configuring I2C1");
  digitalWrite(8, LOW);
  
////  


  I2C.attachReadCallback(mpu0Callback);
  I2C.readBytes(MPU6050_ADDRESS0, GYRO_XOUT_H, data0A + i0, 6);
}

void loop() {
  if (write0) {
    Serial.println("mpu0 Count" + String(count0));
    write0 = false;
    Serial.println("write0 now false!");
    if (high0) {
      digitalWrite(LED_BUILTIN, LOW);
      high0 = 0;
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
      high0 = 1;
    }
    if (!alt1) {
      file.write(data0A,sizeof(data0A));
    }
    else {
      file.write(data0B,sizeof(data0B));
    }
  }

  if (write1) {
    Serial.println("mpu1 " + String(count1));
    write1 = false;
    Serial.println("write1 now false!");
    if (high1) {
      digitalWrite(8, LOW);
      high1 = 0;
    } else {
      digitalWrite(8, HIGH);
      high1 = 1;
    }
    if (!alt1) {
      file.write(data1A,sizeof(data1A));
    }
    else {
      file.write(data1B,sizeof(data1B));
    }
  }
}

void mpu0Callback()
{
  int oldi0 = i0;

  Serial.println("mpu0 call " + String(i0) + " " + String(alt0));
    if (alt1) {
    I2C.attachReadCallback(mpu1Callback);
    I2C.readBytes(MPU6050_ADDRESS1, GYRO_XOUT_H, data1A + i1, 6);
  }
  else {
    I2C.attachReadCallback(mpu1Callback);
    I2C.readBytes(MPU6050_ADDRESS1, GYRO_XOUT_H, data1B + i1, 6);
  }
  count1++;
  i1 = i1 + 6;
  if (i1 >= 6 * BUFF_LEN) {
    i1 = 0;
    write1 = true;
    Serial.println("write1 true!");
    alt1 = !alt1;
  }
  Serial.println("mpu0 call " + String(oldi0) + " finished");
}
void mpu1Callback()
{  
  int oldi1 = i1;
  if (write1) {
    loop();
    return;
  }
  Serial.println("mpu1 call " + String(i1) + " " + String(alt1));

  if (alt0) {
    I2C.attachReadCallback(mpu0Callback);
    I2C.readBytes(MPU6050_ADDRESS0, GYRO_XOUT_H, data0A + i0, 6);
  }
  else {
    I2C.attachReadCallback(mpu0Callback);
    I2C.readBytes(MPU6050_ADDRESS0, GYRO_XOUT_H, data0B + i0, 6);
  }
  count0++;
  i0 = i0 + 6;
  if (i0 >= 6 * BUFF_LEN) {
    i0 = 0;
    write0 = true;
    Serial.println("write0 true!");
    alt0 = !alt0;
  }

  Serial.println("mpu1 call " + String(oldi1) + " finished");
}

void sercomErrorCallback() {
  Serial.println("Sercom Error!");
}
void dmacErrorCallback() {
  Serial.println("Dmac Error!");
}
