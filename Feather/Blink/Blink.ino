#include <SPI.h>
#include <SD.h>
#include "wiring_private.h"
#ifdef SOFTWARE_SPI
#undef SOFTWARE_SPI
#endif
#define USE_SPI_LIB
#define SDCARD_SPI sercom4SPI

SPIClass sercom4SPI(&sercom4,MISO,SCK,MOSI,SPI_PAD_2_SCK_3,SERCOM_RX_PAD_0);

void setup() {
  pinPeripheral(SCK,PIO_SERCOM_ALT );
  pinPeripheral(MISO,PIO_SERCOM_ALT );
  pinPeripheral(MOSI,PIO_SERCOM_ALT );
  pinPeripheral(SS,PIO_OUTPUT);
  sercom4SPI.begin();
  sercom4SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPISettings settings = SPISettings(125000, MSBFIRST, SPI_MODE0);
  sercom4SPI.beginTransaction(settings);
  
  while (!SD.begin(SS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    delay(100);
  }
  Serial.println("Card Found, or not present");
  // put your setup code here, to run once:
//  pinMode(24,OUTPUT);
//  pinMode(16,OUTPUT);
//  digitalWrite(24,LOW);
//  digitalWrite(16,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:


//
//  digitalWrite(16,LOW);
//  delay(1000);
//  for(int i = 0; i<10; i++){
//    digitalWrite(24,HIGH);
//    delay(50);
//    digitalWrite(24,LOW);
//    delay(50);
//  }
//  delay(2000);
//  digitalWrite(24,HIGH);
//  for(int i = 0; i<10; i++){
//    digitalWrite(16,HIGH);
//    delay(50);
//    digitalWrite(16,LOW);
//    delay(50);
//  }
//  digitalWrite(24,LOW);
//  delay(4000);
}
