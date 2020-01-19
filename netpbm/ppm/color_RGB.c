#include "color_RGB.h"



int color_RGB_add(color_RGB *color1, color_RGB *color2)
{
	if(color1 == NULL || color2 == NULL)
		return EXIT_FAILURE
	
	color1->R = (color1->R + color2->R) % (MAX_VAL + 1);
	color1->G = (color1->G + color2->G) % (MAX_VAL + 1);
	color1->B = (color1->B + color2->B) % (MAX_VAL + 1);
	
	return EXIT_SUCCESS;
}

int color_RGB_add_saturation(color_RGB *color1, color_RGB *color2)
{
	if(color1 == NULL || color2 == NULL)
		return EXIT_FAILURE
	
	if( (color1->R + color2->R) > MAX_VAL )
	//TODO implement
	
	color1->R = (color1->R + color2->R) % (MAX_VAL + 1);
	color1->G = (color1->G + color2->G) % (MAX_VAL + 1);
	color1->B = (color1->B + color2->B) % (MAX_VAL + 1);
	
	return EXIT_SUCCESS;
	
}







