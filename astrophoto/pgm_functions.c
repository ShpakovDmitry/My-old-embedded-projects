#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <fcntl.h>


#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define PRINT 1
#define DO_NOT_PRINT -1

#define STAT_HEADER "\tChannel Width Height min_px max_px      mean            sigma               sum\n"
#define STAT_STR "%5d  %5d  %5d  %5d  %14.8lf   %14.8lf   %14.0lf\n"



typedef struct channel
{
	int width;
	int height;
	unsigned short int *data;
	unsigned short int min_value;
	unsigned short int max_value;
	double mean_value;
	double sigma;
	int sigma_1_count;
	int sigma_2_count;
	int sigma_3_count;
	double sum;
	int pixel_size;
}channel;

int GetDataFromPGM(char *filename, channel *pgmdata);
int CheckFileHeader(FILE *inputfile);
int GetFileHeaderInfo(FILE *inputfile, int *width, int *height, int *pixel_size);
int GetPixelDataFromFile(FILE *inputfile, unsigned short int *data, int width, int height, int pixel_size);
int GetBayerPixelData(channel *all, channel *red, channel *green, channel *blue);
int GetBayerInterpolatedPixelData(channel *all, channel *red, channel *green, channel *blue);
int GetDataStatistics(channel *channel_str, int print);
int ScaleData(unsigned short int *data, int width, int height, int pixel_size, double scale);
int ShiftData(unsigned short int *data, int width, int height, int pixel_size, double shift_level);
int SubstractData(channel *result, channel *from, channel *what);
int ExportColorChannel(char *filename, unsigned short int *data, int width, int height, int pixel_size);
int PrintStatisticsLine(const char *ch_name, channel ch);
int FormFilenameForExport(char *filename_out, char *filename_in, const char *channel);
int CopyTmpDataToChannelData(double *tmp_data, channel *ch);




int GetDataFromPGM(char *filename, channel *pgmdata)
{
	struct stat file;
	if(stat(filename, &file) == -1)
	{
		printf("Error. Can't get information about file or file %s doesn't exists\n", filename);
		return -1;
	}

	if(!S_ISREG(file.st_mode))
	{
		printf("Error. %s isn't a file\n", filename);
		return -1;
	}

	FILE *inputfile;
	inputfile = fopen(filename, "r");

	if(CheckFileHeader(inputfile) == -1)
	{
		printf("File %s header isn't correct\n", filename);
		fclose(inputfile);
		return -1;
	}

	int width, height, pixel_size;
	if(GetFileHeaderInfo(inputfile, &width, &height, &pixel_size) == -1)
	{
		printf("Can't propoerly extract header information from file\n");
		fclose(inputfile);
		return -1;
	}
	
	pgmdata->pixel_size = pixel_size;
	pgmdata->height = height;
	pgmdata->width = width;
	int tot_pixel_size = width*height*(int)sizeof(unsigned short int);
	pgmdata->data = (unsigned short int *)malloc(tot_pixel_size);
	if(pgmdata->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing pixel data\n", tot_pixel_size);
		fclose(inputfile);
		return -1;
	}

	if(GetPixelDataFromFile(inputfile, pgmdata->data, width, height, pixel_size) == -1)
	{
		printf("Error occured while extracting pixel data\n");
		fclose(inputfile);
		return -1;
	}
	fclose(inputfile);

	return 1;
}

int CheckFileHeader(FILE *inputfile)
{
	char header[3];

	//printf("Reading header...\n");
	header[0] = fgetc(inputfile);
	header[1] = fgetc(inputfile);
	header[2] = fgetc(inputfile);
	
	//printf("Checking header...\n");
	if(header[0] != 'P' && header[1] != '5')
	{
		return -1;
	}

	//printf("Header is correct\n");

	return 1;
}

int GetFileHeaderInfo(FILE *inputfile, int *width, int *height, int *pixel_size)
{
	char height_char[10];
	char width_char[10];
	char size_char[10];
	char c;
	int i = 0;

	//printf("Extracting information from header...\n");
	//printf("Getting image width...\n");
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

	//printf("Getting image height...\n");
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

	//printf("Getting image max pixel value...\n");
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

	return 1;
}

int GetPixelDataFromFile(FILE *inputfile, unsigned short int *data, int width, int height, int pixel_size)
{
	int i,j;

	//printf("Extracting pixel data...\n");
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
	//printf("Pixel data extraction done\n");

	return 1;
}

int GetBayerPixelData(channel *all, channel *red, channel *green, channel *blue)
{
	int i,j;

	red->pixel_size = all->pixel_size;
	green->pixel_size = all->pixel_size;
	blue->pixel_size = all->pixel_size;

	int width  = all->width;
	int height = all->height;

	red->width = width/2;
	red->height = height/2;
	int red_pixel_size = (red->width)*(red->height)*sizeof(unsigned short int);
	red->data = (unsigned short int *)malloc(red_pixel_size);
	if(red->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing red pixel data\n", red_pixel_size);
		return -1;
	}
	for(i = 0; i < height; i += 2)
	{
		for(j = 0; j < width; j += 2)
		{
			red->data[i/2*red->width + j/2] = all->data[i*width + j];
		}
	}

	green->width = width/2;
	green->height = height;
	int green_pixel_size = (green->width)*(green->height)*sizeof(unsigned short int);
	green->data = (unsigned short int *)malloc(green_pixel_size);
	if(green->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing green pixel data\n", green_pixel_size);
		return -1;
	}
	for(i = 0; i < height; i += 1)
	{
		for(j = 1 - i%2; j < width; j += 2)
		{
			green->data[i*green->width + (j - 1 + i%2)/2] = all->data[i*width + j];
		}
	}

	blue->width = width/2;
	blue->height = height/2;
	int blue_pixel_size = (blue->width)*(blue->height)*sizeof(unsigned short int);
	blue->data = (unsigned short int *)malloc(blue_pixel_size);
	if(blue->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing blue pixel data\n", blue_pixel_size);
		return -1;
	}
	for(i = 1; i < height; i += 2)
	{
		for(j = 1; j < width; j += 2)
		{
			blue->data[(i - 1)/2*blue->width + (j - 1)/2] = all->data[i*width + j];
		}
	}

	
	return 1;
}

int GetBayerInterpolatedPixelData(channel *all, channel *red, channel *green, channel *blue)
{
	int i,j;
	double *tmp_data;
	int width = all->width;
	int height = all->height;
	int pixel_size = all->pixel_size;
	red->width = green->width = blue->width = width;
	red->height = green->height = blue->height = height;
	red->pixel_size = green->pixel_size = blue->pixel_size = pixel_size;

	printf("Assuming Bayer filter is RGGB\n");
	
	int tmp_data_size = width*height*sizeof(double);
	tmp_data = (double *)malloc(tmp_data_size);
	if(tmp_data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing temporary pixel data\n", tmp_data_size);
		return -1;
	}

	// red	
	int red_pixel_size = width*height*sizeof(unsigned short int);
	red->data = (unsigned short int *)malloc(red_pixel_size);
	if(red->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing red pixel data\n", red_pixel_size);
		return -1;
	}
	for(i = 0; i < height; i += 2)
	{
		for(j = 0; j < width; j += 2)
		{
			tmp_data[i*width + j] = (double)all->data[i*width + j];
		}
	}
	for(i = 1; i < height - 2; i+=2)
	{
		for(j = 1; j < width - 2; j+=2)
		{
			tmp_data[(i+0)*width + (j+0)] = (tmp_data[(i-1)*width + (j-1)] + tmp_data[(i-1)*width + (j+1)] + tmp_data[(i+1)*width + (j-1)] + tmp_data[(i+1)*width + (j+1)])/4.0;
			tmp_data[(i+0)*width + (j-1)] = (tmp_data[(i-1)*width + (j-1)] + tmp_data[(i+1)*width + (j-1)])/2.0;
			tmp_data[(i+0)*width + (j+1)] = (tmp_data[(i-1)*width + (j+1)] + tmp_data[(i+1)*width + (j+1)])/2.0;
			tmp_data[(i-1)*width + (j+0)] = (tmp_data[(i-1)*width + (j-1)] + tmp_data[(i-1)*width + (j+1)])/2.0;
			tmp_data[(i+1)*width + (j+0)] = (tmp_data[(i+1)*width + (j-1)] + tmp_data[(i+1)*width + (j+1)])/2.0;
		}
	}
	j = width - 1;
	for(i = 0; i < height - 1; i++)
	{
		tmp_data[i*width + (j-0)] = 2*tmp_data[i*width + (j-1)] - tmp_data[i*width + (j-2)];
	}
	i = height - 1;
	for(j = 0; j < width; j++)
	{
		tmp_data[(i-0)*width + j] = 2*tmp_data[(i-1)*width + j] - tmp_data[(i-2)*width + j];
	}
	CopyTmpDataToChannelData(tmp_data, red);
	
	
	// green
	int green_pixel_size = width*height*sizeof(unsigned short int);
	green->data = (unsigned short int *)malloc(green_pixel_size);
	if(green->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing green pixel data\n", green_pixel_size);
		return -1;
	}
	for(i = 0; i < height; i += 1)
	{
		for(j = 1 - i%2; j < width; j += 2)
		{
			tmp_data[i*width + j] = (double)all->data[i*width + j];
		}
	}

	for(i = 1; i < height - 2; i+=1)
	{
		for(j = 2 - i%2; j < (width-1-i%2); j+=2)
		{
			tmp_data[(i+0)*width + (j+0)] = (tmp_data[(i-0)*width + (j-1)] + tmp_data[(i-1)*width + (j-0)] + tmp_data[(i-0)*width + (j+1)] + tmp_data[(i+1)*width + (j-0)])/4.0;
		}
	}
	// borders without corners;
	// top border
	i = 0;
	for(j = 2; j < width - 1; j++)
	{
		tmp_data[(i-0)*width + j] = (tmp_data[(i-0)*width + j-1] + tmp_data[(i-0)*width + j+1])/2;
	}
	// bottom border
	i = height - 1;
	for(j = 1; j < width - 2; j++)
	{
		tmp_data[(i-0)*width + j] = (tmp_data[(i-0)*width + j-1] + tmp_data[(i-0)*width + j+1])/2;
	}
	// left border
	j = 0;
	for(i = 2; i < height - 1; i++)
	{
		tmp_data[(i-0)*width + j] = (tmp_data[(i-1)*width + j] + tmp_data[(i+1)*width + j])/2;
	}
	// right border
	j = width - 1;
	for(i = 1; i < height - 2; i++)
	{
		tmp_data[(i-0)*width + j] = (tmp_data[(i-1)*width + j] + tmp_data[(i+1)*width + j])/2;
	}
	// corners
	// top left
	i = 0;
	j = 0;
	tmp_data[(i-0)*width + j] = (tmp_data[(i+1)*width + j] + tmp_data[(i+0)*width + (j+1)])/2;
	// bottom right
	i = height - 1;
	j = width - 1;
	tmp_data[(i-0)*width + j] = (tmp_data[(i-1)*width + j] + tmp_data[(i+0)*width + (j-1)])/2;
	CopyTmpDataToChannelData(tmp_data, green);


	int blue_pixel_size = width*height*sizeof(unsigned short int);
	blue->data = (unsigned short int *)malloc(blue_pixel_size);
	if(blue->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing blue pixel data\n", blue_pixel_size);
		return -1;
	}
	for(i = 1; i < height; i += 2)
	{
		for(j = 1; j < width; j += 2)
		{
			tmp_data[i*width + j] = (double)all->data[i*width + j];
		}
	}
	for(i = 2; i < height - 1; i+=2)
	{
		for(j = 2; j < width - 1; j+=2)
		{
			tmp_data[(i+0)*width + (j+0)] = (tmp_data[(i-1)*width + (j-1)] + tmp_data[(i-1)*width + (j+1)] + tmp_data[(i+1)*width + (j-1)] + tmp_data[(i+1)*width + (j+1)])/4.0;
			tmp_data[(i+0)*width + (j-1)] = (tmp_data[(i-1)*width + (j-1)] + tmp_data[(i+1)*width + (j-1)])/2.0;
			tmp_data[(i+0)*width + (j+1)] = (tmp_data[(i-1)*width + (j+1)] + tmp_data[(i+1)*width + (j+1)])/2.0;
			tmp_data[(i-1)*width + (j+0)] = (tmp_data[(i-1)*width + (j-1)] + tmp_data[(i-1)*width + (j+1)])/2.0;
			tmp_data[(i+1)*width + (j+0)] = (tmp_data[(i+1)*width + (j-1)] + tmp_data[(i+1)*width + (j+1)])/2.0;
		}
	}
	j = 0;
	for(i = 1; i < height; i++)
	{
		tmp_data[i*width + (j-0)] = 2*tmp_data[i*width + (j+1)] - tmp_data[i*width + (j+2)];
	}
	i = 0;
	for(j = 0; j < width; j++)
	{
		tmp_data[(i-0)*width + j] = 2*tmp_data[(i+1)*width + j] - tmp_data[(i+2)*width + j];
	}
	CopyTmpDataToChannelData(tmp_data, blue);

	free(tmp_data);

	return 1;
}

int GetDataStatistics(channel *channel_str, int print)
{
	int i,j;
	double sum1 = 0;
	double sum2 = 0;
	double temp;
	unsigned short int min_value = 0xffff;
	unsigned short int max_value = 0x0000;
	int sigma_1_count = 0;
	int sigma_2_count = 0;
	int sigma_3_count = 0;
	int width = channel_str->width;
	int height = channel_str->height;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			sum1 += channel_str->data[i*width + j];
			if(channel_str->data[i*width + j] > max_value)
			{
				max_value = channel_str->data[i*width + j];
			}
			if(channel_str->data[i*width + j] < min_value)
			{
				min_value = channel_str->data[i*width + j];
			}
		}
	}
	double mean_value = (double)sum1/(double)width/(double)height;

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			temp =  (double)channel_str->data[i*width + j] - mean_value;
			sum2 += temp*temp;
		}
	}
	double sigma = sqrt(sum2/(double)width/(double)height);

	for(i = 0; i < height; i++)
	{
		for(j = 0; j < width; j++)
		{
			if( ((double)channel_str->data[i*width + j]) >= (mean_value - sigma) && ((double)channel_str->data[i*width + j]) < (mean_value + sigma) )
			{
				sigma_1_count++;
			}
			if( ((double)channel_str->data[i*width + j]) >= (mean_value - sigma*2.0) && ((double)channel_str->data[i*width + j]) < (mean_value + sigma*2.0) )
			{
				sigma_2_count++;
			}
			if( ((double)channel_str->data[i*width + j]) >= (mean_value - sigma*3.0) && ((double)channel_str->data[i*width + j]) < (mean_value + sigma*3.0) )
			{
				sigma_3_count++;
			}
		}
	}

	channel_str->min_value = min_value;
	channel_str->max_value = max_value;
	channel_str->mean_value = mean_value;
	channel_str->sigma = sigma;
	channel_str->sum = sum1;

	if(print > 0)
	{
		printf("\tData width = %d height = %d total = %d\n", width, height, width*height);
		printf("\tMinimal value = %d\n", min_value);
		printf("\tMaximal value = %d\n", max_value);
		printf("\tMean value = %.5lf\n", mean_value);
		printf("\tSigma      = %.5lf\n", sigma);
		printf("\tValues in 1 sigma interval %d = %.3lf%\n", sigma_1_count, (double)sigma_1_count*100.0/(double)width/(double)height);
		printf("\tValues in 2 sigma interval %d = %.3lf%\n", sigma_2_count, (double)sigma_2_count*100.0/(double)width/(double)height);
		printf("\tValues in 3 sigma interval %d = %.3lf%\n", sigma_3_count, (double)sigma_3_count*100.0/(double)width/(double)height);
		printf("\tSum of all pixels = %.0lf\n", sum1);
		//printf("\tSum of (x_i - x_mean)^2 = %.5lf\n", sum2);
	}

	return 1;

}

int ScaleData(unsigned short int *data, int width, int height, int pixel_size, double scale)
{
	int pixel_max_val;
	int i,j;
	char ovf_flag = 0;
	if(pixel_size == 2)
	{
		double tmp;
		pixel_max_val = 65535;
		if(scale > 65535.0)
		{
			printf("Adjusting scale factor to 65535.0");
			scale = 65535.0;
		}

		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				tmp = ((double)data[i*width+j]*scale);
				if(tmp > 65535.0)
				{
					ovf_flag = 1;
					tmp = 65535.0;
				}
				data[i*width+j] = tmp;
			}
		}
	}
	else
	{
		double tmp;
		pixel_max_val = 255;
		if(scale > 255.0)
		{
			printf("Adjusting scale factor to 255.0");
			scale = 255.0;
		}

		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				tmp = ((double)data[i*width+j]*scale);
				if(tmp > 255.0)
				{
					ovf_flag = 1;
					tmp = 255.0;
				}
				data[i*width+j] = tmp;
			}
		}

	}

	if(ovf_flag == 1)
	{
		printf("Warning. While scaling, pixel overflow occured.\n");
	}

	return 1;
}

int ShiftData(unsigned short int *data, int width, int height, int pixel_size, double shift_level)
{
	int pixel_max_val;
	int i,j;
	char ovf_flag = 0;
	char undf_flag = 0;
	if(pixel_size == 2)
	{
		double tmp;
		pixel_max_val = 65535;
		if(shift_level > 65535.0)
		{
			printf("Adjusting shift factor to 65535.0");
			shift_level = 65535.0;
		}
		if(shift_level < -65535.0)
		{
			printf("Adjusting shift factor to -65535.0");
			shift_level = -65535.0;
		}

		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				tmp = ((double)data[i*width+j] + shift_level);
				if(tmp > 65535.0)
				{
					ovf_flag = 1;
					tmp = 65535.0;
				}
				if(tmp < 0.0)
				{
					undf_flag = 1;
					tmp = 0.0;
				}
				data[i*width+j] = tmp;
			}
		}
	}
	else
	{
		double tmp;
		pixel_max_val = 255;
		if(shift_level > 255.0)
		{
			printf("Adjusting shift factor to 255.0");
			shift_level = 255.0;
		}
		if(shift_level < -255.0)
		{
			printf("Adjusting shift factor to -255.0");
			shift_level = -255.0;
		}

		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				tmp = ((double)data[i*width+j] + shift_level);
				if(tmp > 255.0)
				{
					ovf_flag = 1;
					tmp = 255.0;
				}
				if(tmp < 0.0)
				{
					undf_flag = 1;
					tmp = 0.0;
				}
				data[i*width+j] = tmp;
			}
		}
	}
	
	if(ovf_flag == 1)
	{
		printf("Warning. While scaling, pixel overflow occured.\n");
	}
	if(undf_flag == 1)
	{
		printf("Warning. While shifting level, pixel underflow occured.\n");
	}

	return 1;
}

int SubstractData(channel *result, channel *from, channel *what)
{
	if(from->width != what->width || from->height != what->height)
	{
		printf("Error. Sizes of images are not equal.\n");
		return -1;
	}
	if(from->pixel_size != what->pixel_size)
	{
		printf("Error. Images data sizes are not equal.\n");
		return -1;
	}

	result->pixel_size = from->pixel_size;
	result->width = from->width;
	result->height = from->height;
	int tot_pixel_size = result->width*result->height*(int)sizeof(unsigned short int);
	result->data = (unsigned short int *)malloc(tot_pixel_size);
	if(result->data == NULL)
	{
		printf("Can't allocate %d bytes of memory for storing pixel data\n", tot_pixel_size);
		return -1;
	}

	int pixel_max_val;
	int i,j;
	char undf_flag = 0;
	int pixel_size = result->pixel_size;
	int width = result->width;
	int height = result->height;
	if(pixel_size == 2)
	{
		double tmp;
		pixel_max_val = 65535;
		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				tmp = ((double)from->data[i*width+j] - (double)what->data[i*width+j]);
				if(tmp < 0.0)
				{
					undf_flag = 1;
					tmp = 0.0;
				}
				result->data[i*width+j] = tmp;
			}
		}
	}
	else
	{
		double tmp;
		pixel_max_val = 255;
		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				tmp = ((double)from->data[i*width+j]  - (double)what->data[i*width+j]);
				if(tmp < 0.0)
				{
					undf_flag = 1;
					tmp = 0.0;
				}
				result->data[i*width+j] = tmp;
			}
		}
	}
	
	if(undf_flag == 1)
	{
		printf("Warning. While shifting level, pixel underflow occured.\n");
	}

	return 1;
}

int ExportColorChannel(char *filename, unsigned short int *data, int width, int height, int pixel_size)
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
				tmp = ((double)data[i*width+j]);
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
				tmp = ((double)data[i*width+j]);
				fputc( ( (unsigned short int)tmp & 0x00ff) >> 0, outputfile);
			}
		}

	}

	fclose(outputfile);
	return 1;
}


int PrintStatisticsLine(const char *ch_name, channel ch)
{
	printf("\t%s\t"STAT_STR, ch_name, ch.width, ch.height, ch.min_value, ch.max_value, ch.mean_value, ch.sigma, ch.sum);
	
	return 1;
}


int FormFilenameForExport(char *filename_out, char *filename_in, const char *channel)
{
	strncpy(filename_out, filename_in, strlen(filename_in) - 4);
	strcat(filename_out, "_");
	strcat(filename_out, channel);
	strcat(filename_out, ".pgm");

	return 1;
}

int CopyTmpDataToChannelData(double *tmp_data, channel *ch)
{
	int row, col;
	
	for(row = 0; row < ch->height; row++)
	{
		for(col = 0; col < ch->width; col++)
		{
			if(tmp_data[row*ch->width + col] < 0.0)
			{
				ch->data[row*ch->width + col] = (unsigned short int)0;
			}
			else
			{
				ch->data[row*ch->width + col] = (unsigned short int)tmp_data[row*ch->width + col]; 
			}
		}
	}

	return 1;
}












