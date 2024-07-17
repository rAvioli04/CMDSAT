#include <SPI.h>
#include <Wire.h>
#include <SD.h>

#define BUFF_LEN 500

#include <wiring_private.h>
#define MPU6050_ADDRESS0    0x68        
#define MPU6050_ADDRESS1    0x69                             
#define PWR_MGMT_1          0x6B
#define SAMPLE_RATE_DIVIDER 0x19
#define CONFIG              0x1A
#define GYRO_CONFIG         0x1B
#define ACCEL_CONFIG        0x1C
#define ACCEL_XOUT_H         0x3B
#define WHO_AM_I            0x75

String configFile = "config.txt";
String filename = "default.txt";
#define LINE_LEN (12)
uint8_t data0A[LINE_LEN*BUFF_LEN];
uint8_t data0B[LINE_LEN*BUFF_LEN];
uint8_t data1A[LINE_LEN*BUFF_LEN];
uint8_t data1B[LINE_LEN*BUFF_LEN];
uint8_t command[2];
volatile int i0 = 0;
volatile int i1 = 0;
volatile int alt0 = false;
volatile int write0 = true;
volatile int alt1 = false;
volatile int write1 = false;
int writeCount = 0;

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

  file = SD.open(configFile, FILE_READ);
  if (file) {
    Serial.println("File opened");
  }
  else {
    Serial.println("File did not open");
  }

  String fileNumberString = "";
  while (file.available()) {
      fileNumberString += file.read();
  }
  int fileNumber = (fileNumberString.toInt()) + 1;
  Serial.println(String(fileNumber));
  file.close();
  SD.remove(configFile);
  file = SD.open(configFile, FILE_WRITE);
  if (file) {
    Serial.println("File opened");
  }
  else {
    Serial.println("File did not open");
  }
  uint8_t bytes[2];
  bytes[0] = (fileNumber >> 8);
  bytes[1] = (fileNumber >> 0);
  file.write(bytes,2);
  file.close();
  filename = "data" + String(fileNumber) + ".txt";
  Serial.println(filename);
  file = SD.open(filename, FILE_WRITE);
  if (file) {
    Serial.println("File opened");
  }
  else {
    Serial.println("File did not open");
  }

  Wire.begin();  
  Wire.setClock(100000);

  writeI2C(MPU6050_ADDRESS0,PWR_MGMT_1,0x07);
  delay(100);
  writeI2C(MPU6050_ADDRESS0,PWR_MGMT_1,0x80);
  delay(100);
  writeI2C(MPU6050_ADDRESS0,PWR_MGMT_1,0x01);
  delay(100);
  writeI2C(MPU6050_ADDRESS0,SAMPLE_RATE_DIVIDER,0x00);
  writeI2C(MPU6050_ADDRESS0,CONFIG,0x00);
  writeI2C(MPU6050_ADDRESS0,GYRO_CONFIG,0x1 << 3);
  writeI2C(MPU6050_ADDRESS0,ACCEL_CONFIG,0x1 << 3);
  digitalWrite(LED_BUILTIN, LOW);

  
  writeI2C(MPU6050_ADDRESS1,PWR_MGMT_1,0x07);
  delay(100);
  writeI2C(MPU6050_ADDRESS1,PWR_MGMT_1,0x80);
  delay(100);
  writeI2C(MPU6050_ADDRESS1,PWR_MGMT_1,0x01);
  delay(100);
  writeI2C(MPU6050_ADDRESS1,SAMPLE_RATE_DIVIDER,0x00);
  writeI2C(MPU6050_ADDRESS1,CONFIG,0x00);
  writeI2C(MPU6050_ADDRESS1,GYRO_CONFIG,0x1 << 3);
  writeI2C(MPU6050_ADDRESS1,ACCEL_CONFIG,0x1 << 3);
  digitalWrite(8, LOW);
}

void writeI2C(uint8_t address, uint8_t regist, uint8_t cmd) {
  Wire.beginTransmission(address);
  command[0] = regist;
  command[1] = cmd;
  Wire.write(command,2);
  Wire.endTransmission();
}

void readI2C(uint8_t address, uint8_t regist, uint8_t bytes, uint8_t buf[]) {
  Wire.beginTransmission(address);
  command[0] = regist;
  Wire.write(command,1);
  Wire.endTransmission(false);
  Wire.requestFrom(address, bytes, true);
  for (int i = 0; i < bytes; i++) {
    buf[i] = Wire.read();
  }
  uint32_t ti = micros();
  buf[6] = ti >> 24;
  buf[7] = ti >> 16;
  buf[8] = ti >> 8;
  buf[9] = ti >> 0;
  buf[11] = '\n';
}

void loop() {
  if (!alt0) {
    readI2C(MPU6050_ADDRESS0,ACCEL_XOUT_H,6,data0A + i0);
//    uint32_t ti = micros();
//    (data0A + i0 + 6)[0] = ti >> 24;
//    (data0A + i0 + 6)[0] = ti >> 24;
//    (data0A + i0 + 6)[0] = ti >> 24;
//    (data0A + i0 + 6)[0] = ti >> 24;
    (data0A + i0)[10]='0';
  }
  else {
    readI2C(MPU6050_ADDRESS0,ACCEL_XOUT_H,6,data0B + i0);
    (data0B + i0)[10]='0';
  }

  if (!alt1) {
    readI2C(MPU6050_ADDRESS0,ACCEL_XOUT_H,6,data1A + i1);
    (data1A + i1)[10]='1';
  }
  else {
    readI2C(MPU6050_ADDRESS0,ACCEL_XOUT_H,6,data1B + i1);
    (data1B + i1)[10]='1';
  }

  i0 = i0 + LINE_LEN;
  if (i0 >= LINE_LEN * BUFF_LEN) {
    i0 = 0;
    write0 = true;
    Serial.println("write0 true!");
    alt0 = !alt0;
  }

  i1 = i1 + LINE_LEN;
  if (i1 >= LINE_LEN * BUFF_LEN) {
    i1 = 0;
    write1 = true;
    Serial.println("write1 true!");
    alt1 = !alt1;
  }

  if (write0) {
    write0 = false;
    Serial.println("write0 now false!");
    if (high0) {
      digitalWrite(LED_BUILTIN, LOW);
      high0 = 0;
    } else {
      digitalWrite(LED_BUILTIN, HIGH);
      high0 = 1;
    }

    if (!alt0) {
      file.write(data0A,sizeof(data0A));
    }
    else {
      file.write(data0B,sizeof(data0B));
    }
    
    writeCount++;
  }

  if (write1) {
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
    writeCount++;
  }

  if (writeCount >= 20) {
    file.close();
    file = SD.open(filename,FILE_WRITE);
    Serial.println("File closed");
    writeCount = 0;
  }
}
