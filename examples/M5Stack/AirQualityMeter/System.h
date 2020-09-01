#pragma once

void initSystem();
void shutdownSystem();
void restartSystem();
void haltSystem();

typedef enum { NONE, BTN1, BTN2, RESET } ButtonCode;

ButtonCode getPressedButton();