
#include <Arduino.h>
#include "ELECHOUSE_CC1101_SRC_DRV.h"

int gdo0;

byte buffers[4][15] = {
//                          UU    ID   ID    ID    ID   FR    PRES  TEMP   BB    CC  
  { 0x00,0x00,0x00, 0x01, 0xe0, 0x3e, 0xcd, 0x27, 0xe4, 0xe2, 0xb6, 0x45, 0x01, 0x6d, 0x00 },
  { 0x00,0x00,0x00, 0x01, 0xe0, 0x3e, 0xcd, 0x27, 0xf2, 0xe2, 0xba, 0x57, 0x01, 0x6d, 0x00 },
  { 0x00,0x00,0x00, 0x01, 0xe0, 0x3e, 0xcd, 0x27, 0xf7, 0xe2, 0xb2, 0x54, 0x01, 0x6d, 0x00 },
  { 0x00,0x00,0x00, 0x01, 0xe0, 0x3e, 0xcd, 0x27, 0xe2, 0xe2, 0xba, 0x51, 0x01, 0x6d, 0x00 }
};


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
  ELECHOUSE_cc1101.setSyncWord(0x55, 0x56);
  ELECHOUSE_cc1101.setManchester(1);     // Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable.
  Serial.println("Tx Mode");
  
}

byte calculate_crc8(byte data[], size_t length) {
  byte crc = 0xAA;  // Initial value

  for (size_t i = 4; i < length-2; i++) {
    crc ^= data[i];
    for (byte bit = 0; bit < 8; bit++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x07;
      } else {
        crc <<= 1;
      }
    }
  }

  return crc;
}

void loop() {

  
  ELECHOUSE_cc1101.SetTx();

// Iterate through the rows of the array
for (int wheel = 0; wheel < 4; wheel++) {
  for (int message=225; message<228; message++)
    {
      buffers[wheel][9]=message;
      buffers[wheel][13] = calculate_crc8(buffers[wheel], 15);
      ELECHOUSE_cc1101.SendData(buffers[wheel], 16, 100);
  for (int j = 0; j < 15; j++) {
        Serial.print(buffers[wheel][j],HEX);
        Serial.print(" ");
        }
        Serial.println();
    }
    delay(1000);
}
  delay(20000); 

}
