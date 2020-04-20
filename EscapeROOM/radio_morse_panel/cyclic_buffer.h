#ifndef CYCLIC_BUFFER_H
#define CYCLIC_BUFFER_H

#include <stdbool.h>

// cyclic buffer implementation
typedef struct {
	unsigned char pos;
	unsigned char size;
	char buff[MAX_PASS_LEN];
}CyclicBuffer;

// functions to handle cyclic buffer
void initCyclicBuffer(CyclicBuffer *buff);
void addCharToCyclicBuffer(CyclicBuffer *buff, const char ch);
bool findSequenceInCyclicBuffer(CyclicBuffer *buff, const char *seq);

#endif // CYCLIC_BUFFER_H
