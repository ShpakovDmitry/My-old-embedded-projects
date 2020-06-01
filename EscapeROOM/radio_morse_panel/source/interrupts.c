#include "globals.h"
#include "interrupts.h"

volatile uint32_t jiffies;

ISR (TIMER0_COMPA_vect) {
	jiffies++;
}

