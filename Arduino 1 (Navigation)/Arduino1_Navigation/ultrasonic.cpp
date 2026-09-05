#include <Arduino.h>
#include "config.h"
#include "ultrasonic.h"


// ================================================================
// INITIALIZE
// ================================================================

void ultrasonicBegin() {

  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);

  pinMode(REAR_TRIG, OUTPUT);
  pinMode(REAR_ECHO, INPUT);

  digitalWrite(FRONT_TRIG, LOW);
  digitalWrite(REAR_TRIG, LOW);
}


// ================================================================
// READ ONE ULTRASONIC SENSOR
// ================================================================

long readUltrasonic(uint8_t trigPin, uint8_t echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);


  long duration = pulseIn(
    echoPin,
    HIGH,
    ULTRASONIC_TIMEOUT
  );


  // No echo received.
  // Treat this as "far away".
  if (duration == 0) {
    return 999;
  }


  // Speed of sound ≈ 0.0343 cm/us
  long distance = duration * 0.0343 / 2;

  return distance;
}


// ================================================================
// FRONT
// ================================================================

long getFrontDistance() {

  return readUltrasonic(
    FRONT_TRIG,
    FRONT_ECHO
  );
}


// ================================================================
// REAR
// ================================================================

long getRearDistance() {

  return readUltrasonic(
    REAR_TRIG,
    REAR_ECHO
  );
}