#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

void environmentBegin();
void environmentUpdate();

float environmentGetTemp();
float environmentGetHumidity();
int   environmentGetGas();
bool  environmentGetMotion();
bool  environmentHasData();

#endif