#include "main.h"

char cmdbuf[CMDMAX];
unsigned char cmdbuf_n = 0;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
}

void loop()
{
  cmd_chk();
}

void cmd_chk(void)
{
  char c;

  if (Serial.available())
  {
    c = Serial.read();
    if (cmdbuf_n >= CMDMAX - 1)
    {
      Serial.print("\r\n");
      Serial.print("command error.\r\n");
      buf_clr();
    }
    else
    {
      if (c == '\r')
      {
        Serial.print("\r\n");
        if (strncmp(cmdbuf, "vel:", 4) == 0)
        {
          //モード切換え
          byte vel_mode[10] = {
              0x01, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};
          Serial2.write(vel_mode, 10);

          String strbuf = String(cmdbuf).substring(4);
          int16_t input_vel = strbuf.toInt();

          uint8_t dataLH = (uint8_t)((input_vel & 0x0000FF00) >> 8);
          uint8_t dataLL = (uint8_t)((input_vel & 0x000000FF) >> 0);

          byte data[9] = {0x01, 0x64, dataLH, dataLL, 0x00, 0x00, 0x00, 0x00, 0x00};

          // CRC計算
          uint8_t result8 = crcx::crc8(data, 9, ht::util::crc::Crc8::MAXIM);

          //送信データ
          byte send_data[10] = {
              data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], result8};

          Serial2.write(send_data, 10);
        }
        else if (strncmp(cmdbuf, "angle:", 6) == 0)
        {
          //モード切替
          byte angle_mode[10] = {
              0x01, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};
          Serial2.write(angle_mode, 10);

          String strbuf = String(cmdbuf).substring(6);
          uint16_t input_angle = strbuf.toInt();
          uint16_t calc_angle = 32767 / 360 * input_angle;

          uint8_t dataLH = (uint8_t)((calc_angle & 0x0000FF00) >> 8);
          uint8_t dataLL = (uint8_t)((calc_angle & 0x000000FF) >> 0);

          byte data[9] = {0x01, 0x64, dataLH, dataLL, 0x00, 0x00, 0x00, 0x00, 0x00};

          // CRC計算
          uint8_t result8 = crcx::crc8(data, 9, ht::util::crc::Crc8::MAXIM);

          //送信データ
          byte send_data[10] = {
              data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], result8};

          Serial2.write(send_data, 10);
          /*
          for (int i = 0; i <= 9; i++)
          {
            Serial.print(send_data[i], HEX);
            Serial.print(",");
          }
          Serial.println("");
          */
        }
        else if (strncmp(cmdbuf, "stop", 4) == 0)
        {
          byte data[9] = {0x01, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

          // CRC計算
          uint8_t result8 = crcx::crc8(data, 9, ht::util::crc::Crc8::MAXIM);

          //送信データ
          byte send_data[10] = {
              data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], result8};

          Serial2.write(send_data, 10);
        }

        else
        {
          if (cmdbuf_n != 0)
          {
            Serial.printf("command syntax error.\r\n");
          }
        }
        buf_clr();
      }
      else if (c == '\b')
      {
        if (cmdbuf_n >= 1)
        {
          cmdbuf[cmdbuf_n] = 0;
          cmdbuf_n--;
          cmdbuf[cmdbuf_n] = 0;
          Serial.print('\b');
          Serial.print(' ');
          Serial.print('\b');
        }
      }
      else if (c == '\n')
      {
        ;
      }
      else
      {
        cmdbuf[cmdbuf_n] = c;
        cmdbuf_n++;
        Serial.print(c);
      }
    }
  }
  return;
}

void buf_clr(void)
{
  cmdbuf_n = 0;
  memset(cmdbuf, clr, sizeof(cmdbuf));
  return;
}

void ver(void)
{
  Serial.printf("Build Info :%s\r\n", __DATE__);
  return;
}

void help(void)
{
  Serial.print("+---------------------------------------------------------------------------------+\r\n");
  Serial.print("|  コマンド一覧                                                                   |\r\n");
  Serial.print("+---------------------------------------------------------------------------------+\r\n");
  Serial.print("|  help                : コマンド一覧を表示します。                               |\r\n");
  Serial.print("|  ver                 : 書き込み日付を表示します。                               |\r\n");
  Serial.print("|  reset               : 再起動を行います。                                       |\r\n");
  Serial.print("+---------------------------------------------------------------------------------+\r\n");
  return;
}