// implement binary ppm image
// chanel is 1 byte long

#include <stdio.h>		// fprintf(), fopen(), fclose();
#include <stdlib.h>		// malloc(),
#include <math.h>		// fabs(),
#include "ppm.h"


image_point image_make_point(int x, int y)
{
	image_point point;
	
	point.x = x;
	point.y = y;
	
	return point;
}

int image_write(char *filename, image_ppm *img)
{
	FILE *f;
	size_t i;
	
	f = fopen(filename, "write");
	if(f == NULL)
		return WRITE_ERROR;
	
	fprintf(f, "P6\n%ld %ld\n255\n", img->width, img->height);
	for(i = 0; i < img->width * img->height; ++i)
		fprintf(f, "%c%c%c", (img->data + i)->R, (img->data + i)->G, (img->data + i)->B);
	
	fclose(f);
	return WRITE_OK;
}

int image_read(char *filename, image_ppm *img)
{
	FILE *f;
	size_t i;
	char ch_P, ch_6;
	unsigned int max_val;
	size_t width, height;
	
	f = fopen(filename, "read");
	if(f == NULL)
		return READ_ERROR;
	
	fscanf(f,"%c%c\n%ld %ld\n%d\n", &ch_P, &ch_6, &width, &height, &max_val);
	
	if(ch_P != 'P' || ch_6 != '6' || max_val != MAX_VAL)
	{
		fclose(f);
		return READ_ERROR;
	}
	
	if( image_init(width, height, img) == INIT_ERROR )
	{
		fclose(f);
		return READ_ERROR;
	}
	
	for(i = 0; i < width*height; ++i)
		fscanf(f, "%c%c%c", &(img->data + i)->R, &(img->data + i)->G, &(img->data + i)->B);
	
	fclose(f);
	return READ_OK;
	
}

int image_init(size_t width, size_t height, image_ppm *img)
{
	img->data = (color_RGB *)malloc(width*height*sizeof(color_RGB));
	if(img->data == NULL)
		return INIT_ERROR;
	img->width = width;
	img->height = height;
	return INIT_OK;
}

int image_destroy(image_ppm *img)
{
	free(img->data);
	return EXIT_SUCCESS;
}

int image_point_belongs_to(image_ppm *img, image_point point)
{
	if(point.x >= img->width || point.y >= img->height ||\
		point.x < 0 || point.y < 0)
		return FALSE;
	return TRUE;
	
}

int image_set_point(image_ppm *img, color_RGB color, image_point point)
{
	if( image_point_belongs_to(img, point) == FALSE )
		return EXIT_FAILURE;
	
	*(img->data + img->width*point.y + point.x) = color;
	
	return EXIT_SUCCESS;
}

int image_set_point_additive(image_ppm *img, color_RGB color, image_point point)
{
	if( image_point_belongs_to(img, point) == FALSE )
		return EXIT_FAILURE;
	
	color_RGB tmp_color;
	
	image_get_point(img, &tmp_color, point);
	tmp_color.R = (tmp_color.R + color.R) % (MAX_VAL + 1);
	tmp_color.G = (tmp_color.G + color.G) % (MAX_VAL + 1);
	tmp_color.B = (tmp_color.B + color.B) % (MAX_VAL + 1);
	
	*(img->data + img->width*point.y + point.x) = tmp_color;
	
	return EXIT_SUCCESS;
}

int image_get_point(image_ppm *img, color_RGB *color, image_point point)
{
	if( image_point_belongs_to(img, point) == FALSE )
	{
		*color = COLOR_BLACK;
		return EXIT_FAILURE;
	}
		
	*color = *(img->data + img->width*point.y + point.x);
	
	return EXIT_SUCCESS;
}

int image_clear(image_ppm *img)
{
	image_point tmp_point;
	
	for(tmp_point.y = 0; tmp_point.y < img->height; ++tmp_point.y)
		for(tmp_point.x = 0; tmp_point.x < img->width; ++tmp_point.x)
			image_set_point(img, COLOR_BLACK, tmp_point);
	
	return EXIT_SUCCESS;
}

int image_equal_size(image_ppm *img1, image_ppm *img2)
{
	if( (img1->width != img2->width) || (img1->height != img2->height) )
		return FALSE;
	return TRUE;
}

int image_equal(image_ppm *img1, image_ppm *img2)
{
	color_RGB color1, color2;
	image_point tmp_point;
	
	if( !image_equal_size(img1, img2) )
		return FALSE;
	
	for(tmp_point.y = 0; tmp_point.y < img1->height; ++tmp_point.y)
		for(tmp_point.x = 0; tmp_point.x < img1->width; ++tmp_point.x)
		{
			image_get_point(img1, &color1, tmp_point);
			image_get_point(img2, &color2, tmp_point);
			
			if(color1.R != color2.R || color1.G != color2.G || color1.B != color2.B)
				return FALSE;
		}
	
	return TRUE;
}

int image_copy(image_ppm *img_to, image_ppm *img_from)
{
	color_RGB color;
	image_point tmp_point;
	
	if( !image_equal_size(img_to, img_from) )
		return EXIT_FAILURE;
	
	for(tmp_point.y = 0; tmp_point.y < img_to->height; ++tmp_point.y)
		for(tmp_point.x = 0; tmp_point.x < img_to->width; ++tmp_point.x)
		{
			image_get_point(img_from, &color, tmp_point);
			image_set_point(img_to, color, tmp_point);
		}
	return EXIT_SUCCESS;
}

int image_add_image(image_ppm *img1, image_ppm *img2)
{
	color_RGB color1, color2;
	image_point tmp_point;
	
	if( !image_equal_size(img1, img2) )
		return EXIT_FAILURE;
	
	for(tmp_point.y = 0; tmp_point.y < img1->height; ++tmp_point.y)
		for(tmp_point.x = 0; tmp_point.x < img1->width; ++tmp_point.x)
		{
			image_get_point(img1, &color1, tmp_point);
			image_get_point(img2, &color2, tmp_point);
			color1.R = (color1.R + color2.R) % (MAX_VAL + 1);
			color1.G = (color1.G + color2.G) % (MAX_VAL + 1);
			color1.B = (color1.B + color2.B) % (MAX_VAL + 1);
			image_set_point(img1, color1, tmp_point);
		}
	
	return EXIT_SUCCESS;
}

int image_draw_v_line(image_ppm *img, color_RGB color, int y1, int y2, int x1)
{
	int y_start, y_end, y, x;
	
	if(y1 > y2)
	{
		y_start = y2;
		y_end   = y1;
	}
	else
	{
		y_start = y1;
		y_end   = y2;
	}
	x = x1;
	
	for(y = y_start; y <= y_end; y++)
		image_set_point(img, color, image_make_point(x, y));
	
	return EXIT_SUCCESS;
}

int image_draw_h_line(image_ppm *img, color_RGB color, int x1, int x2, int y1)
{
	int x_start, x_end, y, x;
	
	if(x1 > x2)
	{
		x_start = x2;
		x_end   = x1;
	}
	else
	{
		x_start = x1;
		x_end   = x2;
	}
	y = y1;
	
	for(x = x_start; x <= x_end; x++)
		image_set_point(img, color, image_make_point(x, y));
	
	return EXIT_SUCCESS;
}

int image_draw_line(image_ppm *img, color_RGB color, image_point p1, image_point p2)
{
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;
	float x, y;
	float xstep, ystep;
	
	if(dx > 0)
		xstep = 1.0;
	else if(dx < 0)
		xstep = -1.0f;
	else
	{
		image_draw_v_line(img, color, p1.y, p2.y, p1.x);
		return EXIT_SUCCESS;
	}
	
	if(dy > 0)
		ystep = 1.0;
	else if(dy < 0)
		ystep = -1.0f;
	else
	{
		image_draw_h_line(img, color, p1.x, p2.x, p1.y);
		return EXIT_SUCCESS;
	}
	
	x = p1.x;
	y = p1.y;
	while(x != p2.x || y != p2.y)
	{
		image_set_point(img, color, image_make_point(x, y));
		if( fabs( dy*(x+xstep-p1.x)-dx*(y-p1.y) ) < fabs(dy*(x-p1.x)-dx*(y+ystep-p1.y) ) )
			x+=xstep;
		else
			y+=ystep;
	}
	
	return EXIT_SUCCESS;
}

int image_draw_circle(image_ppm *img, color_RGB color, image_point center, int radius)
{
	// (x-x0)^2 + (y-y0)^2 - R^2 = 0
	int x, y, x_last, y_last, x_min, y_min, x_start, y_start;
	float val_curr, val_min;
	
	x_start = center.x + radius;
	y_start = center.y;
	
	x = x_start;
	y = y_start;
	
	do
	{
		image_set_point(img, color, image_make_point(x, y));
		//...TODO...
		
	}while(x != x_start || y != y_start);
	
	
	return EXIT_SUCCESS;
}









