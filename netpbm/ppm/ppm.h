#define WRITE_OK	0
#define WRITE_ERROR	1

#define READ_OK		0
#define READ_ERROR	1

#define INIT_OK		0
#define INIT_ERROR	1

#define FALSE		0
#define TRUE		1

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

typedef struct
{
	size_t width;
	size_t height;
	color_RGB *data;
}image_ppm;

typedef struct
{
	int x;
	int y;
}image_point;

image_point image_make_point(int x, int y);
int image_write(char *filename, image_ppm *img);
int image_read(char *filename, image_ppm *img);
int image_init(size_t width, size_t height, image_ppm *img);
int image_destroy(image_ppm *img);
int image_point_belongs_to(image_ppm *img, image_point point);
int image_set_point(image_ppm *img, color_RGB color, image_point point);
int image_set_point_additive(image_ppm *img, color_RGB color, image_point point);
int image_get_point(image_ppm *img, color_RGB *color, image_point point);
int image_clear(image_ppm *img);
int image_equal_size(image_ppm *img1, image_ppm *img2);
int image_equal(image_ppm *img1, image_ppm *img2);
int image_copy(image_ppm *img_to, image_ppm *img_from);
int image_add_image(image_ppm *img1, image_ppm *img2);
int image_draw_v_line(image_ppm *img, color_RGB color, int y1, int y2, int x1);
int image_draw_h_line(image_ppm *img, color_RGB color, int x1, int x2, int y1);
int image_draw_line(image_ppm *img, color_RGB color, image_point p1, image_point p2);
int image_draw_circle(image_ppm *img, color_RGB color, image_point center, int radius);





