#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	double real;
	double imaginary;
} complex;

typedef struct
{
	complex *a;
	complex b;
} set;

void f(set *data)
{
	data->a = (complex *)malloc(5*sizeof(complex));
	data->a[0].real = 5.0;
}

int main(int argc, char **argv)
{
	complex i;
	set data;
	
	f(&data);
	printf("info: %lf\n", data.a[0].real);
	
	i.real = 0;
	i.imaginary = 1;
	
	printf("Re{i} = %lf\n", i.real);
	printf("Im{i} = %lf\n", i.imaginary);
	
	return 0;
}
