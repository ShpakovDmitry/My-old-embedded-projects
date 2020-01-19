
void butterfly_data(complex *E, complex *O, complex *R, unsigned int N);
void separate(data_sequence *data, data_sequence *E, data_sequence *O);
void init_data_sequence(data_sequence *data, unsigned int N, double sample_rate);
void replicate_data_sequence(data_sequence *copy, data_sequence *orig);

void butterfly_data(complex *E, complex *O, complex *R, unsigned int N)
{
	unsigned int i;
	double tmp_sin, tmp_cos;
	
	for(i = 0; i < N/2; i++)
	{
		tmp_sin = sin(2.0*PI*i/N);
		tmp_cos = cos(2.0*PI*i/N);
		R[i].real = \
			E[i].real + \
			O[i].real*tmp_cos + \
			O[i].imaginary*tmp_sin;
		R[i].imaginary = \
			E[i].imaginary + \
			O[i].imaginary*tmp_cos - \
			O[i].real*tmp_sin;
		
		R[i + N/2].real = \
			E[i].real - \
			O[i].real*tmp_cos - \
			O[i].imaginary*tmp_sin;
		R[i + N/2].imaginary = \
			E[i].imaginary - \
			O[i].imaginary*tmp_cos + \
			O[i].real*tmp_sin;
	}
}

void separate(data_sequence *data, data_sequence *E, data_sequence *O)
{
	unsigned int i;
	for(i = 0; i < data->N/2; i++)
	{
		E->signal[i] = data->signal[2*i];
		O->signal[i] = data->signal[2*i+1];
	}
}

void init_data_sequence(data_sequence *data, unsigned int N, double sample_rate)
{
	data->signal = (complex *)malloc(sizeof(complex)*N);
	data->spectrum = (complex *)malloc(sizeof(complex)*N);
	data->N = N;
	data->sample_rate = sample_rate;
}

void replicate_data_sequence(data_sequence *copy, data_sequence *orig)
{
	unsigned i;
	
	copy->signal = (complex *)malloc(sizeof(complex)*orig->N);
	copy->spectrum = (complex *)malloc(sizeof(complex)*orig->N);
	copy->N = orig->N;
	copy->sample_rate = orig->sample_rate;
	
	for(i = 0; i < copy->N; ++i)
	{
		copy->signal[i] = orig->signal[i];
		copy->spectrum[i] = orig->spectrum[i];
	}
}




void direct_dft_size2(data_sequence *data);

void direct_dft_size2(data_sequence *data)
{
	data_sequence E;
	data_sequence O;
	
	if( (data->N % 2) != 0 )
	{
		printf("Error. direct_dft_size2. N must be even\n");
		return;
	}
	init_data_sequence(&E, data->N/2, data->sample_rate);
	init_data_sequence(&O, data->N/2, data->sample_rate);
	separate(data, &E, &O);
	
	direct_dft(&E);
	direct_dft(&O);
	
	butterfly_data(E.spectrum, O.spectrum, data->spectrum, data->N);
	
	free(E.signal);	free(E.spectrum);
	free(O.signal);	free(O.spectrum);
	
}
