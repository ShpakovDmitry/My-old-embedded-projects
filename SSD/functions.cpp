#ifndef _MATH_H_
 #include <math.h>
 #define _MATH_H_
#endif

double keplers_equation(double mean_anomaly, double eccentricity)
{
	double E = mean_anomaly*M_PI/180.0;
	int i;
	
	for(i=0; i<100; i++)
	{
		E = mean_anomaly + eccentricity*sin(E);
	}
	
	return E*180.0/M_PI;
	
}

void from_elements_to_coord(double elements[6], double *coord[6])
{
	
}
