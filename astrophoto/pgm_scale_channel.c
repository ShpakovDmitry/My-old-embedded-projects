// program for scaling .CR2 --> .pgm channel
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
	char outputfilename[FILENAME_MAX];	// .pgm filename
	double scale_factor;			// all pixel data will be scaled by this factor
}inputparameters;


int CheckInputParameters(int argc, char **argv, inputparameters *input_args); // returns 0 if OK,else -1
int Usage(int argc, char **argv);

int main(int argc, char **argv)
{
	inputparameters input_args;
	channel all_px;

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
		
	ScaleData(all_px.data, all_px.width, all_px.height, all_px.pixel_size, input_args.scale_factor);
	if(ExportColorChannel(input_args.outputfilename, all_px.data, all_px.width, all_px.height, all_px.pixel_size) == -1)
	{
		printf("Can't export all pixel data\n");
		return -1;
	}
	
	return 0;
}


int CheckInputParameters(int argc, char **argv, inputparameters *input_args)
{
	if(argc == 4)
	{
		strcpy(input_args->inputfilename,argv[1]);
		strcpy(input_args->outputfilename,argv[2]);
		input_args->scale_factor = atof(argv[3]);
		if(input_args->scale_factor <= 0.0)
		{
			return -1;
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
	printf("\t%s <inputfilename> <outputfilename> [scale factor]\n", argv[0]);

	return 0;
}



