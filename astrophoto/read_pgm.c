#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// flags for program input parameter behaviour
#define EXPORT 0


typedef struct channel
{
	int width;
	int height;
	unsigned short int *data;
}channel;

int CheckInputArguments(int argc, char **argv, char *flags);
int CheckFileHeader(FILE *inputfile);
int GetFileHeaderInfo(FILE *inputfile, int *width, int *height, int *pixel_size);
int GetPixelDataFromFile(FILE *inputfile, unsigned short int *data, int width, int height, int pixel_size);
int GetBayerPixelData(unsigned short int *data, int width, int height, channel *red, channel *green, channel *blue);
int GetDataStatistics(unsigned short int *data, int width, int height, unsigned short int *min_value,\
							unsigned short int *max_value,\
							double *mean_value, double *sigma, int print);
int ExportColorChannel(char *filename, unsigned short int *data, int width, int height, int pixel_size, double scale);






int main(int argc, char **argv)
{
	char flags = 0;
	if(CheckInputArguments(argc, argv, &flags) == -1)
	{
		printf("Exiting program\n");
		return 0;
	}
	
	if( flags & (1 << EXPORT) )
	{
		printf("Need to export channels\n");
	}

	FILE *inputfile;
	inputfile = fopen(argv[1], "r");

	if(CheckFileHeader(inputfile) == -1)
	{
		printf("File %s header isn't correct\n", argv[1]);
		fclose(inputfile);
		return 0;
	}

	int width, height, pixel_size;
	if(GetFileHeaderInfo(inputfile, &width, &height, &pixel_size) == -1)
	{
		printf("Can't propoerly extract header information from file\n");
		fclose(inputfile);
		return 0;
	}

	printf("Trying to allocate memory for storing pixel data...\n");
	int tot_pixel_size = width*height*(int)sizeof(unsigned short int);
	unsigned short int *data = (unsigned short int *)malloc(tot_pixel_size);
	if(data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing pixel data\n", tot_pixel_size);
		fclose(inputfile);
		return 0;
	}
	printf("Allocated %d bytes = %.4f KiB = %.4f MiB of memory\n",\
						tot_pixel_size,\
						(float)tot_pixel_size/1024.0,\
						(float)tot_pixel_size/1024.0/1024.0);

	printf("Starting to extracting pixel data...\n");
	if(GetPixelDataFromFile(inputfile, data, width, height, pixel_size) == -1)
	{
		printf("Error occured while extracting pixel data\n");
		fclose(inputfile);
		return 0;
	}
	fclose(inputfile);
	
	channel red,green,blue;
	if(GetBayerPixelData(data, width, height, &red, &green, &blue) == -1)
	{
		printf("Error occured while extracting Bayer pixel data\n");
		return 0;
	}

	if(flags & (1 << EXPORT))
	{
		char filename_all[FILENAME_MAX];
		char filename_red[FILENAME_MAX];
		char filename_green[FILENAME_MAX];
		char filename_blue[FILENAME_MAX];
		double scale_all = 1.0;
		double scale_red = 1.0;
		double scale_green = 1.0;
		double scale_blue = 1.0;
		int i;

		for(i = 0; i < argc; i++)
		{
			if(strcmp(argv[i], "--export") == 0 || strcmp(argv[i], "-e") == 0)
			{
				scale_all   = atof(argv[i + 1]);
				scale_red   = atof(argv[i + 2]);
				scale_green = atof(argv[i + 3]);
				scale_blue  = atof(argv[i + 4]);
				i = argc;
			}
		}

		if(pixel_size == 2)
		{
			if(scale_all > 65535.0)
			{
				printf("All pixel scale is %lf, so adjusting all pixel scale to 65535\n", scale_all);
				scale_all = 65535.0;
			}
			if(scale_red > 65535.0)
			{
				printf("Red pixel scale is %lf, so adjusting red pixel scale to 65535\n", scale_red);
				scale_red = 65535.0;
			}
			if(scale_green > 65535.0)
			{
				printf("Green pixel scale is %lf, so adjusting green pixel scale to 65535\n", scale_green);
				scale_green = 65535.0;
			}
			if(scale_blue > 65535.0)
			{
				printf("Blue pixel scale is %lf, so adjusting blue pixel scale to 65535\n", scale_blue);
				scale_blue = 65535.0;
			}
		}
		else
		{
			if(scale_all > 255.0)
			{
				printf("All pixel scale is %lf, so adjusting all pixel scale to 255\n", scale_all);
				scale_all = 255.0;
			}
			if(scale_red > 255.0)
			{
				printf("Red pixel scale is %lf, so adjusting red pixel scale to 255\n", scale_red);
				scale_red = 255.0;
			}
			if(scale_green > 255.0)
			{
				printf("Green pixel scale is %lf, so adjusting green pixel scale to 255\n", scale_green);
				scale_green = 255.0;
			}
			if(scale_blue > 255.0)
			{
				printf("Blue pixel scale is %lf, so adjusting blue pixel scale to 255\n", scale_blue);
				scale_blue = 255.0;
			}

		}

		printf("Starting to export all pixel data\n");
		strncpy(filename_all, argv[1], strlen(argv[1]) - 4);
		strcat(filename_all, "_all.pgm");
		if(ExportColorChannel(filename_all, data, width, height, pixel_size, scale_all) == -1)
		{
			printf("Can't export all pixel data\n");
			return 0;
		}
		printf("All pixel data exported\n");
	
		printf("Starting to export red channel data\n");
		strncpy(filename_red, argv[1], strlen(argv[1]) - 4);
		strcat(filename_red, "_red.pgm");
		if(ExportColorChannel(filename_red, red.data, red.width, red.height, pixel_size, scale_red) == -1)
		{
			printf("Can't export red channel data\n");
			return 0;
		}
		printf("Red channel data exported\n");
		
		printf("Starting to export green channel data\n");
		strncpy(filename_green, argv[1], strlen(argv[1]) - 4);
		strcat(filename_green, "_green.pgm");
		if(ExportColorChannel(filename_green, green.data, green.width, green.height, pixel_size, scale_green) == -1)
		{
			printf("Can't export green channel data\n");
			return 0;
		}
		printf("Green channel data exported\n");

		printf("Starting to export blue channel data\n");
		strncpy(filename_blue, argv[1], strlen(argv[1]) - 4);
		strcat(filename_blue, "_blue.pgm");
		if(ExportColorChannel(filename_blue, blue.data, blue.width, blue.height, pixel_size, scale_blue) == -1)
		{
			printf("Can't export blue channel data\n");
			return 0;
		}
		printf("Blue channel data exported\n");
	}

	printf("Calculating pixel data statistics...\n");
	unsigned short int min_value_all, max_value_all;
	unsigned short int min_value_red, max_value_red;
	unsigned short int min_value_green, max_value_green;
	unsigned short int min_value_blue, max_value_blue;
	double mean_value_all, sigma_all;
	double mean_value_red, sigma_red;
	double mean_value_green, sigma_green;
	double mean_value_blue, sigma_blue;
	GetDataStatistics(     data,        width,       height,   &min_value_all,   &max_value_all,   &mean_value_all,   &sigma_all, -1);
	GetDataStatistics(  red.data,   red.width,   red.height,   &min_value_red,   &max_value_red,   &mean_value_red,   &sigma_red, -1);
	GetDataStatistics(green.data, green.width, green.height, &min_value_green, &max_value_green, &mean_value_green, &sigma_green, -1);
	GetDataStatistics( blue.data,  blue.width,  blue.height,  &min_value_blue,  &max_value_blue,  &mean_value_blue,  &sigma_blue, -1);
	printf("All pixel statistics:\n");
	printf("\tChannel Width Height min_px_val max_px_val mean_val    sigma\n");
	printf("\tAll     %05.0d %05.0d   %05.0d      %05.0d     %5.4lf   %5.4lf\n", width, height, min_value_all, max_value_all, mean_value_all, sigma_all);
	printf("\tRed     %05.0d %05.0d   %05.0d      %05.0d     %5.4lf   %5.4lf\n", red.width, red.height, min_value_red, max_value_red, mean_value_red, sigma_red);
	printf("\tGreen   %05.0d %05.0d   %05.0d      %05.0d     %5.4lf   %5.4lf\n", green.width, green.height, min_value_green, max_value_green, mean_value_green, sigma_green);
	printf("\tBlue    %05.0d %05.0d   %05.0d      %05.0d     %5.4lf   %5.4lf\n", blue.width, blue.height, min_value_blue, max_value_blue, mean_value_blue, sigma_blue);



	return 0;
}



int CheckInputArguments(int argc, char **argv, char *flags)
{
	printf("Checking program arguments...\n");
	if(argc != 2 && argc != 7)
	{
		printf("Error. Usage: %s <filename> [parameter]\n", argv[0]);
		printf("Parameters:\n");
		printf("\t --export -e [A R G B]\n");
		printf("\t\texport all pixels,red,green and blue channels separatelly\n");
		printf("\t\twith name filename_[channel].pgm and pixel scale [A R G B]\n");
		return -1;
	}
	printf("Program arguments OK\n");


	struct stat file;
	printf("Checking file %s existance...\n", argv[1]);
	if(stat(argv[1], &file) == -1)
	{
		printf("Error. Can't get information about file or file %s doesn't exists\n", argv[1]);
		return -1;
	}

	if(S_ISREG(file.st_mode))
	{
		printf("File %s exists\n", argv[1]);
	}
	else
	{
		printf("Error. %s isn't a file\n", argv[1]);
		return -1;
	}

	int i;
	for(i = 0; i < argc && !(*flags & (1 << EXPORT)); i++)
	{
		if(strcmp(argv[i], "--export") == 0 || strcmp(argv[i], "-e") == 0)
		{
			*flags |= (1 << EXPORT);
		}
	}
	if(*flags & (1 << EXPORT))
	{
		if((argc - i) < 3)
		{
			printf("Error. Specify four channel scale to export\n");
			return -1;
		}

		int k;
		for(k = 0; k < 4; k++)
		{
			double tmp;
			tmp = atof(argv[i+k]);
			if(tmp < 0.0)
			{
				printf("Error. Channel scale should be positive number\n");
				return -1;
			}
		}
	}

	return 1;
}

int CheckFileHeader(FILE *inputfile)
{
	char header[3];

	printf("Reading header...\n");
	header[0] = fgetc(inputfile);
	header[1] = fgetc(inputfile);
	header[2] = fgetc(inputfile);
	
	printf("Checking header...\n");
	if(header[0] != 'P' && header[1] != '5')
	{
		return -1;
	}

	printf("Header is correct\n");

	return 1;
}

int GetFileHeaderInfo(FILE *inputfile, int *width, int *height, int *pixel_size)
{
	char height_char[10];
	char width_char[10];
	char size_char[10];
	char c;
	int i = 0;

	printf("Extracting information from header...\n");
	printf("Getting image width...\n");
	do
	{
		c = fgetc(inputfile);
		width_char[i] = c;
	}while(i++ < 10 && c != 0x20 );
	width_char[i - 1] = '\0';
	if(i == 10 || c != 0x20)
	{
		printf("Error extracting width\n");
		return -1;
	}

	printf("Getting image height...\n");
	i = 0;
	do
	{
		c = fgetc(inputfile);
		height_char[i] = c;
	}while(i++ < 10 && c != 0x0a);
	height_char[i - 1] = '\0';
	if(i == 10 || c != 0x0a)
	{
		printf("Error extracting height\n");
		return -1;
	}

	printf("Getting image max pixel value...\n");
	i = 0;
	do
	{
		c = fgetc(inputfile);
		size_char[i] = c;
	}while(i++ < 10 && c != 0x0a);
	size_char[i - 1] = '\0';
	if(i == 10 || c != 0x0a)
	{
		printf("Error extracting max pixel value\n");
		return -1;
	}


	*width = atoi(width_char);
	*height = atoi(height_char);
	if(atoi(size_char) > 256)
	{
		*pixel_size = 2;
	}
	else
	{
		*pixel_size = 1;
	}

	printf("Extraction done\n");
	printf("Width  = %d\n", *width);
	printf("Height = %d\n", *height);
	printf("Pixel size = %d byte%c\n", *pixel_size, (*pixel_size == 1) ? ' ' : 's');
	printf("Total size = %d pixels = %1.3f Mpixels\n", (*width)*(*height), (float)(*width)*(float)(*height)/1000000.0);
	
	return 1;

}

int GetPixelDataFromFile(FILE *inputfile, unsigned short int *data, int width, int height, int pixel_size)
{
	int i,j;

	printf("Extracting pixel data...\n");
	if(pixel_size == 1)
	{		
		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				data[i*width + j] = fgetc(inputfile);
			}
		}
	}
	else
	{
		int temp_low, temp_high;
		unsigned short int temp;
		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				temp_high = fgetc(inputfile);
				temp_low = fgetc(inputfile);
				temp_high = temp_high & 0xff;
				temp_low = temp_low & 0xff;
				temp = (temp_high << 8) | (temp_low << 0);
				data[i*width + j] = temp;
			}
		}
	}
	printf("Pixel data extraction done\n");

	return 1;
}

int GetBayerPixelData(unsigned short int *data, int width, int height, channel *red, channel *green, channel *blue)
{
	int i,j;

	printf("Extracting Bayer filter color data\n");
	printf("Assuming Bayer filter is RGGB\n");
	
	printf("Extracting Red channel...\n");
	red->width = width/2;
	red->height = height/2;
	int red_pixel_size = (red->width)*(red->height)*sizeof(unsigned short int);
	printf("Trying to allocate memory for storing red pixel data\n");
	red->data = (unsigned short int *)malloc(red_pixel_size);
	if(red->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing red pixel data\n", red_pixel_size);
		return -1;
	}
	printf("Allocated %d bytes = %.4f KiB = %.4f MiB of memory\n",\
						red_pixel_size,\
						(float)red_pixel_size/1024.0,\
						(float)red_pixel_size/1024.0/1024.0);
	printf("Reading red pixel data...\n");
	for(i = 0; i < height; i += 2)
	{
		for(j = 0; j < width; j += 2)
		{
			red->data[i/2*red->width + j/2] = data[i*width + j];
		}
	}
	printf("Red channel extracted\n");

	printf("Extracting Green channel...\n");
	green->width = width/2;
	green->height = height;
	int green_pixel_size = (green->width)*(green->height)*sizeof(unsigned short int);
	printf("Trying to allocate memory for storing green pixel data\n");
	green->data = (unsigned short int *)malloc(green_pixel_size);
	if(green->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing green pixel data\n", green_pixel_size);
		return -1;
	}
	printf("Allocated %d bytes = %.4f KiB = %.4f MiB of memory\n",\
						green_pixel_size,\
						(float)green_pixel_size/1024.0,\
						(float)green_pixel_size/1024.0/1024.0);
	printf("Reading green pixel data...\n");
	for(i = 0; i < height; i += 1)
	{
		for(j = 1 - i%2; j < width; j += 2)
		{
			green->data[i*green->width + (j - 1 + i%2)/2] = data[i*width + j];
		}
	}
	printf("Green channel extracted\n");

	printf("Extracting Blue channel...\n");
	blue->width = width/2;
	blue->height = height/2;
	int blue_pixel_size = (blue->width)*(blue->height)*sizeof(unsigned short int);
	printf("Trying to allocate memory for storing blue pixel data\n");
	blue->data = (unsigned short int *)malloc(blue_pixel_size);
	if(blue->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing blue pixel data\n", blue_pixel_size);
		return -1;
	}
	printf("Allocated %d bytes = %.4f KiB = %.4f MiB of memory\n",\
						blue_pixel_size,\
						(float)blue_pixel_size/1024.0,\
						(float)blue_pixel_size/1024.0/1024.0);
	printf("Reading blue pixel data...\n");
	for(i = 1; i < height; i += 2)
	{
		for(j = 1; j < width; j += 2)
		{
			blue->data[(i - 1)/2*blue->width + (j - 1)/2] = data[i*width + j];
		}
	}
	printf("Blue channel extracted\n");

	printf("Bayer filter color data extracted\n");
	
	return 0;
}

int GetDataStatistics(unsigned short int *data, int width, int height, unsigned short int *min_value,\
							unsigned short int *max_value,\
							double *mean_value, double *sigma, int print)
{
	int i,j;
	double sum1 = 0;
	double sum2 = 0;
	double temp;
	*min_value = 0xffff;
	*max_value = 0x0000;
	int sigma_1_count = 0;
	int sigma_2_count = 0;
	int sigma_3_count = 0;


	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			sum1 += data[i*width + j];
			if(data[i*width + j] > *max_value)
			{
				*max_value = data[i*width + j];
			}
			if(data[i*width + j] < *min_value)
			{
				*min_value = data[i*width + j];
			}
		}
	}
	*mean_value = (double)sum1/(double)width/(double)height;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			temp =  (double)data[i*width + j] - *mean_value;
			sum2 += temp*temp;
		}
	}
	*sigma = sqrt(sum2/(double)width/(double)height);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			if( ((double)data[i*width + j]) >= (*mean_value - *sigma) && ((double)data[i*width + j]) < (*mean_value + *sigma) )
			{
				sigma_1_count++;
			}
			if( ((double)data[i*width + j]) >= (*mean_value - *sigma*2.0) && ((double)data[i*width + j]) < (*mean_value + *sigma*2.0) )
			{
				sigma_2_count++;
			}
			if( ((double)data[i*width + j]) >= (*mean_value - *sigma*3.0) && ((double)data[i*width + j]) < (*mean_value + *sigma*3.0) )
			{
				sigma_3_count++;
			}
		}
	}

	if(print > 0)
	{
		printf("\tData width = %d height = %d total = %d\n", width, height, width*height);
		printf("\tMinimal value = %d\n", *min_value);
		printf("\tMaximal value = %d\n", *max_value);
		printf("\tMean value = %.5lf\n", *mean_value);
		printf("\tSigma      = %.5lf\n", *sigma);
		printf("\tValues in 1 sigma interval %d = %.3lf%\n", sigma_1_count, (double)sigma_1_count*100.0/(double)width/(double)height);
		printf("\tValues in 2 sigma interval %d = %.3lf%\n", sigma_2_count, (double)sigma_2_count*100.0/(double)width/(double)height);
		printf("\tValues in 3 sigma interval %d = %.3lf%\n", sigma_3_count, (double)sigma_3_count*100.0/(double)width/(double)height);
		printf("\tSum of all pixels = %.0lf\n", sum1);
		printf("\tSum of (x_i - x_mean)^2 = %.5lf\n", sum2);
	}

	return 1;

}

int ExportColorChannel(char *filename, unsigned short int *data, int width, int height, int pixel_size, double scale)
{
	FILE *outputfile = fopen(filename, "w");

	if(outputfile == NULL)
	{
		printf("Can't create file %s\n", filename);
		return -1;
	}
	printf("File %s successfully created\n", filename);
	printf("Writing data now...\n");

	int pixel_max_val;
	if(pixel_size == 2)
	{
		pixel_max_val = 65535;
	}
	else
	{
		pixel_max_val = 255;
	}
	fprintf(outputfile,"P5%c%d%c%d%c%d%c", 0x0a, width, 0x20, height, 0x0a, pixel_max_val, 0x0a);

	int i,j;

	if(pixel_size == 2)
	{
		double tmp;
		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				tmp = ((double)data[i*width+j]*scale);
				if(tmp > 65535.0)
				{
					tmp = 65535.0;
				}
				fputc( ( (unsigned short int)tmp & 0xff00) >> 8, outputfile);
				fputc( ( (unsigned short int)tmp & 0x00ff) >> 0, outputfile);
			}
		}
	}
	else
	{
		double tmp;
		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				tmp = ((double)data[i*width+j]*scale);
				if(tmp > 255.0)
				{
					tmp = 255.0;
				}
				fputc( ( (unsigned short int)tmp & 0x00ff) >> 0, outputfile);
			}
		}

	}

	fclose(outputfile);
	return 1;
}



















