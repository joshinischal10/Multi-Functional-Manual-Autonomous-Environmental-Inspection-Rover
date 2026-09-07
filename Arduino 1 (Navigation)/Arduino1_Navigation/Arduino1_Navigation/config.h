// ================================================================
// DEFAULT SYSTEM STATE
// ================================================================

#define DEFAULT_AUTO_MODE          false
#define DEFAULT_LFR_ENABLED        false
#define DEFAULT_OBSTACLE_ENABLED   true
#define DEFAULT_BUZZER_ENABLED     true
#ifndef CONFIG_H
#define CONFIG_H

// ================================================================
// AIRO INNOVATE
// Multi-Functional Manual & Autonomous Environmental Inspection Rover
//
// ARDUINO #1 — NAVIGATION SYSTEM
// ================================================================


// ================================================================
// MOTOR DRIVER — L298N
// ================================================================

#define ENA 6
#define IN1 9
#define IN2 8

#define ENB 5
#define IN3 7
#define IN4 4


// ================================================================
// BLUETOOTH MODULE
//
// Bluetooth RX -> Arduino TX
// Bluetooth TX -> Arduino RX
//
// SoftwareSerial:
// Arduino D2 = RX
// Arduino D3 = TX
// ================================================================

#define BT_RX 3
#define BT_TX 2


// ================================================================
// 5-CHANNEL IR SENSOR ARRAY
//
// From the ROBOT'S perspective:
//
//                  FRONT
//                    ↑
//
//          S5    S4    S3    S2    S1
//          ●     ●     ●     ●     ●
//
//          A0   D13   D12   D11   D10
//
// Internally we store them LEFT -> RIGHT.
// ================================================================

#define IR_LEFTMOST   10
#define IR_LEFT       11
#define IR_CENTER     12
#define IR_RIGHT      13
#define IR_RIGHTMOST  A0


// ================================================================
// FRONT ULTRASONIC
// ================================================================

#define FRONT_TRIG A1
#define FRONT_ECHO A2


// ================================================================
// REAR ULTRASONIC
// ================================================================

#define REAR_TRIG A3
#define REAR_ECHO A4


// ================================================================
// BUZZER
//
// A5 is currently the remaining spare Arduino pin.
// ================================================================

#define BUZZER_PIN A5


// ================================================================
// SPEED SETTINGS
// ================================================================

#define BASE_SPEED       150
#define GENTLE_INNER     110
#define MEDIUM_INNER     130

#define MANUAL_SPEED     150


// ================================================================
// OBSTACLE DISTANCES
// ================================================================

// Warning distance:
// buzzer gives warning but movement is still allowed.

#define OBSTACLE_WARNING_DISTANCE 30


// Stop distance:
// robot will not move toward an obstacle closer than this.

#define OBSTACLE_STOP_DISTANCE 15


// ================================================================
// ULTRASONIC SETTINGS
// ================================================================

#define ULTRASONIC_TIMEOUT 25000


// ================================================================
// COMMUNICATION TIMING
// ================================================================

// If no valid byte arrives from the phone for this long while
// actively moving in manual mode, the safety watchdog in
// bluetooth.cpp forces a stop. Comfortably above the app's
// hold-resend interval (150ms) so a couple of missed resends in a
// row won't falsely trip it, but still short enough to react quickly
// to a real dropped connection.
#define COMMAND_TIMEOUT_MS 500

// How often bluetoothSendTelemetry() is called from loop() (see
// Arduino1_Navigation.ino). Sending on every loop() iteration would
// flood the SoftwareSerial link and can delay incoming commands.
#define TELEMETRY_INTERVAL 200

#endif