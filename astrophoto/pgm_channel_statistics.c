// program for analyze .CR2 --> .pgm channel statistics
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
	char use_bayer_filter;			// 1 - use , 0 - not use
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
	
	GetDataStatistics(&all_px, DO_NOT_PRINT);
	printf("All pixel statistics:\n");
	printf(STAT_HEADER);
	PrintStatisticsLine("All", all_px);
	if(input_args.use_bayer_filter == use)
	{
		channel red_px, green_px, blue_px;
		if(GetBayerPixelData(&all_px, &red_px, &green_px, &blue_px) == -1)
		{
			printf("Error occured while extracting Bayer pixel data\n");
			return -1;
		}
		
		GetDataStatistics(&red_px,   DO_NOT_PRINT);
		GetDataStatistics(&green_px, DO_NOT_PRINT);
		GetDataStatistics(&blue_px,  DO_NOT_PRINT);

		PrintStatisticsLine("Red",   red_px);
		PrintStatisticsLine("Green", green_px);
		PrintStatisticsLine("Blue",  blue_px);
	}

	return 0;
}


int CheckInputParameters(int argc, char **argv, inputparameters *input_args)
{
	if(argc == 2)
	{
		strcpy(input_args->inputfilename,argv[1]);
		input_args->use_bayer_filter = not_use;
	}
	else if(argc == 3)
	{
		strcpy(input_args->inputfilename,argv[1]);
		if(strcmp(argv[2], "--bayer") == 0 || strcmp(argv[2], "-b") == 0)
		{
			input_args->use_bayer_filter = use;
		}
		else
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
	printf("\t%s <filename> [parameters]\n", argv[0]);
	printf("\t\t-b\tif specified red, green and blue chanell statistics will be calculated\n\n");

	return 0;
}











