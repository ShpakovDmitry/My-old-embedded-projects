#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dft.h"

int main(int argc, char **argv)
{
	unsigned int N = 0;
	int i;
	data_sequence signal;

	if(argc != 2)
	{
		printf("Error. Input parameters\n");
		return 0;
	}
	N = (unsigned int)atof(argv[1]);
	
	signal.signal = (complex *)malloc(N*sizeof(complex));
	signal.spectrum = (complex *)malloc(N*sizeof(complex));
	signal.N = N;
	signal.sample_rate = 2.0;
	
	if(signal.signal == NULL || signal.spectrum == NULL)
	{
		printf("Memory\n");
		return 1;
	}

	printf("Generating signal\n");
	for(i = 0; i < N; i++)
	{
		//signal.signal[i].real = cos((double)i);
		//signal.signal[i].imaginary =  0.0;
		signal.signal[i].real = (double)(i*1 + 0);
		signal.signal[i].imaginary =  0.0;
	}
	print_complex_signal(&signal);
	//printf("\n");
	//sort_even_odd_window(signal.signal, 0, N - 1);
	//print_complex_signal(&signal);
	
	for(i = 0; i < 1; i++)
	{
		printf("Calculating spectrum\n");
		direct_dft(&signal);
		print_complex_spectrum(&signal);
	}
	for(i = 0; i < 0; i++)
	{
		printf("Calculating fft_2 spectrum\n");
		direct_dft_size2(&signal);
		print_complex_spectrum(&signal);
	}

	for(i = 0; i < 1; i++)
	{
		printf("Calculating fft spectrum\n");
		fft(&signal);
		print_complex_spectrum(&signal);
	}
	
	//print_complex_signal(&signal);
	//printf("\n");
	
	
	for(i = 0; i < 0; i++)
	{
		printf("Calculating signal\n");
		inverse_dft(&signal);
		print_complex_signal(&signal);
	}

	free(signal.signal);
	free(signal.spectrum);
	
	return 0;
}
