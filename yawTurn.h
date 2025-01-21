int startYawNormalLock = 0;
int CCW90Lock = 0;
int CW90Lock = 0;

float startYaw_setpoint = 0;
float startYaw_input;
float startYaw_error;
float startYaw_output;
float integral;
float derivative;
float lastError;
float startYaw_Kp = 5;
float startYaw_Ki = 0;
float startYaw_Kd = 0.1;

unsigned long sampleTime = 10;
unsigned long lastTime;


//startYaw_setpoint is the setpoint value for the PID, it is actually setpoint plus what the yaw value is currently, corrected for the logic of the gyroscope values


void startYaw(int degYaw) {
  // Serial.print(startYaw_output);
  // Serial.print("\t");
  // Serial.print(startYaw_input);
  // Serial.print("\t");
  // Serial.print(startYaw_error);
  // Serial.println("");


  // Return to the initialYaw
  if (degYaw == 0) {
    if (startYawNormalLock == 0) {
      startYaw_setpoint = initialYaw;
      startYawNormalLock = 1;
    }
  }

  // Rotate counter clockwise 90 degrees from the initialYaw
  if (degYaw == 90) {
    if (CCW90Lock == 0) {
      startYaw_setpoint = initialYaw + 90;
      if (startYaw_setpoint > 180) {
        startYaw_setpoint = startYaw_setpoint - 360;
      }
      CCW90Lock = 1;
    }
  }

  // Rotate clockwise 90 degrees from the initialYaw
  if (degYaw == -90) {
    if (CW90Lock == 0) {
      startYaw_setpoint = initialYaw - 90;
      if (startYaw_setpoint < -180) {
        startYaw_setpoint = startYaw_setpoint + 360;
      }
      CW90Lock = 1;
    }
  }

  unsigned long startYaw_currentTime = millis();
  unsigned long startYaw_elapsedTime = startYaw_currentTime - lastTime;

  if (startYaw_elapsedTime >= sampleTime) {
    lastTime = startYaw_currentTime;

    // Read sensor input
    startYaw_input = ypr.yaw;

    // Calculate error
    startYaw_error = startYaw_setpoint - startYaw_input;

    if (startYaw_error > 180) {
      startYaw_error -= 360;  // Adjust error for positive deviation
    } else if (startYaw_error < -180) {
      startYaw_error += 360;  // Adjust error for negative deviation
    }

    // Calculate integral and derivative terms
    integral += startYaw_error * startYaw_elapsedTime;
    derivative = (startYaw_error - lastError) / startYaw_elapsedTime;

    // Calculate PID output
    startYaw_output = startYaw_Kp * startYaw_error + startYaw_Ki * integral + startYaw_Kd * derivative;

    // Update last error
    lastError = startYaw_error;
  }



  if (abs(startYaw_error) >= 1) {

    if (startYaw_output > 50) {
      startYaw_output = 50;
    }

    if (startYaw_output < -50) {
      startYaw_output = -50;
    }


    // Output is +, so we must CCW
    if (startYaw_output > 0) {
      motorDrive(1, -1, abs(startYaw_output) + 60);
      motorDrive(-1, -1, abs(startYaw_output) + 60);
    }

    // Output is -, so we must CW
    if (startYaw_output < 0) {
      motorDrive(1, 1, abs(startYaw_output) + 60);
      motorDrive(-1, 1, abs(startYaw_output) + 60);
    }
  }

  else if (abs(startYaw_error) < 1) {
    motorDrive(-1, 0, 0);
    motorDrive(1, 0, 0);
  }
}

unsigned long turnFunction_timer = 0;
int turnFunction_state = 0;
//triggers the startYaw function but also has a timer inside that terminates the function when it "finishes" turning
void turnFunction(int deg) {

  if (turnFunction_state == 0) {
    turnFunction_state = 1;
    turnFunction_timer = millis();
  }

  if (turnFunction_state == 1 && labs(millis() - turnFunction_timer) < 3000) {
    startYaw(deg);
  }

  else if (turnFunction_state == 1 && labs(millis() - turnFunction_timer) > 3000 && labs(millis() - turnFunction_timer) < 5000) {
    motorDrive(-1, 0, 0);
    motorDrive(1, 0, 0);

  }

  else if (turnFunction_state == 1 && labs(millis() - turnFunction_timer) > 5000) {
    turnFunction_host++;
    CCW90Lock = 0;
    CW90Lock = 0;
    initialYaw = ypr.yaw;
    turnFunction_state = 2;
  }
}