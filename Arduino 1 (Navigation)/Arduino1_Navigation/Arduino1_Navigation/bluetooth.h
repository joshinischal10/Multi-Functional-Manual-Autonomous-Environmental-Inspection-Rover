#ifndef BLUETOOTH_H
#define BLUETOOTH_H

void bluetoothBegin();

void bluetoothUpdate();

bool bluetoothIsAutoMode();

bool bluetoothIsLfrEnabled();

bool bluetoothIsObstacleEnabled();

bool bluetoothIsBuzzerEnabled();

char bluetoothGetMovement();

// Sends the current status + sensor readings back to the phone app,
// e.g. "MODE:AUTONOMOUS", "FRONT:42". Call periodically from loop()
// (throttled — see TELEMETRY_INTERVAL in config.h).
void bluetoothSendTelemetry();

#endif