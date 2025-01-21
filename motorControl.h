#include "esp32-hal.h"
#include "esp32-hal-gpio.h"
//We are  using the IBT2 motor driver
//We need to send data through two pins RPWM and LPWM for each motors.

//Motor control logic is as follows
//if the RPWM is some PWM value and then LPWM is HIGH, the motor will turn CW
//if the LPWM is some PWM value and then RPWM is HIGH, the motor will turn CCW
//if they are both low the motor will brake
// !! THEY CAN NEVER BE BOTH HIGH !! THEY SHOULD NEVER BE BOTH 255 AND 255 //

//https://www.electronicwings.com/esp32/pwm-of-esp32

//Right motor pins
#define RightRPWM 26
#define RightLPWM 26



//Left motor pins
#define LeftRPWM 32
#define LeftLPWM 33




void motorSetup() {
  pinMode(RightRPWM, OUTPUT);
  pinMode(RightLPWM, OUTPUT);

  pinMode(LeftLPWM, OUTPUT);
  pinMode(LeftRPWM, OUTPUT);
}

void motorDrive(int motor, int dir, int pwm) {
  //right motor
  if (motor == 1) {

    //forward right motor
    if (dir == 1) {
      // Serial.println("CW");
      analogWrite(RightRPWM, 0);
      analogWrite(RightLPWM, pwm);
    }

    //reverse right motor
    if (dir == -1) {
      // Serial.println("CCW");
      analogWrite(RightRPWM, pwm);
      analogWrite(RightLPWM, 0);
    }

    //brake the right motor
    if (dir == 0) {
      //Serial.println("Brake");
      analogWrite(RightRPWM, 0);
      analogWrite(RightLPWM, 0);
    }
  }


  //left motor
  if (motor == -1) {

    //forward left motor
    if (dir == 1) {
      analogWrite(LeftRPWM, 0);
      analogWrite(LeftLPWM, pwm);
    }

    //reverse left motor
    if (dir == -1) {
      analogWrite(LeftRPWM, pwm);
      analogWrite(LeftLPWM, 0);
    }

    if (dir == 0) {
      analogWrite(LeftRPWM, 0);
      analogWrite(LeftLPWM, 0);
    }
  }
}

//functions to control the motor logic failure
void errorMotorHandler() {
  if (RightRPWM == 255 && RightLPWM == 255) {
    while (1) {
      Serial.println("Motor control logic failure!, both states are high");
    }
  }

  if (LeftRPWM == 255 && LeftLPWM == 255) {
    while (1) {
      Serial.println("Motor control logic failure!, both states are high");
    }
  }
}
