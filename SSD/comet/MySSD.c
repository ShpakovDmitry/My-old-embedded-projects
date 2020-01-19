#include "MySSD.h"


int int_part(double a)                                                              // возвращает целую часть числа без округления
{
	return a;
}

int ssd_Int(double argument)                                                        //возвращает наибольшее целое число, меньшее или равное "argument"
{
	if ( (argument < 0) && (argument == (double) int_part(argument)) )
	     return int_part(argument);
	else if (argument < 0)
	     return ( int_part(argument) - 1 );
	else return int_part(argument);
}

double ssd_Frac(double argument)                                                   // возвращает дробную часть числа (с округлением)
{
	return ( argument - (double)int_part(argument) );
}

double ssd_Absolute_lf(double argument)
{
	if(argument < 0)
	{
		return -argument;
	}
	return argument;
}

double ssd_sqr_lf(double argument)
{
	return argument*argument;
}

double ssd_cube_lf(double argument)
{
	return argument*argument*argument;
}

void LogCurrentPlanetPossition(FILE *PlanetLogFile, planet body)
{
	fprintf(PlanetLogFile, "%lf %lf ", SimulationTime, GlobalTimeJD);
	fprintf(PlanetLogFile, "%lf %lf %lf ", body.pos.X_coord, body.pos.Y_coord, body.pos.Z_coord);
	fprintf(PlanetLogFile, "\n");
}

void LogCurrentCometPossition(FILE *CometLogFile, comet body)
{
	fprintf(CometLogFile, "%lf %lf ", SimulationTime, GlobalTimeJD);

	fprintf(CometLogFile, "%lf, %lf %lf ", body.pos.X_coord, body.pos.Y_coord, body.pos.Z_coord);
	fprintf(CometLogFile, "%lf, %lf %lf ", body.vel.VX_vel, body.vel.VY_vel, body.vel.VZ_vel);
	fprintf(CometLogFile, "%lf, %lf %lf ", body.grav_accel.aX_accel, body.grav_accel.aY_accel, body.grav_accel.aZ_accel  );
	fprintf(CometLogFile, "%lf, %lf %lf ", body.non_grav_accel.aX_accel, body.non_grav_accel.aY_accel, body.non_grav_accel.aZ_accel  );
	fprintf(CometLogFile, "\n");
}
void GregorianDateToJulianDate(int Year, int Month, int Day, double UT, double *JD)
{
	int y, m, B;

	if( Month <= 2 )
	{
		y = Year - 1;
		m = Month + 12;
	}
	else
	{
		y = Year;
		m = Month;
	}

	if( Year < 1582 )
	{
		B = -2;
	}
	else if( Year == 1582 )
	{
		if( Month < 10 )
		{
			B = -2;
		}
		else if( Month == 10 )
		{
			if( Day <= 4 )
			{
				B = -2;
			}
			if( Day >= 15 )
			{
				B = ssd_Int((double)y/400.0) - ssd_Int((double)y/100.0);
			}
		}
		else if( Month > 10 )
		{
			B = ssd_Int((double)y/400.0) - ssd_Int((double)y/100.0);
		}
	}
	else if( Year > 1582 )
	{
		B = ssd_Int((double)y/400.0) - ssd_Int((double)y/100.0);
	}

	*JD = ssd_Int(365.25*(double)y) + ssd_Int(30.6001*((double)m + 1)) + \
		(double)B + 1720996.5 + (double)Day + UT/24.0;
}

void JulianDateToGregorianDate(double JD, int *Year, int *Month, int *Day, double *UT)
{
	int a, b, c, d, e, f;

	a = ssd_Int( JD + 0.5 );
	
	if( a < 2299161 )
	{
		c = 1524;
	}
	else
	{
		b = ssd_Int(((double)a - 1867216.25)/36524.25);
		c = a + b - ssd_Int((double)b/4.0) + 1525;
	}
	
	d = ssd_Int(((double)c - 122.1)/365.25);
	e = ssd_Int(365.25*(double)d);
	f = ssd_Int(((double)c -(double)e)/30.6001);

	*UT   =24.0 * ssd_Frac( JD + 0.5 );
	*Day = (int)( (double)c - (double)e - ssd_Int(30.6001*(double)f) + ssd_Frac(JD + 0.5) );
	*Month = (int)( (double)f - 1 - 12.0*ssd_Int((double)f/14.0) );
	*Year = ( (double)d - 4715.0 - ssd_Int((7.0 + (double)*Month)/10.0) );
}

void KeplersEquation(double MeanAnomaly, double Eccentricity, double *EccentricAnomaly)
{
	*EccentricAnomaly = MeanAnomaly;
	double temporary = *EccentricAnomaly;
	double rel_error;
	unsigned long int i = 0;

	do
	{
		*EccentricAnomaly = MeanAnomaly + Eccentricity * (double)sin(*EccentricAnomaly);
		rel_error = ssd_Absolute_lf(1 - temporary/(*EccentricAnomaly));
		temporary = *EccentricAnomaly;
	}while( (i++ < MAX_Kepler_Iterations) && (rel_error > Kepler_relative_error)  );
}

void RecalculatePlanetEphemeris(planet *X)
{
	double timediff = (GlobalTimeJD - (*X).Epoch.value)/(Julian_century_in_days);
	
	(*X).SemiMajorAxis.value += (*X).SemiMajorAxis.change * timediff;
	(*X).Eccentricity.value  += (*X).Eccentricity.change * timediff;
	(*X).Inclination.value   += (*X).Inclination.change * timediff;
	(*X).LongitudeOfPericenter.value += (*X).LongitudeOfPericenter.change * timediff;
	(*X).AscendingNode.value += (*X).AscendingNode.change * timediff;
	(*X).MeanLongitude.value += ( (*X).MeanLongitude.change + (*X).N_r.value ) * timediff;
}


void RecalculateCurrentPlanetPosition(planet *body)
{
	double EccentricAnomaly;
	double SemiMajorAxis = (*body).SemiMajorAxis.value;
	double Eccentricity = (*body).Eccentricity.value;
	double MeanAnomaly = (*body).MeanLongitude.value - (*body).LongitudeOfPericenter.value;

	KeplersEquation(MeanAnomaly, Eccentricity, &EccentricAnomaly);

	double x,y;

	x = SemiMajorAxis * (cos(EccentricAnomaly) - Eccentricity);
	y = SemiMajorAxis * sqrt( 1 - ssd_sqr_lf(Eccentricity) ) * sin(EccentricAnomaly);

	double ArgumentOfPericenter = (*body).LongitudeOfPericenter.value - (*body).AscendingNode.value;

	double sin_ArgOfPericenter = sin(ArgumentOfPericenter);
	double cos_ArgOfPericenter = cos(ArgumentOfPericenter);
	double sin_AscendingNode = sin( (*body).AscendingNode.value );
	double cos_AscendingNode = cos( (*body).AscendingNode.value );
	double sin_Inclination = sin( (*body).Inclination.value );
	double cos_Inclination = cos( (*body).Inclination.value );

	double X_1 = cos_AscendingNode * cos_ArgOfPericenter - sin_AscendingNode * cos_Inclination * sin_ArgOfPericenter;
	double X_2 = -cos_AscendingNode * sin_ArgOfPericenter - sin_AscendingNode * cos_Inclination * cos_ArgOfPericenter;
	double Y_1 = sin_AscendingNode * cos_ArgOfPericenter + cos_AscendingNode * cos_Inclination * sin_ArgOfPericenter;
	double Y_2 = -sin_AscendingNode * sin_ArgOfPericenter + cos_AscendingNode * cos_Inclination * cos_ArgOfPericenter;
	double Z_1 = sin_Inclination * sin_ArgOfPericenter;
	double Z_2 = sin_Inclination * cos_ArgOfPericenter;

	(*body).pos.X_coord = X_1 * x + X_2 * y;
	(*body).pos.Y_coord = Y_1 * x + Y_2 * y;
	(*body).pos.Z_coord = Z_1 * x + Z_2 * y;

}

void RecalculateCurrentCometPosition(comet *body)
{
	double EccentricAnomaly;
	double Eccentricity = (*body).Eccentricity.value;
	double SemiMajorAxis = (*body).PerihelionDistance.value / (1 - Eccentricity);
	double MeanMotion = Gauss_const / sqrt( ssd_cube_lf(SemiMajorAxis) );
	double MeanAnomaly = MeanMotion * ( GlobalTimeJD - (*body).PericenterTransitTime.value );

	KeplersEquation(MeanAnomaly, Eccentricity, &EccentricAnomaly);

	double x,y;

	x = SemiMajorAxis * (cos(EccentricAnomaly) - Eccentricity);
	y = SemiMajorAxis * sqrt( 1 - ssd_sqr_lf(Eccentricity) ) * sin(EccentricAnomaly);

	double ArgumentOfPericenter = (*body).ArgumentOfPericenter.value;
	
	double sin_ArgOfPericenter = sin(ArgumentOfPericenter);
	double cos_ArgOfPericenter = cos(ArgumentOfPericenter);
	double sin_AscendingNode = sin( (*body).AscendingNode.value );
	double cos_AscendingNode = cos( (*body).AscendingNode.value );
	double sin_Inclination = sin( (*body).Inclination.value );
	double cos_Inclination = cos( (*body).Inclination.value );

	double X_1 = cos_AscendingNode * cos_ArgOfPericenter - sin_AscendingNode * cos_Inclination * sin_ArgOfPericenter;
	double X_2 = -cos_AscendingNode * sin_ArgOfPericenter - sin_AscendingNode * cos_Inclination * cos_ArgOfPericenter;
	double Y_1 = sin_AscendingNode * cos_ArgOfPericenter + cos_AscendingNode * cos_Inclination * sin_ArgOfPericenter;
	double Y_2 = -sin_AscendingNode * sin_ArgOfPericenter + cos_AscendingNode * cos_Inclination * cos_ArgOfPericenter;
	double Z_1 = sin_Inclination * sin_ArgOfPericenter;
	double Z_2 = sin_Inclination * cos_ArgOfPericenter;

	(*body).pos.X_coord = X_1 * x + X_2 * y;
	(*body).pos.Y_coord = Y_1 * x + Y_2 * y;
	(*body).pos.Z_coord = Z_1 * x + Z_2 * y;
}



















