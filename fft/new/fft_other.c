//
//
//

#include "fft_other.h"
#include <stdio.h>

void print_complex_signal(signal *sig)
{
	unsigned int i;
	
	for(i = 0; i < sig->sample_num; i++)
		printf("N = %u t = %.6lf %10.6lf %c %.6lfi\tMag^2 = %.6lf\tMag = %.6lf\tPhi = %.6lf rad = %.6lf deg\n",
			i,
			(double)i/sig->sample_rate,
			sig->data[i].Re,
			(sig->data[i].Im >= 0.0) ? '+' : '-',
			ValueAbsolute(sig->data[i].Im),
			ComplexAlgMag2(sig->data[i]),
			ComplexAlgMag(sig->data[i]),
			(ComplexAlgToComplexExp(sig->data[i])).Phi,
			((ComplexAlgToComplexExp(sig->data[i])).Phi)*360.0/PI_2
		);
}

void print_complex_spectrum(signal *sig)
{
	unsigned int i;
	
	for(i = 0; i < sig->sample_num; i++)
		printf("K = %u freq = %lf %10.6lf %c %.6lfi\t|Mag|^2 = %.6lf\tMag = %.6lf\tPhi = %.6lf rad = %.6lf deg\n",
			i,
			(double)i/sig->sample_num*sig->sample_rate,
			sig->spectrum[i].Re,
			(sig->spectrum[i].Im >= 0.0) ? '+' : '-',
			ValueAbsolute(sig->spectrum[i].Im),
			ComplexAlgMag2(sig->spectrum[i]),
			ComplexAlgMag(sig->spectrum[i]),
			(ComplexAlgToComplexExp(sig->spectrum[i])).Phi,
			((ComplexAlgToComplexExp(sig->spectrum[i])).Phi)*360.0/PI_2
		);
}
