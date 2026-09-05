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
  // 3. SELECT CONTROL MODE
  // ==============================================================

  if (bluetoothIsAutoMode()) {

    autonomousControl();

  }

  else {

    manualControl();
  }
}