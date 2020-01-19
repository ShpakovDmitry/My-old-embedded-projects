// program for exporting channels
//
//

#include <stdio.h>
#include <string.h>

#include "pgm_functions.c"


#define use 1
#define not_use 0

typedef struct inputparameters
{
	char inputfilename[FILENAME_MAX];	// .pgm filename
	char use_bilinear_interpolation;	// pixel data will be interpolated
}inputparameters;


int CheckInputParameters(int argc, char **argv, inputparameters *input_args); // returns 0 if OK,else -1
int Usage(int argc, char **argv);

int main(int argc, char **argv)
{
	inputparameters input_args;
	channel all_px, red_px, green_px, blue_px;

	if( CheckInputParameters(argc, argv, &input_args) == -1 )
	{
		printf("Error. Check parameters\n");
		Usage(argc, argv);
		return -1;
	}

	if(GetDataFromPGM(input_args.inputfilename, &all_px) == -1)
	{
		printf("Error. Getting data from .pgm file\n");
		return -1;
	}
	
	if(input_args.use_bilinear_interpolation == not_use)
	{
		if(GetBayerPixelData(&all_px, &red_px, &green_px, &blue_px) == -1)
		{
			printf("Error occured while extracting Bayer pixel data\n");
			return -1;
		}
	}
	else
	{
		if(GetBayerInterpolatedPixelData(&all_px, &red_px, &green_px, &blue_px) == -1)
		{
			printf("Error occured while extracting interpolated Bayer pixel data\n");
			return -1;
		}
	}

	char filename_red[FILENAME_MAX];
	char filename_green[FILENAME_MAX];
	char filename_blue[FILENAME_MAX];
	FormFilenameForExport(filename_red, input_args.inputfilename, "red");
	if(ExportColorChannel(filename_red, red_px.data, red_px.width, red_px.height, red_px.pixel_size) == -1)
	{
		printf("Can't export red channel data\n");
		return 0;
	}
	
	FormFilenameForExport(filename_green, input_args.inputfilename, "green");
	if(ExportColorChannel(filename_green, green_px.data, green_px.width, green_px.height, green_px.pixel_size) == -1)
	{
		printf("Can't export green channel data\n");
		return 0;
	}

	FormFilenameForExport(filename_blue, input_args.inputfilename, "blue");
	if(ExportColorChannel(filename_blue, blue_px.data, blue_px.width, blue_px.height, blue_px.pixel_size) == -1)
	{
		printf("Can't export blue channel data\n");
		return 0;
	}

	return 0;
}


int CheckInputParameters(int argc, char **argv, inputparameters *input_args)
{
	if(argc == 3)
	{
		strcpy(input_args->inputfilename,argv[1]);
		if(strcmp(argv[2], "--bilinear") == 0 || strcmp(argv[2], "-b") == 0)
		{
			input_args->use_bilinear_interpolation = use;
		}
		else
		{
			input_args->use_bilinear_interpolation = not_use;
		}

	}
	else
	{
		return -1;
	}

	return 0;
}


int Usage(int argc, char **argv)
{
	printf("Usage:\n");
	printf("\t%s <inputfilename> [parameter]\n", argv[0]);
	printf("\t\t-b --bilinear\tif specified red, green and blue chanells will be interpolated to original image size\n\n");

	return 0;
}



