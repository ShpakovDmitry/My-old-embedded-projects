#include <stdio.h>

#define BLACK 1
#define WHITE 0

typedef struct image_pbm
{
	unsigned width;
	unsigned height;
	char *data;
}
