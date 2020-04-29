#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include "cyclic_buffer.h"

#define DEBOUNCE_TIME 100
#define DOT_DURATION 150
#define MAX_REPEAT 5
#define RED_LED PD3
#define GREEN_LED PD4

static const char morsePhraseToPlay[] = "472";
static const char PASSWORD[MAX_BUFF_LEN] = "357\0";
CyclicBuffer pressedButtons;
extern volatile uint32_t msFromReset;

#endif // GLOBALS_H

