#include <Wire.h>
#include <Arduino.h>
#include <CRCx.h>
#define RX_PIN 32
#define TX_PIN 26

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("test");

  pinMode(39, INPUT);

  // const size_t size = sizeof(data);
  // uint8_t result8 = crcx::crc8(data, size);

  // byte set_id[9] = {0xAA, 0x55, 0x53, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
}

void loop()
{
  if (digitalRead(39) == LOW)
  {
    /*
    byte id_check[10] = {0xC8, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDE};
    Serial2.write(id_check, 10);
    Serial.println("id_check");
    delay(100);
    */

    byte spin[10] = {0x01, 0x64, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC4};
    Serial2.write(spin, 10);
    Serial.println("spin");
    delay(1);
  }

  if (Serial2.available())
  {
    uint8_t data = Serial2.read();
    // Serial.println(data, HEX);
  }
  // delay(100);
}