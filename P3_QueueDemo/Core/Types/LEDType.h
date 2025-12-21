#ifndef __LED_TYPE_H
#define __LED_TYPE_H

#include "cmsis_os2.h"

typedef enum {
    LEDColor_Red = 0,
    LEDColor_Green = 1,
    LEDColor_Blue = 2
} LEDColor;

typedef enum {
    LEDState_On = 1,
    LEDState_Off = 0
} LEDState;

typedef struct {
    LEDColor color;
    LEDState state;
} LEDMessage;

#endif /* __LED_TYPE_H */