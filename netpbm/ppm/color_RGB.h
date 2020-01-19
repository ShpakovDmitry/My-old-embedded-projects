#define MAX_VAL		255


typedef struct
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
}color_RGB;

const color_RGB COLOR_BLACK = {0x00, 0x00, 0x00};
const color_RGB COLOR_WHITE = {0xff, 0xff, 0xff};
const color_RGB COLOR_RED   = {0xff, 0x00, 0x00};
const color_RGB COLOR_GREEN = {0x00, 0xff, 0x00};
const color_RGB COLOR_BLUE  = {0x00, 0x00, 0xff};



int color_RGB_add(color_RGB *color1, color_RGB *color2);
int color_RGB_add_saturation(color_RGB *color1, color_RGB *color2);




