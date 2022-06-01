#include "main.h"

char cmdbuf[CMDMAX];
unsigned char cmdbuf_n = 0;

void setup()
{
  Serial.begin(115200);
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
          String strbuf = String(cmdbuf).substring(4);
          int16_t input_vel = strbuf.toInt();
          Serial.println(input_vel, HEX);

          byte data[9] = {0x01, 0x64, input_vel, 0x00, 0x00, 0x00, 0x00, 0x00};

          // CRC計算
          uint8_t result8 = crcx::crc8(data, 9, ht::util::crc::Crc8::MAXIM);

          //送信データ
          byte send_data[10] = {
              data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], result8};

          for (int i = 0; i <= 9; i++)
          {
            Serial.print(send_data[i], HEX);
            Serial.print(",");
          }
          Serial.println("");
        }
        else if (strncmp(cmdbuf, "crc", 3) == 0)
        {
          const uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
          uint8_t result8 = crcx::crc8(data, 9, ht::util::crc::Crc8::MAXIM);
          Serial.print("crc8  = 0x");
          Serial.println(result8, HEX);
        }
        else if (strncmp(cmdbuf, "help", 4) == 0)
        {
          help();
        }
        else if (strncmp(cmdbuf, "ver", 3) == 0)
        {
          ver();
        }
        else if (strncmp(cmdbuf, "reset", 5) == 0)
        {
          ESP.restart();
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