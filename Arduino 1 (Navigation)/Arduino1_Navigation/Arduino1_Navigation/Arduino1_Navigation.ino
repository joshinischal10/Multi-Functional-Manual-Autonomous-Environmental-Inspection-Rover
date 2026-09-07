// ================================================================
// AIRO INNOVATE
// Multi-Functional Manual & Autonomous Environmental Inspection Rover
//
// ARDUINO #1 — NAVIGATION CONTROLLER
// ================================================================


#include "config.h"

#include "motors.h"
#include "line_follower.h"
#include "ultrasonic.h"
#include "bluetooth.h"
#include "safety.h"
#include "environment.h"


// ================================================================
// SETUP
// ================================================================

void setup() {

  // Motor system
  motorsBegin();


  // Line following system
  lfrBegin();


  // Ultrasonic sensors
  ultrasonicBegin();


  // Bluetooth
  bluetoothBegin();


  // Safety system
  safetyBegin();


  // Environment sensor link (Arduino 2 over hardware Serial)
  environmentBegin();


  // Give system time to initialize
  delay(2000);
}


// ================================================================
// MANUAL CONTROL
// ================================================================

void manualControl() {

  char movement = bluetoothGetMovement();


  switch (movement) {


    // ------------------------------------------------------------
    // FORWARD
    // ------------------------------------------------------------

    case 'F':

      if (safetyAllowsForward()) {

        setMotors(
          MANUAL_SPEED,
          MANUAL_SPEED
        );

      } else {

        stopMotors();
      }

      break;


    // ------------------------------------------------------------
    // REVERSE
    // ------------------------------------------------------------

    case 'B':

      if (safetyAllowsReverse()) {

        setMotors(
          -MANUAL_SPEED,
          -MANUAL_SPEED
        );

      } else {

        stopMotors();
      }

      break;


    // ------------------------------------------------------------
    // LEFT
    // ------------------------------------------------------------

    case 'L':

      // Turning is currently allowed.
      // We can later make this more sophisticated.

      setMotors(
        MANUAL_SPEED,
        -MANUAL_SPEED
      );

      break;


    // ------------------------------------------------------------
    // RIGHT
    // ------------------------------------------------------------

    case 'R':

      setMotors(
        -MANUAL_SPEED,
        MANUAL_SPEED
      );

      break;


    // ------------------------------------------------------------
    // STOP
    // ------------------------------------------------------------

    case 'S':

    default:

      stopMotors();

      break;
  }
}


// ================================================================
// AUTONOMOUS CONTROL
// ================================================================

void autonomousControl() {

  // If LFR has been disabled,
  // autonomous navigation has nothing to control.

  if (!lfrIsEnabled()) {

    stopMotors();

    return;
  }


  // --------------------------------------------------------------
  // Check front obstacle
  // --------------------------------------------------------------

  if (!safetyAllowsForward()) {

    stopMotors();

    return;
  }


  // --------------------------------------------------------------
  // Run line-following algorithm
  // --------------------------------------------------------------

  lfrUpdate();
}


// ================================================================
// TELEMETRY TIMING
//
// bluetoothSendTelemetry() must NOT be called on every loop()
// iteration — loop() runs thousands of times per second, and
// blasting that much text down the SoftwareSerial link back-to-back
// can starve bluetoothUpdate()'s ability to read incoming commands
// promptly (SoftwareSerial can't send and receive at the same time).
// TELEMETRY_INTERVAL (config.h) throttles it to a fixed interval.
// ================================================================

unsigned long lastTelemetryTime = 0;


// ================================================================
// MAIN LOOP
// ================================================================

void loop() {


  // ==============================================================
  // 1. PROCESS BLUETOOTH
  // ==============================================================

  bluetoothUpdate();


  // ==============================================================
  // 2. UPDATE SAFETY / BUZZER
  // ==============================================================

  safetyUpdate();


  // ==============================================================
  // 3. READ ENVIRONMENT SENSOR DATA (Arduino 2, hardware Serial)
  // ==============================================================

  environmentUpdate();


  // ==============================================================
  // 4. SELECT CONTROL MODE
  // ==============================================================

  if (bluetoothIsAutoMode()) {

    autonomousControl();

  }

  else {

    manualControl();
  }


  // ==============================================================
  // 5. SEND TELEMETRY (throttled — see TELEMETRY_INTERVAL in config.h)
  // ==============================================================

  unsigned long now = millis();

  if (now - lastTelemetryTime >= TELEMETRY_INTERVAL) {

    lastTelemetryTime = now;

    bluetoothSendTelemetry();
  }
}