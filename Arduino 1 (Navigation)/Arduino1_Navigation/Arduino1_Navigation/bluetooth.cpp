#include <Arduino.h>
#include <SoftwareSerial.h>

#include "config.h"
#include "bluetooth.h"
#include "motors.h"
#include "line_follower.h"
#include "environment.h"
#include "ultrasonic.h"

SoftwareSerial bluetooth(BT_RX, BT_TX);

// ================================================================
// SYSTEM STATE
// ================================================================

bool autoMode = DEFAULT_AUTO_MODE;
bool obstacleEnabled = DEFAULT_OBSTACLE_ENABLED;
bool buzzerEnabled = DEFAULT_BUZZER_ENABLED;

char movementCommand = 'S';

// Timestamp of the last valid byte received from the phone.
// Used by the safety watchdog below.
unsigned long lastCommandTime = 0;

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

  // Avoid an immediate false-positive watchdog trip at boot,
  // before any command has ever been received.
  lastCommandTime = millis();

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

    // Any real command byte resets the watchdog clock.
    lastCommandTime = millis();

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

  // ================================================================
  // SAFETY WATCHDOG
  //
  // If we're in manual mode, currently moving, and haven't heard a
  // single valid byte from the phone in COMMAND_TIMEOUT_MS, force a
  // stop. This protects against a dropped 'S' (stop) byte - e.g. a
  // brief SoftwareSerial collision or electrical noise - leaving the
  // rover driving indefinitely after the user has released the
  // button (or the Bluetooth link has dropped entirely).
  //
  // Only applies in manual mode: in autonomous mode the rover is
  // meant to keep driving on its own via the line follower without
  // needing a constant stream of phone commands.
  // ================================================================

  if (!autoMode &&
      movementCommand != 'S' &&
      millis() - lastCommandTime > COMMAND_TIMEOUT_MS) {

    movementCommand = 'S';
    stopMotors();
  }
}

// ================================================================
// TELEMETRY (Arduino -> Phone)
//
// Sends KEY:VALUE lines that the Flutter app's updateFromTelemetry()
// parses directly. Called periodically from loop() (see
// TELEMETRY_INTERVAL in config.h).
//
// TEMP/HUM/GAS only send once Arduino 2 has delivered at least one
// reading over the environment link (environmentHasData()).
// ================================================================

void bluetoothSendTelemetry() {
  bluetooth.print("MODE:");
  bluetooth.println(autoMode ? "AUTONOMOUS" : "MANUAL");

  bluetooth.print("MOVEMENT:");
  bluetooth.println(movementCommand);

  bluetooth.print("OBSTACLE:");
  bluetooth.println(obstacleEnabled ? "ON" : "OFF");

  bluetooth.print("BUZZER:");
  bluetooth.println(buzzerEnabled ? "ON" : "OFF");

  bluetooth.print("FRONT:");
  bluetooth.println(getFrontDistance());

  bluetooth.print("REAR:");
  bluetooth.println(getRearDistance());

  if (environmentHasData()) {
    bluetooth.print("TEMP:");
    bluetooth.println(environmentGetTemp());

    bluetooth.print("HUM:");
    bluetooth.println(environmentGetHumidity());

    bluetooth.print("GAS:");
    bluetooth.println(environmentGetGas());

    bluetooth.print("MOTION:");
    bluetooth.println(environmentGetMotion() ? "1" : "0");
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