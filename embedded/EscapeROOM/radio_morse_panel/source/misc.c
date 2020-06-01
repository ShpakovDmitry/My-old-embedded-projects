#include <util/atomic.h>
#include <stdint.h>
#include <stdbool.h>
#include "globals.h"
#include "misc.h"

bool isOneBitSet (uint8_t x) {
	return ( x && !(x & (x - 1)) ) ? true : false;
}

int8_t getSetBitPosition (uint8_t x) {
	uint8_t pos = 0;

	if (x == 0) {
		return -1;
	}
	
	while ( x != 1 ) {
		x >>= 1;
		pos++;
	}
	return pos;
}

uint32_t getDeltaTime (uint32_t t1, uint32_t t0) {
	return t1 - t0;
}

uint32_t getJiffies() {
	uint32_t tmp;

	ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
		tmp = jiffies;
	}
	return tmp;
}

