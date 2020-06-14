#include <stdint.h>
#include <avr/io.h>
#include "cyclic_buffer.h"
#include "button.h"
#include "misc.h"

CyclicBuffer pressedButtons;

#define BTN_1_REL (RELEASED << PB0)
#define BTN_2_REL (RELEASED << PB1)
#define BTN_3_REL (RELEASED << PB2)
#define BTN_4_REL (RELEASED << PB3)
#define BTN_5_REL (RELEASED << PB4)
#define BTN_6_REL (RELEASED << PB5)
#define BTN_7_REL (RELEASED << PB6)
#define BTN_8_REL (RELEASED << PB7)
#define ALL_RELEASED	( BTN_1_REL | BTN_2_REL | BTN_3_REL | BTN_4_REL | \
						BTN_5_REL | BTN_6_REL | BTN_7_REL | BTN_8_REL )

enum ButtonState { PRESSED, RELEASED };

void updateButtonState(void) {
	typedef struct {
		unsigned char pin : 3;
		char keyChar;
	} Button;
	
	static const Button buttons[] = {
		{ PB0, '1'},
		{ PB1, '2'},
		{ PB2, '3'},
		{ PB3, '4'},
		{ PB4, '5'},
		{ PB5, '6'},
		{ PB6, '7'},
		{ PB7, '8'}
	};

	static uint32_t oldMs = 0;
	
	if (oldMs != getJiffies()) {
		static unsigned char currButtonState = ALL_RELEASED;
		static unsigned char lastButtonState = ALL_RELEASED;
		static uint32_t lastDebounceTime = 0;
		
		oldMs = getJiffies();
		unsigned char cachedPinB = PINB;
		
		if (cachedPinB != lastButtonState) {
			lastDebounceTime = getJiffies();
		}
		
		uint32_t deltaTime = getDeltaTime(getJiffies(), lastDebounceTime);
		
		if ( deltaTime > DEBOUNCE_TIME ) {
			if (cachedPinB != currButtonState) {
				currButtonState = cachedPinB;
				
				if ( isOneBitSet(~currButtonState) == true ) {
					uint8_t i = getSetBitPosition( ~currButtonState );
					addCharToCyclicBuffer(&pressedButtons, buttons[i].keyChar);
				}
			}
		}
		lastButtonState = cachedPinB;
	}
}

