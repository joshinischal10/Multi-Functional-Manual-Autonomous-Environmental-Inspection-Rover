#ifndef BLUETOOTH_H
#define BLUETOOTH_H

void bluetoothBegin();

void bluetoothUpdate();

bool bluetoothIsAutoMode();

bool bluetoothIsLfrEnabled();

bool bluetoothIsObstacleEnabled();

bool bluetoothIsBuzzerEnabled();

char bluetoothGetMovement();

#endif