#include "globals.h"
#include "interrupts.h"

volatile uint32_t msFromReset;

ISR (TIMER0_COMPA_vect) {
	msFromReset++;
}

