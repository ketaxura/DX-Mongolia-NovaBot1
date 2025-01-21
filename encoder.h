#include "esp32-hal-gpio.h"
#define LENCA 12
#define LENCB 13

#define RENCA 14
#define RENCB 27


void readRightenca() {
  if (digitalRead(RENCA) == 1)
    pulse1--;
  else
    pulse1++;
}

void readRightencb() {
  if (digitalRead(RENCB) == 1)
    pulse2++;
  else
    pulse2--;
}

void readLeftenca() {
  if (digitalRead(LENCA) == 1)
    pulse3--;
  else
    pulse3++;
}

void readLeftencb() {
  if (digitalRead(LENCB) == 1)
    pulse4--;
  else
    pulse4++;
}

void encoderSetup() {
  pinMode(RENCA, INPUT_PULLUP);
  pinMode(RENCB, INPUT_PULLUP);

  pinMode(LENCA, INPUT_PULLUP);
  pinMode(LENCB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(RENCA), readRightenca, RISING);
  attachInterrupt(digitalPinToInterrupt(RENCB), readRightencb, RISING);
  attachInterrupt(digitalPinToInterrupt(LENCA), readLeftenca, RISING);
  attachInterrupt(digitalPinToInterrupt(LENCB), readLeftencb, RISING);
}
