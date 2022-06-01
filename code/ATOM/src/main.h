#include "Arduino.h"
#include <CRCx.h>

#define clr 0
#define set 1
#define CMDMAX 100

#define RX_PIN 32
#define TX_PIN 26

void help(void);
void ver(void);
void cmd_chk(void);
void buf_clr(void);
void receiveData(void);
void display_data(void);
void sendData(void);
// void onReceive(int packetSize);