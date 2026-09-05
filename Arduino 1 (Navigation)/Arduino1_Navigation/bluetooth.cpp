#include <Arduino.h>
#include <SoftwareSerial.h>

#include "config.h"
#include "bluetooth.h"
#include "motors.h"
#include "line_follower.h"

SoftwareSerial bluetooth(BT_RX, BT_TX);

// ================================================================
// SYSTEM STATE
// ================================================================

bool autoMode = DEFAULT_AUTO_MODE;
bool obstacleEnabled = DEFAULT_OBSTACLE_ENABLED;
bool buzzerEnabled = DEFAULT_BUZZER_ENABLED;

char movementCommand = 'S';

// ================================================================
// BLUETOOTH STARTUP
// ================================================================

void bluetoothBegin() {
  bluetooth.begin(9600);

  // Start safely in manual mode
  autoMode = false;
  movementCommand = 'S';

  // LFR OFF in manual mode
  lfrSetEnabled(false);

  // Restore default safety settings
  obstacleEnabled = DEFAULT_OBSTACLE_ENABLED;
  buzzerEnabled = DEFAULT_BUZZER_ENABLED;

  stopMotors();
}

// ================================================================
// BLUETOOTH COMMAND PROCESSING
// ================================================================

void bluetoothUpdate() {

  while (bluetooth.available()) {

    char command = bluetooth.read();

    // Ignore terminal formatting characters
    if (command == '\r' ||
        command == '\n' ||
        command == ' ') {
      continue;
    }

    switch (command) {

      // ----------------------------------------------------------
      // FORWARD
      // ----------------------------------------------------------

      case 'F':
      case 'f':
        autoMode = false;
        lfrSetEnabled(false);
        movementCommand = 'F';
        break;


      // ----------------------------------------------------------
      // BACKWARD
      // ----------------------------------------------------------

      case 'B':
      case 'b':
        autoMode = false;
        lfrSetEnabled(false);
        movementCommand = 'B';
        break;


      // ----------------------------------------------------------
      // LEFT
      // ----------------------------------------------------------

      case 'L':
      case 'l':
        autoMode = false;
        lfrSetEnabled(false);
        movementCommand = 'L';
        break;


      // ----------------------------------------------------------
      // RIGHT
      // ----------------------------------------------------------

      case 'R':
      case 'r':
        autoMode = false;
        lfrSetEnabled(false);
        movementCommand = 'R';
        break;


      // ----------------------------------------------------------
      // STOP
      // ----------------------------------------------------------

      case 'S':
      case 's':
        movementCommand = 'S';
        stopMotors();
        break;


      // ----------------------------------------------------------
      // AUTONOMOUS MODE
      // A = Autonomous + LFR ON
      // ----------------------------------------------------------

      case 'A':
      case 'a':
        autoMode = true;
        lfrSetEnabled(true);
        movementCommand = 'S';
        break;


      // ----------------------------------------------------------
      // MANUAL MODE
      // M = Manual + LFR OFF
      // ----------------------------------------------------------

      case 'M':
      case 'm':
        autoMode = false;
        lfrSetEnabled(false);
        movementCommand = 'S';
        stopMotors();
        break;


      // ----------------------------------------------------------
      // OBSTACLE PROTECTION ON
      // O = ON
      // ----------------------------------------------------------

      case 'O':
        obstacleEnabled = true;
        break;


      // ----------------------------------------------------------
      // OBSTACLE PROTECTION OFF
      // o = OFF
      // ----------------------------------------------------------

      case 'o':
        obstacleEnabled = false;
        break;


      // ----------------------------------------------------------
      // BUZZER ON
      // Z = ON
      // ----------------------------------------------------------

      case 'Z':
        buzzerEnabled = true;
        break;


      // ----------------------------------------------------------
      // BUZZER OFF
      // z = OFF
      // ----------------------------------------------------------

      case 'z':
        buzzerEnabled = false;
        break;


      // ----------------------------------------------------------
      // UNKNOWN COMMAND
      // ----------------------------------------------------------

      default:
        break;
    }
  }
}

// ================================================================
// STATUS FUNCTIONS
// ================================================================

bool bluetoothIsAutoMode() {
  return autoMode;
}

bool bluetoothIsLfrEnabled() {
  return lfrIsEnabled();
}

bool bluetoothIsObstacleEnabled() {
  return obstacleEnabled;
}

bool bluetoothIsBuzzerEnabled() {
  return buzzerEnabled;
}

char bluetoothGetMovement() {
  return movementCommand;
}