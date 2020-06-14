#include "interrupts.h"
#include <avr/interrupt.h>

volatile uint32_t jiffies;

ISR (TIMER0_COMPA_vect) {
	jiffies++;
}

