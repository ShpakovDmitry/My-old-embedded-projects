#define PI 3.1415926535897932384626433832795
#define FALSE 0
#define TRUE  1

typedef struct
{
	double real;
	double imaginary;
} complex;

typedef struct
{
	complex *signal;
	complex *spectrum;
	unsigned int N;
	double sample_rate;
} data_sequence;


void direct_dft(data_sequence *data);
void inverse_dft(data_sequence *data);

double magnitude_complex(complex num);
double absolute_value(double num);
void print_complex_signal(data_sequence *data);
void print_complex_spectrum(data_sequence *data);

unsigned int pow_of_two(unsigned int pow);
unsigned int is_power_of_two(unsigned int number);
unsigned int max_divisions_of_two(unsigned int number);

void sort_even_odd_window(complex *data, unsigned start, unsigned end);
void sort_data(data_sequence *data, unsigned int fft_depth);
void dft_window(data_sequence *data, unsigned int fft_depth);
void direct_dft_window(data_sequence *data, unsigned start, unsigned end);
void butterfly_data_window(data_sequence *data, unsigned fft_depth);
void butterfly_data_start_end(data_sequence *data, unsigned start, unsigned end);


void fft(data_sequence *data);













