#include <Arduino.h>

#include "config.h"
#include "safety.h"
#include "ultrasonic.h"
#include "bluetooth.h"
#include "motors.h"


// ================================================================
// BUZZER STATE
// ================================================================

unsigned long lastBeepTime = 0;

bool buzzerState = false;


// ================================================================
// INITIALIZE
// ================================================================

void safetyBegin() {

  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);

  lastBeepTime = 0;
}


// ================================================================
// FORWARD SAFETY CHECK
// ================================================================

bool safetyAllowsForward() {

  if (!bluetoothIsObstacleEnabled()) {
    return true;
  }


  long distance = getFrontDistance();


  if (distance <= OBSTACLE_STOP_DISTANCE) {

    return false;
  }


  return true;
}


// ================================================================
// REVERSE SAFETY CHECK
// ================================================================

bool safetyAllowsReverse() {

  if (!bluetoothIsObstacleEnabled()) {
    return true;
  }


  long distance = getRearDistance();


  if (distance <= OBSTACLE_STOP_DISTANCE) {

    return false;
  }


  return true;
}


// ================================================================
// BUZZER UPDATE
// ================================================================

void safetyUpdate() {

  if (!bluetoothIsBuzzerEnabled()) {

    digitalWrite(BUZZER_PIN, LOW);

    return;
  }


  // --------------------------------------------------------------
  // Determine relevant direction
  // --------------------------------------------------------------

  char movement = bluetoothGetMovement();

  long distance = 999;


  if (bluetoothIsAutoMode()) {

    // Autonomous LFR normally moves forward.
    distance = getFrontDistance();

  }

  else {

    if (movement == 'F') {

      distance = getFrontDistance();

    }

    else if (movement == 'B') {

      distance = getRearDistance();

    }

    else {

      digitalWrite(BUZZER_PIN, LOW);

      return;
    }
  }


  // --------------------------------------------------------------
  // CRITICAL
  // --------------------------------------------------------------

  if (distance <= OBSTACLE_STOP_DISTANCE) {

    digitalWrite(BUZZER_PIN, HIGH);

    return;
  }


  // --------------------------------------------------------------
  // WARNING
  // --------------------------------------------------------------

  if (distance <= OBSTACLE_WARNING_DISTANCE) {

    unsigned long now = millis();

    if (now - lastBeepTime >= 400) {

      lastBeepTime = now;

      buzzerState = !buzzerState;

      digitalWrite(
        BUZZER_PIN,
        buzzerState
      );
    }

    return;
  }


  // --------------------------------------------------------------
  // SAFE
  // --------------------------------------------------------------

  digitalWrite(BUZZER_PIN, LOW);

  buzzerState = false;
}