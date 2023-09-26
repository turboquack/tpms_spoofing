
#include <Arduino.h>
#include "ELECHOUSE_CC1101_SRC_DRV.h"

int gdo0;

byte buffers[4][15] = {
//                          UU    ID   ID    ID    ID   FR    PRES  TEMP   BB    CC  
  { 0x00,0x00,0x00, 0x01, 0xc2, 0x0e, 0xe6, 0xd1, 0x85, 0xa0, 0xb7, 0x32, 0x0a, 0xaf, 0x4 },
  { 0x00,0x00,0x00, 0x01, 0xc2, 0x0e, 0xe7, 0x2c, 0x82, 0xa0, 0xb7, 0x32, 0x0a, 0xaf, 0x4 },
  { 0x00,0x00,0x00, 0x01, 0xc2, 0x0e, 0xe6, 0xd1, 0x96, 0xa0, 0xb7, 0x32, 0x0a, 0xaf, 0x4 },
  { 0x00,0x00,0x00, 0x01, 0xc2, 0x0e, 0xe6, 0xd1, 0x7f, 0xa0, 0xb7, 0x32, 0x0a, 0xaf, 0x4 }
};

byte sync[8] = {0xa,0xa,0xa,0xa,0xa,0xa,0xa,0xa} ;

void setup() {

#define ONBOARD_LED 16

#ifdef ESP32
  gdo0 = 2;  // for esp32! GDO0 on GPIO pin 2.

#elif ESP8266
  gdo0 = 5;  // for esp8266! GDO0 on pin 5 = D1.
#else
  gdo0 = 6;  // for Arduino! GDO0 on pin 6.
#endif

  Serial.begin(9600);

  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, LOW);

  if (ELECHOUSE_cc1101.getCC1101()) {  // Check the CC1101 Spi connection.
    Serial.println("Connection OK");
  } else {
    Serial.println("Connection Error");
  }

  ELECHOUSE_cc1101.Init();  // must be set to initialize the cc1101!
  ELECHOUSE_cc1101.setSyncMode(0);
  ELECHOUSE_cc1101.setSyncWord(0xaa, 0xa9);
  ELECHOUSE_cc1101.setManchester(1);     // Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.
  Serial.println("Tx Mode");
  
}

byte calculateChecksum(byte buffer[], int bufferSize) {
  byte checksum = 0;
  
  // Calculate checksum by XORing bytes 1 to 9
  for (int i = 5; i <= 12; i++) {
    checksum ^= buffer[i];
  }
  
  return checksum;
}


void loop() {


  ELECHOUSE_cc1101.SetTx();
  digitalWrite(ONBOARD_LED, HIGH);

 // Iterate through the rows of the array
  for (int wheel = 0; wheel < 4; wheel++) {
    for (int i=0; i<12; i++)
    {
      ELECHOUSE_cc1101.SendData(sync, 8, 100);
      delay(2);
    }
    delay(50);
    Serial.print("Sync number = ");
    Serial.println(wheel);
    for (int message=1; message<4; message++)
    {
      buffers[wheel][9]=message;
      buffers[wheel][13]=calculateChecksum(buffers[wheel], sizeof(buffers[wheel]));
      ELECHOUSE_cc1101.SendData(buffers[wheel], 15, 100);
      for (int j = 0; j < 13; j++) {
        Serial.print(buffers[wheel][j],HEX);
        }
        Serial.println();
      delay(50);
    }
  }
  delay(8000);  
}
