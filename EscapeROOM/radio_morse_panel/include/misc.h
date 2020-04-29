#ifndef MISC_H
#define MISC_H

#include <stdint.h>
#include <stdbool.h>

bool isOneBitSet (uint8_t x);
uint8_t getSetBitPosition (uint8_t x);
uint32_t getDeltaTime (uint32_t t1, uint32_t t0);
void updateButtonState(void);

#endif // MISC_H

