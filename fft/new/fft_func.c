//
//
//


#include "fft_math.h"
#include "fft_func.h"
#include "fft_data_manipulation.h"




void direct_dft(signal *sig)
{
	unsigned int k,n;
	
	for(k = 0; k < sig->sample_num; k++)
	{
		sig->spectrum[k] = ComplexAlgSub(sig->spectrum[k], sig->spectrum[k]);	// set as Zero;  A - A = 0
		for(n = 0; n < sig->sample_num; n++)
		{
			// we do X[k] += x[n] * exp(...)
			sig->spectrum[k] = ComplexAlgSum(sig->spectrum[k], ComplexAlgMulComplexExp(sig->data[n], ComplexExpSet(1.0, -PI_2/(sig->sample_num)*k*n)));
		}
	}
}

void inverse_dft(signal *sig)
{
	unsigned k,n;
	
	for(n = 0; n < sig->sample_num; n++)
	{
		sig->data[n] = ComplexAlgSub(sig->data[n], sig->data[n]);	// set as Zero; A - A = 0
		for(k = 0; k < sig->sample_num; k++)
		{
			// we do x[n] += X[k] * exp(...)
			sig->data[n] = ComplexAlgSum(sig->data[n], ComplexAlgMulComplexExp(sig->spectrum[k], ComplexExpSet(1.0, PI_2/(sig->sample_num)*k*n)));
		}
		sig->data[n] = ComplexAlgDiv(sig->data[n], ComplexAlgSet((double)(sig->sample_num), 0.0));
	}
}


void fft(signal *sig)				// O(N log N) Cooley-Tookey; non recursive
{
	signal sig_copy
	
}









