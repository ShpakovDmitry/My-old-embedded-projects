#include <string.h>
#include <stdbool.h>
#include "cyclic_buffer.h"

void initCyclicBuffer(CyclicBuffer *buff) {
	buff->pos = 0;
	buff->size = MAX_PASS_LEN;
	memset (buff->buff, 0, buff->size);
}

void addCharToCyclicBuffer(CyclicBuffer *buff, const char ch) {
	buff->buff[buff->pos] = ch;
	buff->pos++;
	if (buff->pos >= buff->size) {
		buff->pos = 0;
	}
}

bool findSequenceInCyclicBuffer(CyclicBuffer *buff, const char *seq) {
	unsigned char i;
	bool flag = true;
	
	for(i = 0; i < buff->size && seq[i] != '\0'; ++i)
	{
		if( buff->buff[(buff->pos + i) % (buff->size)] != seq[i] )
			flag = false;
	}
	
	return flag;
}


