#include <Arduino.h>
#include "config.h"
#include "environment.h"

// ================================================================
// Uses Arduino 1's hardware UART (pins 0/1) to talk to Arduino 2.
//
// NOTE: pins 0/1 are also used for USB programming / Serial Monitor
// on Arduino 1. Disconnect the wire to Arduino 2 before re-uploading
// code to Arduino 1 over USB, then reconnect it afterward.
// ================================================================

static float envTemp = 0;
static float envHumidity = 0;
static int   envGas = 0;
static bool  envMotion = false;
static bool  gotData = false;

static String buffer = "";

void environmentBegin() {
  Serial.begin(9600); // must match Arduino 2's BAUD_RATE
}

void environmentUpdate() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      // Expected: temp,hum,motion,gas
      int p1 = buffer.indexOf(',');
      int p2 = buffer.indexOf(',', p1 + 1);
      int p3 = buffer.indexOf(',', p2 + 1);

      if (p1 > 0 && p2 > p1 && p3 > p2) {
        envTemp     = buffer.substring(0, p1).toFloat();
        envHumidity = buffer.substring(p1 + 1, p2).toFloat();
        envMotion   = buffer.substring(p2 + 1, p3) == "1";
        envGas      = buffer.substring(p3 + 1).toInt();
        gotData = true;
      }

      buffer = "";
    } else if (c != '\r') {
      buffer += c;
    }
  }
}

float environmentGetTemp()     { return envTemp; }
float environmentGetHumidity() { return envHumidity; }
int   environmentGetGas()      { return envGas; }
bool  environmentGetMotion()   { return envMotion; }
bool  environmentHasData()     { return gotData; }