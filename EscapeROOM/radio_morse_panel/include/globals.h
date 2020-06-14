#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include "cyclic_buffer.h"

extern const char gMorsePhraseToPlay[];
extern const char PASSWORD[MAX_BUFF_LEN];
extern CyclicBuffer pressedButtons;
extern volatile uint32_t jiffies;

#endif // GLOBALS_H

