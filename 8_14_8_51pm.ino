// 2 wheel bot programming


//global variables
#include "globalVariables.h"


//Motor control functions
#include "motorControl.h"

//encoder functions
#include "encoder.h"

//Travel straight code
#include "travelStraight.h"

//IMU data functions
#include "imuData.h"

//Yaw code
#include "yawTurn.h"

//PS4 controller functions
#include "controllerCode.h"





// //Right motor pins
// #define RightRPWM 32
// #define RightLPWM 33

// //Left motor pins
// #define LeftRPWM 25
// #define LeftLPWM 26


void setup() {
  Serial.begin(115200);
  motorSetup();
  // encoderSetup();
  controllerSetup();
  //imuSetup();
  //ultraSonicSetup();
}

unsigned long ultraSonicTimer;
unsigned long ultraSonicPrevTimer;


void loop() {
  // ultraSonicTimer = millis();
  // if (ultraSonicTimer - ultraSonicPrevTimer >= 100) {
  //   ultraSonicPrevTimer = ultraSonicTimer;
  //   ultraSonicData();
  // }

  // Serial.print(distance1);
  // Serial.print("\t");
  // Serial.print(distance2);
  // Serial.print("\t");
  // Serial.print(debugVal2);
  // Serial.println("");

  // motorDrive(1, 1, 35);
  // motorDrive(-1, -1, 35);

  // Serial.print(abs(speedVal) - steerVal);
  // Serial.print("\t");
  // Serial.print(abs(speedVal) + steerVal);
  // Serial.println("");

  // imuLoop();

  controllerManual();

  // errorMotorHandler();
}
