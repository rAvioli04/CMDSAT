#include <Wire.h>
#define BUFF_LEN 450

#include <wiring_private.h>
#define MPU6050_ADDRESS0    0x68        
#define MPU6050_ADDRESS1    0x69                             
#define PWR_MGMT_1          0x6B
#define SAMPLE_RATE_DIVIDER 0x19
#define CONFIG              0x1A
#define GYRO_CONFIG         0x1B
#define ACCEL_CONFIG        0x1C
#define ACCEL_XOUT_H        0x3B
#define GYRO_XOUT_H         0x43
#define WHO_AM_I            0x75

uint8_t data0[16];
uint8_t data1[16];
uint8_t command[2];
uint32_t ti;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(8, HIGH);
  Serial.begin(115200);
  // put your setup code here, to run once:
//  while (!Serial) {
//    Serial.println("wait");
//    ; // wait for serial port to connect. Needed for native USB port only
//  }
  Wire.begin();  
  Wire.setClock(400000);

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

}

void loop() {
  // put your main code here, to run repeatedly:
  readI2C(MPU6050_ADDRESS0,ACCEL_XOUT_H,6,data0);
  readI2C(MPU6050_ADDRESS0,GYRO_XOUT_H,6,data0+6);
  ti = micros();
  data0[12] = ti >> 8;
  data0[13] = ti >> 0;
  data0[14]='0';
  data0[15] = '\n';
  readI2C(MPU6050_ADDRESS1,ACCEL_XOUT_H,6,data1);
  readI2C(MPU6050_ADDRESS1,GYRO_XOUT_H,6,data1+6);
  ti = micros();
  data1[12] = ti >> 8;
  data1[13] = ti >> 0;
  data1[14]='1';
  data1[15] = '\n';
  Serial.write(data0,16);
  Serial.write(data1,16);
}
