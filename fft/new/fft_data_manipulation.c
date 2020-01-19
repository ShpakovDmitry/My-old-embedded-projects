//
//
//

#include "fft_func.h"
#include "fft_data_manipulation.h"
#include <stdlib.h>		// malloc(),

unsigned int SignalInit(signal *sig, unsigned int sample_num, double sample_rate)
{
	if( (sig->data = (complex_alg *)malloc(sizeof(complex_alg)* sample_num)) == NULL)
		return EXIT_FAILURE;
	
	if( (sig->spectrum = (complex_alg *)malloc(sizeof(complex_alg)* sample_num)) == NULL)
	{
		free(sig->data);
		return EXIT_FAILURE;
	}
	sig->sample_num = sample_num;
	sig->sample_rate = sample_rate;
	return EXIT_SUCCESS;
}

void SignalFree(signal *sig)
{
	if(sig->data != NULL)
		free(sig->data);
	if(sig->spectrum != NULL)
		free(sig->spectrum);
}

void SignalCopy(signal *B, signal *A)	// A ->copy-> B
{
	unsigned int i;
	SignalFree(B);	// make sure B is empty; so no memory leak
	SignalInit(B, A->sample_num, A->sample_rate);
	for(i = 0; i < B->sample_num; i++)
	{
		B->data[i] = A->data[i];
		B->spectrum[i] = A->spectrum[i];
	}
}


























