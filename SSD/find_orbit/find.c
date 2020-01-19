#ifndef _STDIO_H_
 #include <stdio.h>
 #define _STDIO_H_
#endif

#include "functions.c"

int main(int argc, char **argv)
{
	orbit Ephemeris;
	observation obs1 = {2426498.536852, 4.472342940, -0.238301438};
	observation obs0 = {2426513.537190, 4.416882194, -0.249082240};
	observation obs2 = {2426529.535703, 4.370287721, -0.265193083};

	Get_orbit(obs1, obs0, obs2, &Ephemeris, stdout);

//	int Y, M, D;
//	double UT;
//	double JD;
//	JulianDateToGregorianDate(2415020.0, &Y, &M, &D, &UT);
//	printf("%d %d %d %lf\n", Y, M, D, UT);
//	GregorianDateToJulianDate(1931, 6, 6, 0.88445, &JD);
//	printf("JD = %lf\n", JD);
//	GregorianDateToJulianDate(1931, 6,21, 0.89257, &JD);
//	printf("JD = %lf\n", JD);
//	GregorianDateToJulianDate(1931, 7, 7, 0.85688, &JD);
//	printf("JD = %lf\n", JD);
//
//	JulianDateToGregorianDate(2426529.535703, &Y, &M, &D, &UT);
//	printf("%d %d %d %lf\n", Y, M, D, UT);

	printf("a = %lf\ne = %lf\ni = %lf\n", Ephemeris.SemiMajorAxis, Ephemeris.Eccentricity, Ephemeris.Inclination * 180.0/M_PI);

	return 0;
}
