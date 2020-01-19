#include "fft_func.h"
#include "fft_other.h"
#include "fft_math.h"
#include "fft_data_manipulation.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	unsigned int N = 0;
	int i;
	signal sig;

	if(argc != 2)
	{
		printf("Error. Input parameters\n");
		return 0;
	}
	
	N = (unsigned int)atof(argv[1]);

	if( SignalInit(&sig, N, 1.0) == EXIT_FAILURE )
	{
		printf("Memory\n");
		return 0;
	}
	
	printf("Generating signal\n");
	for(i = 0; i < N; i++)
	{
		sig.data[i] = ComplexAlgSet((double)i, (double)i);
	}
	//print_complex_signal(&sig);
	for(i = 0; i < 0; i++)
	{
		printf("Calculating spectrum\n");
		direct_dft(&sig);
		//print_complex_spectrum(&sig);
	}
		
	for(i = 0; i < 0; i++)
	{
		printf("Calculating Data\n");
		inverse_dft(&sig);
		//print_complex_signal(&sig);
	}
	
	
	return 0;
	
}













