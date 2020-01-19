//
//
//

#include "fft_math.h"

#ifndef _FFT_FUNC_
#define _FFT_FUNC_

typedef struct		// structure to hold signal data and its spectrum with addtional info; Can be used to hold spectrum from signal and vice versa
{
	complex_alg *data;
	complex_alg *spectrum;
	unsigned int sample_num;
	double sample_rate;
}signal;

extern unsigned int SignalInit(signal *sig, unsigned int sample_num, double sample_rate);

void direct_dft(signal *sig);		// O(N^2)
void inverse_dft(signal *sig);		// O(N^2)

void fft(signal *sig);				// O(N log N) Cooley-Tookey; non recursive
void ifft(signal *sig);				// O(N log N) Cooley-Tookey; non recursive













#endif
