#include <stdbool.h>
#include <stdint.h>
#include "button.h"
#include "cyclic_buffer.h"
#include "globals.h"
#include "init.h"
#include "morse.h"

#define MAX_REPEAT 5

const char gMorsePhraseToPlay[] = "472";
const char PASSWORD[MAX_BUFF_LEN] = "357\0";

void main() {
	initContoroller();
	initCyclicBuffer(&pressedButtons);
	
	while (1) {
		updateButtonState();
		if (findSequenceInCyclicBuffer(&pressedButtons, PASSWORD) == true) {
			initCyclicBuffer(&pressedButtons);
			for (uint8_t i = 0; i < MAX_REPEAT; i++) {
				playMorsePhrase(gMorsePhraseToPlay);
			}
		}
	
	}
}

