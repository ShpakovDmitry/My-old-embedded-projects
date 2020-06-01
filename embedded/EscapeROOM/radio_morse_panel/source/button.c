#include <stdint.h>
#include "globals.h"
#include "button.h"
#include "misc.h"

CyclicBuffer pressedButtons;

void updateButtonState(void) {
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

