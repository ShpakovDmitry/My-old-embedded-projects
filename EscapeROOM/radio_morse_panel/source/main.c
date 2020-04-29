#include <stdbool.h>
#include <stdint.h>
#include "cyclic_buffer.h"
#include "globals.h"
#include "init.h"
#include "interrupts.h"
#include "misc.h"
#include "morse.h"

void main() {
	initContoroller();
	initCyclicBuffer(&pressedButtons);
	
	while (1) {
		updateButtonState();
		if (findSequenceInCyclicBuffer(&pressedButtons, PASSWORD) == true) {
			initCyclicBuffer(&pressedButtons);
			for (uint8_t i = 0; i < MAX_REPEAT; i++) {
				playMorsePhrase(morsePhraseToPlay);
			}
		}
	
	}
}

