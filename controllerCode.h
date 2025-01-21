#include <PS4Controller.h>
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_err.h"


//if the state is 1 we are in manual, if we are in 2 we are in automatic, 0 means that ultrasonic has stopped the robot
int controllerState = 1;

unsigned long lastTimeStamp = 0;


int encoderMotor_distance = 0;
int encoderMotor_state = 0;


void encoderMotor(int distance);

//Function predeclaration
void manualLogic();
void onConnect();
void onDisConnect();
void controllerSetup();
void controllerManual();
void controllerAuto();
void manualLogic();


//header variables


unsigned long manualTimer;
int manualState = 0;
// int speedVal = 0;
// int steerVal = 0;
int initialYawLock = 0;


void notify() {
  char messageString[200];
  sprintf(messageString, "%4d,%4d,%4d,%4d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d",
          PS4.LStickX(),
          PS4.LStickY(),
          PS4.RStickX(),
          PS4.RStickY(),
          PS4.Left(),
          PS4.Down(),
          PS4.Right(),
          PS4.Up(),
          PS4.Square(),
          PS4.Cross(),
          PS4.Circle(),
          PS4.Triangle(),
          PS4.L1(),
          PS4.R1(),
          PS4.L2(),
          PS4.R2(),
          PS4.Share(),
          PS4.Options(),
          PS4.PSButton(),
          PS4.Touchpad(),
          PS4.Charging(),
          PS4.Audio(),
          PS4.Mic(),
          PS4.Battery());

  //Only needed to print the message properly on serial monitor. Else we dont need it.
  if (millis() - lastTimeStamp > 50) {
    Serial.println(messageString); 
    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("Connected!.");
}

void onDisConnect() {
  Serial.println("Disconnected!.");
}


void controllerSetup() {

  //PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  uint8_t pairedDeviceBtAddr[20][6];
  int count = esp_bt_gap_get_bond_device_num();
  esp_bt_gap_get_bond_device_list(&count, pairedDeviceBtAddr);
  for (int i = 0; i < count; i++) {
    esp_bt_gap_remove_bond_device(pairedDeviceBtAddr[i]);
  }
  Serial.println("Ready.");
}



void controllerManual() {
  
  //record initial yaw code
  if (PS4.Square() == 1) {
    if (initialYawLock == 0) {
      initialYaw = ypr.yaw;
      initialYawLock = 1;
    }
  }

  // Serial.print("\t");
  // Serial.print(steerVal);
  // Serial.println("");

  //switch between auto and manual
  if (PS4.Left() == 1) {
    if (controllerState == 2 && manualState == 0) {
      controllerState = 1;
      manualState = 1;
      manualTimer = millis();
    }

    if (controllerState == 1 && manualState == 0) {
      controllerState = 2;
      manualState = 1;
      manualTimer = millis();
    }
  }
  //debounce code
  if (manualState == 1 && labs(millis() - manualTimer) > 1000) {
    manualState = 0;
  }

  //manual
  if (controllerState == 1) {
    manualLogic();
  }
  //automatic
  if (controllerState == 2) {
    controllerAuto();
  }
}


int autoState = 0;

int safetyOveride = 0;

void controllerAuto() {

  ultraSonicSafety();

  if (PS4.Up() == 1) {
    if (autoState == 0) {
      autoState = 1;
    }
  }

  else if (PS4.Circle() == 1) {
    safetyOveride = 1;
  }

  //stop the robot, because there is something  infront of it
  if (safetyState == 1) {

    motorDrive(1, 0, 0);
    motorDrive(-1, 0, 0);
  }

  else if (safetyState == 0) {

    //begin the auto movement
    if (autoState == 1) {

      if (turnFunction_host == 0) {
        encoderMotor(2);
      }


      if (turnFunction_host == 1) {
        if (turnFunction_state == 2) {
          turnFunction_state = 0;
        }
        turnFunction(-90);
      }

      else if (turnFunction_host == 2) {
        if (turnFunction_state == 2) {
          turnFunction_state = 0;
        }
        turnFunction(90);
      } else if (turnFunction_host == 3) {
        if (turnFunction_state == 2) {
          turnFunction_state = 0;
        }
        turnFunction(90);
      } else if (turnFunction_host == 4) {
        if (turnFunction_state == 2) {
          turnFunction_state = 0;
        }
        turnFunction(90);
      }

      else if (turnFunction_host == 5) {
        pulse2 = 0;
        debugVal1++;
        encoderMotor_state = 0;
        turnFunction_host = 0;
      }
    }
  }
}

int manualLogicState = 1;

// 98162828 bathuur redactor mongol tv
void manualLogic() {

  //ultraSonicSafety();

  // //stop the robot, because there is something  infront of it
  // if (safetyState == 1) {

  //   motorDrive(1, 0, 0);
  //   motorDrive(-1, 0, 0);
  // }



  // //good to go
  // if (safetyState == 0 && safetyState == 1) {}
  debugVal2++;

  speedVal = PS4.RStickY();


  //if there is x value on the right joystick then we simply add on to the forward/reverse speed on a specific motor
  //steer val, if positive we add to left motor and subtract from right motor
  if (PS4.RStickX() != 0) {
    steerVal = PS4.RStickX() / 2;
  }

  else if (PS4.RStickX() == 0) {
    steerVal = 0;
  }

  if (PS4.RStickX() == 0 && PS4.RStickY() == 0) {
    motorDrive(1, 0, 0);
    motorDrive(-1, 0, 0);
  }


  //go forward
  if (speedVal > 0) {
    motorDrive(1, 1, abs(speedVal) + steerVal);
    motorDrive(-1, -1, abs(speedVal) - steerVal);
  }

  //go back
  if (speedVal < 0) {
    motorDrive(-1, 1, abs(speedVal) - steerVal);
    motorDrive(1, -1, abs(speedVal) + steerVal);
  }
}





unsigned long encoderMotor_timer;
void encoderMotor(int distance) {

  //Serial.println(encoderMotor_distance);

  //1 full rotation is 0.628m and has 169 encoder pulses
  encoderMotor_distance = (distance / 0.628) * 169;

  // Serial.println(pulse2);

  if (encoderMotor_state == 0 && (abs(pulse2) < encoderMotor_distance / 10)) {

    motorDrive(1, 1, 70);
    motorDrive(-1, -1, 70);
  }

  else if (encoderMotor_state == 0 && (abs(pulse2) > encoderMotor_distance / 10)) {
    encoderMotor_state = 1;
  }

  else if (encoderMotor_state == 1 && abs(pulse2) < encoderMotor_distance) {
    travelStraight();

  }

  else if (encoderMotor_state == 1 && abs(pulse2) > encoderMotor_distance) {
    encoderMotor_state = 2;
    encoderMotor_timer = millis();
  }

  else if (encoderMotor_state == 2 && labs(millis() - encoderMotor_timer) < 2000) {
    motorDrive(1, 0, 0);
    motorDrive(-1, 0, 0);
  }

  else if (encoderMotor_state == 2 && labs(millis() - encoderMotor_timer) > 2000) {
    encoderMotor_state = 3;
    turnFunction_host++;
  }
}
