#include <avr/io.h>
#include <avr/interrupt.h>
#include "globals.h"
#include "init.h"
#include "hw_config.h"

void initIO(void) {
	INIT_RED_LED();
	INIT_GREEN_LED();
	INIT_BUZZER();
	INIT_BUTTON1();
	INIT_BUTTON2();
	INIT_BUTTON3();
	INIT_BUTTON4();
	INIT_BUTTON5();
	INIT_BUTTON6();
	INIT_BUTTON7();
	INIT_BUTTON8();
	PULL_UP_ENABLE_BUTTON1();
	PULL_UP_ENABLE_BUTTON2();
	PULL_UP_ENABLE_BUTTON3();
	PULL_UP_ENABLE_BUTTON4();
	PULL_UP_ENABLE_BUTTON5();
	PULL_UP_ENABLE_BUTTON6();
	PULL_UP_ENABLE_BUTTON7();
	PULL_UP_ENABLE_BUTTON8();
}

void initTimer0(void) {
 	//init TIMER0 in CTC mode, to generate 1ms frequency interrupt, assuming that F_CPU = 16 MHz
	TCCR0A |= (1 << WGM01);

	#if ( (F_CPU / 1000) / 64 ) > 0x00ff
		#define PRESCALER_MASK ( (1 << CS02) | (0 << CS01) | (0 << CS00) )
		#define OCRA_VALUE ( (F_CPU / 1000) / 128 )
	#else
		#define PRESCALER_MASK ( (0 << CS02) | (1 << CS01) | (1 << CS00) )
		#define OCRA_VALUE ( (F_CPU / 1000) / 64 )
	#endif

	// F_CPU prescaler
	TCCR0B |= PRESCALER_MASK;
	//
	OCR0A = OCRA_VALUE;
	// enable output compare A match interrupt
	TIMSK |= (1 << OCIE0A);
	// start counting ms at this point; actually when glob interrupt flag is set
	jiffies = 0;   
}

void initContoroller(void) {
	initIO();
	initTimer0();
	sei();
}

