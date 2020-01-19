#include "ppm.c"


int main(int argc, char **argv)
{
	image_ppm image1;
	size_t width  =  1600*1;
	size_t height =  900*1;
	
	if(image_init(width, height, &image1) == INIT_ERROR)
		return 0;
	
	image_clear(&image1);
	
	color_RGB color = {255, 255, 255};
	image_point center = {200, 200};
	int radius = 50;
	image_draw_circle(&image1, color, center, radius);
	
	image_write("image1.ppm", &image1);
	
	image_destroy(&image1);

	return 0;
}
