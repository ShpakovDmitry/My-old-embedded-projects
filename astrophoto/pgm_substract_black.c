// program for substracting black level.
//
//

#include <stdio.h>
#include <string.h>

#include "pgm_functions.c"


#define use 1
#define not_use 0

typedef struct inputparameters
{
	char inputfilename_orig[FILENAME_MAX];	// .pgm filename
	char inputfilename_black[FILENAME_MAX];	// .pgm filename
	char outputfilename[FILENAME_MAX];	// .pgm filename
}inputparameters;


int CheckInputParameters(int argc, char **argv, inputparameters *input_args); // returns 0 if OK,else -1
int Usage(int argc, char **argv);

int main(int argc, char **argv)
{
	inputparameters input_args;
	channel original;
	channel black;
	channel result;

	if( CheckInputParameters(argc, argv, &input_args) == -1 )
	{
		printf("Error. Check parameters\n");
		Usage(argc, argv);
		return -1;
	}

	if(GetDataFromPGM(input_args.inputfilename_orig, &original) == -1)
	{
		printf("Error. Getting data from .pgm file\n");
		return -1;
	}
	
	if(GetDataFromPGM(input_args.inputfilename_black, &black) == -1)
	{
		printf("Error. Getting data from .pgm file\n");
		return -1;
	}

	if(SubstractData(&result, &original, &black) == -1)
	{
		printf("Error. Substracting black level file.\n");
		return -1;
	}

	if(ExportColorChannel(input_args.outputfilename, result.data, result.width, result.height, result.pixel_size) == -1)
	{
		printf("Can't export pixel data\n");
		return -1;
	}
	
	return 0;
}




int CheckInputParameters(int argc, char **argv, inputparameters *input_args)
{
	if(argc == 4)
	{
		strcpy(input_args->inputfilename_orig,argv[1]);
		strcpy(input_args->inputfilename_black,argv[2]);
		strcpy(input_args->outputfilename,argv[3]);
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
	printf("\t%s <inputfilename original> <inputfilename black> <outputfilename>\n", argv[0]);

	return 0;
}



