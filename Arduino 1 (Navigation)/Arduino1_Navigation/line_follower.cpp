#include <Arduino.h>
#include "config.h"
#include "motors.h"
#include "line_follower.h"



// ================================================================
// IR SENSOR PINS
//
// LEFT -> RIGHT
// ================================================================

const uint8_t SENSOR_PINS[5] = {
  IR_LEFTMOST,
  IR_LEFT,
  IR_CENTER,
  IR_RIGHT,
  IR_RIGHTMOST
};


// ================================================================
// LAST MOTOR COMMAND
//
// Used by the recovery algorithm.
// ================================================================

int lastL = 0;
int lastR = 0;


// ================================================================
// LFR STATE
// ================================================================

bool lfrEnabled = DEFAULT_LFR_ENABLED;


// ================================================================
// INITIALIZE
// ================================================================

void lfrBegin() {

  for (int i = 0; i < 5; i++) {
    pinMode(SENSOR_PINS[i], INPUT);
  }

  lfrEnabled = DEFAULT_LFR_ENABLED;
}


// ================================================================
// ENABLE / DISABLE
// ================================================================

void lfrSetEnabled(bool enabled) {

  lfrEnabled = enabled;

  if (!enabled) {
    stopMotors();
  }
}


bool lfrIsEnabled() {

  return lfrEnabled;
}


// ================================================================
// ACTION LOGIC
// ================================================================

void getAction(int pattern, int &L, int &R) {

  switch (pattern) {


    // ============================================================
    // STRAIGHT
    // ============================================================

    case 0b11011:
    case 0b00100:
    case 0b01110:
    case 0b10001:

      L = BASE_SPEED;
      R = BASE_SPEED;

      break;


    // ============================================================
    // LEFT TURNS
    // ============================================================

    case 0b11101:
    case 0b11001:

      L = BASE_SPEED;
      R = -BASE_SPEED;

      break;


    case 0b11100:

      L = BASE_SPEED;
      R = -130;

      break;


    case 0b11110:

      L = BASE_SPEED;
      R = -140;

      break;


    // ============================================================
    // RIGHT TURNS
    // ============================================================

    case 0b10111:
    case 0b10011:

      L = -BASE_SPEED;
      R = BASE_SPEED;

      break;


    case 0b00111:

      L = -130;
      R = BASE_SPEED;

      break;


    case 0b01111:

      L = -140;
      R = BASE_SPEED;

      break;


    // ============================================================
    // DEFAULT AUTO BALANCE
    // ============================================================

    default: {

      int leftWeight =
        ((pattern >> 4) & 1) * 2 +
        ((pattern >> 3) & 1);

      int rightWeight =
        ((pattern >> 1) & 1) +
        ((pattern >> 0) & 1) * 2;


      if (leftWeight > rightWeight) {

        L = GENTLE_INNER;
        R = BASE_SPEED;

      }

      else if (rightWeight > leftWeight) {

        L = BASE_SPEED;
        R = GENTLE_INNER;

      }

      else {

        L = BASE_SPEED;
        R = BASE_SPEED;
      }

      break;
    }
  }
}


// ================================================================
// UPDATE LFR
// ================================================================

void lfrUpdate() {

  if (!lfrEnabled) {
    return;
  }


  // ============================================================
  // READ SENSORS
  // ============================================================

  int s1 = digitalRead(SENSOR_PINS[0]);
  int s2 = digitalRead(SENSOR_PINS[1]);
  int s3 = digitalRead(SENSOR_PINS[2]);
  int s4 = digitalRead(SENSOR_PINS[3]);
  int s5 = digitalRead(SENSOR_PINS[4]);


  // ============================================================
  // CREATE SENSOR PATTERN
  //
  // s1 s2 s3 s4 s5
  //  4  3  2  1  0
  // ============================================================

  int pattern =
    (s1 << 4) |
    (s2 << 3) |
    (s3 << 2) |
    (s4 << 1) |
    s5;


  // ============================================================
  // RECOVERY
  // ============================================================

  if (pattern == 0b11111) {

    if (lastL == lastR) {

      setMotors(-120, -120);

    }

    else if (lastL > lastR) {

      setMotors(180, -160);

    }

    else {

      setMotors(-160, 180);
    }

    return;
  }


  // ============================================================
  // NORMAL LINE FOLLOWING
  // ============================================================

  int L, R;

  getAction(pattern, L, R);

  setMotors(L, R);

  lastL = L;
  lastR = R;
}