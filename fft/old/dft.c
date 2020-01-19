// library implements discrete Fourier transformation
// Dmitry 2018
#include "dft.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void direct_dft(data_sequence *data)
{
	int k, n;
	unsigned int i;
	static double *sin_tab = NULL;
	static double *cos_tab = NULL;
	static char first_call = FALSE;
	static double sample_num = -1.0;
	
	if(first_call == FALSE || sample_num != data->N)
	{
		if(sin_tab != NULL)
			free(sin_tab);
		if(cos_tab != NULL)
			free(cos_tab);
		sin_tab = (double *)malloc(data->N*sizeof(double));
		cos_tab = (double *)malloc(data->N*sizeof(double));
		if(sin_tab == NULL || cos_tab == NULL)
		{
			printf("Error. direct. memory\n");
			return;
		}
		for(i = 0; i < data->N; i++)
		{
			sin_tab[i] = sin(2.0*PI*i/data->N);
			cos_tab[i] = cos(2.0*PI*i/data->N);
		}
		
		first_call = TRUE;
		sample_num = data->N;
	}
	
	for(k = 0; k < data->N; k++)
	{
		data->spectrum[k].real = 0.0;
		data->spectrum[k].imaginary = 0.0;
		for(n = 0; n < data->N; n++)
		{
			data->spectrum[k].real += 
			data->signal[n].real*cos_tab[ k*n % data->N ] +
			data->signal[n].imaginary*sin_tab[ k*n % data->N ];
			
			data->spectrum[k].imaginary +=
			data->signal[n].imaginary*cos_tab[ k*n % data->N ] -
			data->signal[n].real*sin_tab[ k*n % data->N ];
		}
	}
}
void inverse_dft(data_sequence *data)
{
	int k, n;
	unsigned int i;
	static double *sin_tab = NULL;
	static double *cos_tab = NULL;
	static char first_call = FALSE;
	static double sample_num = -1.0;
	
	if(first_call == FALSE || sample_num != data->N)
	{
		if(sin_tab != NULL)
			free(sin_tab);
		if(cos_tab != NULL)
			free(cos_tab);
		sin_tab = (double *)malloc(data->N*sizeof(double));
		cos_tab = (double *)malloc(data->N*sizeof(double));
		if(sin_tab == NULL || cos_tab == NULL)
		{
			printf("Error. direct. memory\n");
			return;
		}
		for(i = 0; i < data->N; i++)
		{
			sin_tab[i] = sin(2.0*PI*i/data->N);
			cos_tab[i] = cos(2.0*PI*i/data->N);
		}
		
		first_call = TRUE;
		sample_num = data->N;
	}
	
	for(n = 0; n < data->N; n++)
	{
		data->signal[n].real = 0.0;
		data->signal[n].imaginary = 0.0;
		for(k = 0; k < data->N; k++)
		{
			data->signal[n].real += 
			data->spectrum[k].real*cos_tab[ k*n % data->N ] -
			data->spectrum[k].imaginary*sin_tab[ k*n % data->N ];
			
			data->signal[n].imaginary +=
			data->spectrum[k].imaginary*cos_tab[ k*n % data->N ] +
			data->spectrum[k].real*sin_tab[ k*n % data->N ];
		}
		data->signal[n].real /= data->N;
		data->signal[n].imaginary /= data->N;
	}
}


double magnitude_complex(complex num)
{
	double res;
	
	res = sqrt( num.real*num.real + num.imaginary*num.imaginary );
	
	return res;
}

double absolute_value(double num)
{
	if(num < 0.0)
		return (-num);
	return num;
}

unsigned int pow_of_two(unsigned int pow)
{
	unsigned int res = 1;
	unsigned int i;
	for(i = 0; i < pow; i++)
		res*=2;
	return res;
}

unsigned int is_power_of_two(unsigned int number)
{
	unsigned int i;
	
	for(i = 0; number != 0; number /= 2)
	{
		i += number%2;
	}
	return i == 1 ? TRUE : FALSE; 
}

unsigned int max_divisions_of_two(unsigned int number)
{
	unsigned int i;
	
	if(number%2 == 1 || number == 0)
		return 0;
	
	for(i = 1; (number/2)%2 != 1 ;i++)
	{
		number /= 2;
	}
	return i;
}

void print_complex_signal(data_sequence *data)
{
	unsigned int i;
	
	for(i = 0; i < data->N; i++)
		printf("N = %u t = %.6lf %10.6lf %c %.6lfi\tmagnitude = %.6lf\n",
			i,
			(double)i/data->sample_rate,
			data->signal[i].real,
			(data->signal[i].imaginary >= 0.0) ? '+' : '-',
			absolute_value(data->signal[i].imaginary),
			magnitude_complex(data->signal[i])
		);
}

void print_complex_spectrum(data_sequence *data)
{
	unsigned int i;
	
	for(i = 0; i < data->N; i++)
		printf("k = %u freq = %lf %10.6lf %c %.6lfi\tmagnitude = %.6lf\n",
			i,
			(double)i/data->N*data->sample_rate,
			data->spectrum[i].real,
			(data->spectrum[i].imaginary >= 0.0) ? '+' : '-',
			absolute_value(data->spectrum[i].imaginary),
			magnitude_complex(data->spectrum[i])
		);
}

void sort_even_odd_window(complex *data, unsigned start, unsigned end)
{
	unsigned win_size = end - start + 1;
	unsigned i;
	complex *tmp = (complex *)malloc(win_size/2*sizeof(complex));
	if(tmp == NULL)
	{
		printf("Error.sort.memory\n");
		return;
	}
	
	for(i = 0; i < win_size/2; ++i)
	{
		tmp[i] = data[start + 2*i + 1];
	}
	for(i = 0; i < win_size/2; ++i)
	{
		data[start + i] = data[start + 2*i];
	}
	for(i = 0; i < win_size/2; ++i)
	{
		data[start + i + win_size/2] = tmp[i];
	}
	return;
	
	free(tmp);
}

void sort_data(data_sequence *data, unsigned int fft_depth)
{
	unsigned i, j;
	unsigned int window;
	
	for(i = 0; i < fft_depth; ++i)
	{
		window = data->N/pow_of_two(i);
		for(j = 0; j <= i; ++j)
		{
			sort_even_odd_window(data->signal, window*j, window*(j + 1) - 1);
		}
	}
}

void dft_window(data_sequence *data, unsigned int fft_depth)
{
	unsigned int window;
	unsigned int i;
	
	window = data->N/pow_of_two(fft_depth);
	//printf("dft_window %d\n", window);
	
	for(i = 0; i < pow_of_two(fft_depth); ++i)
	{
		direct_dft_window(data, window*i, window*(i + 1) - 1);
	}
}

void direct_dft_window(data_sequence *data, unsigned start, unsigned end)
{
	int k, n;
	unsigned int i;
	unsigned win_size = end - start + 1;
	static double *sin_tab = NULL;
	static double *cos_tab = NULL;
	static char first_call = FALSE;
	static double sample_num = -1.0;
	
	//printf("dft window: %d start: %d end: %d\n", win_size, start, end);
	if(first_call == FALSE || sample_num != win_size)
	{
		if(sin_tab != NULL)
			free(sin_tab);
		if(cos_tab != NULL)
			free(cos_tab);
		sin_tab = (double *)malloc(win_size*sizeof(double));
		cos_tab = (double *)malloc(win_size*sizeof(double));
		if(sin_tab == NULL || cos_tab == NULL)
		{
			printf("Error. direct. memory\n");
			return;
		}
		for(i = 0; i < win_size; i++)
		{
			sin_tab[i] = sin(2.0*PI*i/win_size);
			cos_tab[i] = cos(2.0*PI*i/win_size);
		}
		
		first_call = TRUE;
		sample_num = win_size;
	}
	
	for(k = 0; k < win_size; k++)
	{
		data->spectrum[k + start].real = 0.0;
		data->spectrum[k + start].imaginary = 0.0;
		for(n = 0; n < win_size; n++)
		{
			data->spectrum[k + start].real += 
			data->signal[n + start].real*cos_tab[ k*n % win_size ] +
			data->signal[n + start].imaginary*sin_tab[ k*n % win_size ];
			
			data->spectrum[k + start].imaginary +=
			data->signal[n + start].imaginary*cos_tab[ k*n % win_size ] -
			data->signal[n + start].real*sin_tab[ k*n % win_size ];
		}
	}
}

void butterfly_data_window(data_sequence *data, unsigned fft_depth)
{
	unsigned int window, iterations;
	unsigned int i, j;
	
	//window = data->N/pow_of_two(fft_depth);
	//iterations = pow_of_two(fft_depth)/2;
	
	
	for(i = fft_depth; i != 0; --i)
	{
		window = data->N/pow_of_two(i);
		for(j = 0; j <= pow_of_two(i)/2 - 1; ++j)
		{
			butterfly_data_start_end(data, 2*window*j, 2*window*(j + 1) - 1);
		}
	}
	//TODO FIXME
	/*
	do
	{
		for(i = 0; i < iterations; ++i)
		{
			printf("start %d\tend %d\n", 2*window*i, 2*window*(i + 1) - 1);
			printf("iterations %d\n", iterations);
			printf("window %d\n", window);
			butterfly_data_start_end(data, 2*window*i, 2*window*(i + 1) - 1);
			// FIXME !!!
		}
		window*=2;
		iterations/=2;
	}while(iterations > 0);
	*/
	
}

void butterfly_data_start_end(data_sequence *data, unsigned start, unsigned end)
{
	unsigned int window = end - start + 1;
	printf("butterfly window: %d start: %d end: %d\n", window, start, end);
	complex *result;
	unsigned int i;
	double tmp_sin, tmp_cos;
	result = (complex *)malloc(window*sizeof(complex));
	if(result == NULL)
	{
		printf("Error.butterfly_data_window.memory\n");
		return;
	}
	
	for(i = 0; i < window/2; i++)
	{
		tmp_sin = sin(2.0*PI*i/window);
		tmp_cos = cos(2.0*PI*i/window);
		result[i].real = \
			data->spectrum[i + start].real + \
			data->spectrum[i + start + window/2].real*tmp_cos + \
			data->spectrum[i + start + window/2].imaginary*tmp_sin;
		result[i].imaginary = \
			data->spectrum[i + start].imaginary + \
			data->spectrum[i + start + window/2].imaginary*tmp_cos - \
			data->spectrum[i + start + window/2].real*tmp_sin;
		
		result[i + window/2].real = \
			data->spectrum[i + start].real - \
			data->spectrum[i + start + window/2].real*tmp_cos - \
			data->spectrum[i + start + window/2].imaginary*tmp_sin;
		result[i + window/2].imaginary = \
			data->spectrum[i + start].imaginary - \
			data->spectrum[i + start + window/2].imaginary*tmp_cos + \
			data->spectrum[i + start + window/2].real*tmp_sin;
	}
	
	//copy result to data_sequence
	for(i = 0; i < window; ++i)
		data->spectrum[i + start] = result[i];
	free(result);
}




void fft(data_sequence *data)
{
	unsigned int fft_depth;
	
	fft_depth = max_divisions_of_two(data->N);
	printf("fft_depth %d\n", fft_depth);
	
	
	//sort data
	sort_data(data, fft_depth);
	//printf("sorted data\n");
	//print_complex_signal(data);
	
	// calculate dft for each individual window
	dft_window(data, fft_depth);
	//printf("dft_window spectrum data\n");
	//print_complex_spectrum(data);
	
	
	// butterfly_data
	// TODO TEST FIXME
	butterfly_data_window(data, fft_depth);
	
	
	
	
	//TODO FIXME TEST
	
	
	//XXX TEST 
	//data->signal = signal_copy;
	//print_complex_signal(data);
	
	
	
}





