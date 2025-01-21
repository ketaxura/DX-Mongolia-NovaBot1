// int travelStraightNormalLock = 0;
// int CCW90Lock = 0;
// int CW90Lock = 0;

float travelStraight_setpoint = 0;
float travelStraight_input;
float travelStraight_error;
float travelStraight_output;
float travelStraight_integral;
float travelStraight_derivative;
float travelStraight_lastError;
float travelStraight_Kp = 5;
float travelStraight_Ki = 0;
float travelStraight_Kd = 0;

unsigned long travelStraight_sampleTime = 10;
unsigned long travelStraight_lastTime;


//travelStraight_setpoint is the setpoint value for the PID, it is actually setpoint plus what the yaw value is currently, corrected for the logic of the gyroscope values

void travelStraight() {
  // Serial.print(travelStraight_output);
  // Serial.print("\t");
  // Serial.print(travelStraight_input);
  // Serial.print("\t");
  // Serial.print(travelStraight_error);
  // Serial.println("");




  travelStraight_setpoint = initialYaw;



  unsigned long travelStraight_currentTime = millis();
  unsigned long travelStraight_elapsedTime = travelStraight_currentTime - travelStraight_lastTime;

  if (travelStraight_elapsedTime >= travelStraight_sampleTime) {
    travelStraight_lastTime = travelStraight_currentTime;

    // Read sensor input
    travelStraight_input = ypr.yaw;

    // Calculate error
    travelStraight_error = travelStraight_setpoint - travelStraight_input;

    if (travelStraight_error > 180) {
      travelStraight_error -= 360;  // Adjust error for positive deviation
    } else if (travelStraight_error < -180) {
      travelStraight_error += 360;  // Adjust error for negative deviation
    }

    // Calculate integral and derivative terms
    travelStraight_integral += travelStraight_error * travelStraight_elapsedTime;
    travelStraight_derivative = (travelStraight_error - travelStraight_lastError) / travelStraight_elapsedTime;

    // Calculate PID output
    travelStraight_output = travelStraight_Kp * travelStraight_error + travelStraight_Ki * travelStraight_integral + travelStraight_Kd * travelStraight_derivative;

    // Update last error
    travelStraight_lastError = travelStraight_error;
  }

  if (abs(travelStraight_error) >= 1) {

    if (travelStraight_output > 30) {
      travelStraight_output = 30;
    }

    if (travelStraight_output < -30) {
      travelStraight_output = -30;
    }


    // Output is +, so we must CCW
    if (travelStraight_output > 0) {
      motorDrive(1, 1, 70 + abs(travelStraight_output));
      motorDrive(-1, -1, 70 - abs(travelStraight_output));
    }

    // Output is -, so we must CW
    if (travelStraight_output < 0) {
      motorDrive(1, 1, 70 - abs(travelStraight_output));
      motorDrive(-1, -1, 70 + abs(travelStraight_output));
    }
  }

  // else if (abs(travelStraight_error) < 1) {
  //   moveWithDir(0, 0);
  // }
}
