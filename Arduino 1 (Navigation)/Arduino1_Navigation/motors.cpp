#include <Arduino.h>
#include "config.h"
#include "motors.h"


// ================================================================
// INITIALIZE MOTORS
// ================================================================

void motorsBegin() {

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();
}


// ================================================================
// SET MOTOR SPEED
//
// leftSpeed / rightSpeed:
//
//   0     = stopped
//   +255  = maximum forward
//   -255  = maximum reverse
// ================================================================

void setMotors(int leftSpeed, int rightSpeed) {

  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);


  // --------------------------------------------------------------
  // LEFT MOTOR GROUP
  // --------------------------------------------------------------

  if (leftSpeed >= 0) {

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

  } else {

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    leftSpeed = -leftSpeed;
  }

  analogWrite(ENA, leftSpeed);


  // --------------------------------------------------------------
  // RIGHT MOTOR GROUP
  // --------------------------------------------------------------

  if (rightSpeed >= 0) {

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

  } else {

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    rightSpeed = -rightSpeed;
  }

  analogWrite(ENB, rightSpeed);
}


// ================================================================
// STOP
// ================================================================

void stopMotors() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}