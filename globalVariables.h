#include "esp32-hal.h"
#include "Arduino.h"
#include "esp32-hal-gpio.h"
float initialYaw = 0;

struct euler_t {
  float yaw;
  float pitch;
  float roll;
} ypr;


int speedVal = 0;
int steerVal = 0;


//encoder pulse counter variables
volatile long pulse1 = 0;
volatile long pulse2 = 0;
volatile long pulse3 = 0;
volatile long pulse4 = 0;

int debugVal1 = 0;
int debugVal2 = 0;
int debugVal3 = 0;

float ultraDuration1, distance1;

float ultraDuration2, distance2;


#define Trig1 17
#define Echo1 16

#define Trig2 19
#define Echo2 18

void ultraSonicSetup() {
  pinMode(Trig1, OUTPUT);
  pinMode(Echo1, INPUT);


  pinMode(Trig2, OUTPUT);
  pinMode(Echo2, INPUT);
}


void ultraSonicData() {
  digitalWrite(Trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig1, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig1, LOW);
  ultraDuration1 = pulseIn(Echo1, HIGH);
  distance1 = (ultraDuration1 * 0.0343) / 2;


  digitalWrite(Trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig2, LOW);
  ultraDuration2 = pulseIn(Echo2, HIGH);
  distance2 = (ultraDuration2 * 0.0343) / 2;
}

unsigned long safetyTimer;
int safetyState = 0;

void ultraSonicSafety() {
  //stop, there is something infront of the robot
  if (distance1 < 50 || distance2 < 50) {

    if (safetyState == 0) {
      safetyState = 1;
      safetyTimer = millis();
    }
  }


  //wait a bit before continuing
  if (safetyState == 1 && labs(millis() - safetyTimer) > 2000) {
    //good to continue in manual mode
    if (distance1 > 50 && distance2 > 50) {
      safetyState = 0;
    }
  }
}



int turnFunction_host = 0;