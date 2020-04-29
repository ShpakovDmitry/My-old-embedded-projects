#ifndef CYCLIC_BUFFER_H
#define CYCLIC_BUFFER_H

#include <stdbool.h>

#define MAX_BUFF_LEN 10

typedef struct {
	unsigned char pos;
	unsigned char size;
	char buff[MAX_BUFF_LEN];
}CyclicBuffer;

void initCyclicBuffer(CyclicBuffer *buff);
void addCharToCyclicBuffer(CyclicBuffer *buff, const char ch);
bool findSequenceInCyclicBuffer(CyclicBuffer *buff, const char *seq);

#endif // CYCLIC_BUFFER_H

