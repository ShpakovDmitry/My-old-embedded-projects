#ifndef _STDIO_H_
 #include <stdio.h>
 #define _STDIO_H_
#endif

#ifndef _MATH_H_
 #include <math.h>
 #define _MATH_H_
#endif




#define Gauss_const 0.01720209895
#define MAX_Kepler_Iterations 1000
#define Kepler_relative_error 1e-10
#define MAX_Distance_Iterations 1000
#define Distance_relative_error 1e-10
#define MAX_recursive_iterations 1000
#define recursive_relative_error 1e-10
#define MAX_Eta_iterations 1000
#define Eta_relative_error 1e-10
#define MAX_S_iterations 1000
#define S_relative_error 1e-10




typedef struct
{
	double JD;	// Юлианская дата наблюдения
	double RA;	// прямое восхождение объекта наблюдения [радианы]
	double Dec;	// склонение объекта наблюдения [радианы]
}observation;

typedef struct
{
	double SemiMajorAxis;		// большая полуось		[а.е.]
	double Eccentricity;		// эксцентриситет		[ - ]
	double Inclination;		// наклонение орбиты		[радианы]
	double LongitudeOfPericenter;	// долгота перицентра		[радианы]
	double AscendingNode;		// долгота восходящего узла	[радианы]
	double MeanLongitude;		// средняя долгота		[радианы]
	double Epoch;			// эпоха эфемерид		[Юлианская дата]
}orbit;

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

double ssd_sqr_lf(double argument)
{
	return argument*argument;
}

double ssd_cube_lf(double argument)
{
	return argument*argument*argument;
}
double ssd_Absolute_lf(double argument)
{
	if(argument < 0)
	{
		return -argument;
	}
	return argument;
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

unsigned char GetSunGeocentricEquatorialRectangularCoordinates(double Date, double *X, double *Y, double *Z)
{
	double JDTimeDiff = Date - 2451545.0;		//J2000
	double JDTimeDiffCentury = JDTimeDiff/36525.0;
	// пересчёт эфемерид Земли
	double SemiMajorAxis = 1.00000011 + (-5e-8)*JDTimeDiffCentury;
	double Eccentricity  = 0.01671022 + (-3804e-8)*JDTimeDiffCentury;
	double Inclination   = (0.00005 + (-46.94/3600.0)*JDTimeDiffCentury)*M_PI/180.0;
	double LongitudeOfPericenter = (102.94719 + (1198.28/3600.0)*JDTimeDiffCentury)*M_PI/180.0;
	double AscendingNode = (348.73936 + (-18228.25/3600.0)*JDTimeDiffCentury)*M_PI/180.0;
	double MeanLongitude = (100.46435 + (1293740.63/3600.0+360.0*99)*JDTimeDiffCentury)*M_PI/180.0;

	double MeanAnomaly = MeanLongitude - LongitudeOfPericenter;
	double EccentricAnomaly;
	KeplersEquation(MeanAnomaly, Eccentricity, &EccentricAnomaly);
	
	double x,y;
	x = SemiMajorAxis*(cos(EccentricAnomaly) - Eccentricity);
	y = SemiMajorAxis*sqrt(1 - ssd_sqr_lf(Eccentricity))*sin(EccentricAnomaly);
	
	
	double ArgumentOfPericenter = LongitudeOfPericenter - AscendingNode;

	double sin_ArgOfPericenter = sin(ArgumentOfPericenter);
	double cos_ArgOfPericenter = cos(ArgumentOfPericenter);
	double sin_AscendingNode = sin(AscendingNode);
	double cos_AscendingNode = cos(AscendingNode);
	double sin_Inclination = sin(Inclination);
	double cos_Inclination = cos(Inclination);

	double X_1 = cos_AscendingNode * cos_ArgOfPericenter - sin_AscendingNode * cos_Inclination * sin_ArgOfPericenter;
	double X_2 = -cos_AscendingNode * sin_ArgOfPericenter - sin_AscendingNode * cos_Inclination * cos_ArgOfPericenter;
	double Y_1 = sin_AscendingNode * cos_ArgOfPericenter + cos_AscendingNode * cos_Inclination * sin_ArgOfPericenter;
	double Y_2 = -sin_AscendingNode * sin_ArgOfPericenter + cos_AscendingNode * cos_Inclination * cos_ArgOfPericenter;
	double Z_1 = sin_Inclination * sin_ArgOfPericenter;
	double Z_2 = sin_Inclination * cos_ArgOfPericenter;

	double X_coord = X_1 * x + X_2 * y;
	double Y_coord = Y_1 * x + Y_2 * y;
	double Z_coord = Z_1 * x + Z_2 * y;
	
	*X = -X_coord;
	*Y = -Y_coord;
	*Z = -Z_coord;

	return 0;
}

double Determinant3x3(double x0, double x1, double x2, \
		      double y0, double y1, double y2, \
		      double z0, double z1, double z2)
{
	return ((x0*y1*z2 + x1*y2*z0 + x2*y0*z1) - (x2*y1*z0 + x1*y0*z2 + x0*y2*z1));
}

double Determinant2x2(double x0, double x1, \
		      double y0, double y1)
{
	return (x0*y1 - x1*y0);
}

unsigned char GetObjectGeoAndGelioCentricDistance(double P, double Q, double Qprim, double C, double Ssqr, double *rho, double *r)
{
	double r0 = 1.0;
	double rho0;
	double r1, rho1;
	double rel_error;
	double i = 0;
	
	rho0 = P - Q/(r0*r0*r0) - Qprim/(r0*r0*r0*r0*r0*r0);
	do
	{
		r1 = sqrt( ssd_sqr_lf(rho0 + C) + Ssqr );
		rho1 = P - Q/(r0*r0*r0) - Qprim/(r0*r0*r0*r0*r0*r0);
		rel_error = (ssd_Absolute_lf(1.0 - r0/r1) > ssd_Absolute_lf(1.0 - rho0/rho1)) ? \
			(ssd_Absolute_lf(1.0 - r0/r1)) : (ssd_Absolute_lf(1.0 - rho0/rho1));
		r0 = r1;
		rho0 = rho1;
	}while( (i++ < MAX_Distance_Iterations) && (rel_error > Distance_relative_error) );

	*rho = rho0;
	*r = r0;

	return 0;
}

unsigned char GetRecursiveEta(double *eta, double parameter)
{
	unsigned int i = 0;
	double res_old = parameter + 1.0;
	double res;
	double rel_error;

	do
	{
		res = 1 + parameter/res_old;
		rel_error = ssd_Absolute_lf(1.0 - res_old/res);
		res_old = res;
	}while((i++ < MAX_recursive_iterations) && (rel_error > recursive_relative_error));

	*eta = 1.0 + 10.0/11.0*parameter/res;
	
	return 0;
}

unsigned char GetSParameter(double *s, double x)
{
	double res;
	double res_old = 0.0;
	unsigned int i = 0;
	double rel_error;
	do
	{
		res += (2.0*i + 10.0)/(2.0*i + 9) * pow(x,i);
		rel_error = ssd_Absolute_lf(1.0 - res_old/res);
		res_old = res;
	}while((i++ < MAX_S_iterations) && (rel_error > S_relative_error));

	*s = res*(6.0*8.0)/(5.0*7.0)*ssd_cube_lf(x);

	return 0;
}

unsigned char GetEtaValue(double *eta,				\
			  double tau,				\
			  double r0, double r1,			\
			  double x0, double y0, double z0,	\
			  double x1, double y1, double z1)
{
	double b;
	double h;
	double m;
	double l;
	double hi;
	double ksi;
	double x;
	double s;
	double eta_int_old;
	double eta_int;
	unsigned int i = 0;
	double rel_error;

	hi = sqrt(2.0*( r0*r1 + x0*x1 + y0*y1 + z0*z1 ));
	l = (r0 + r1)/(2.0*hi) - 0.5;
	m = ssd_sqr_lf(tau)/ssd_cube_lf(hi);
	ksi = 0;
	h = m/(5.0/6.0 + l);
	b = 11.0/9.0*h;

	GetRecursiveEta(&eta_int_old, b);

	do
	{
		x = m/ssd_sqr_lf(eta_int_old) - l;
		GetSParameter(&s, x);
		ksi = (2.0/35.0*ssd_sqr_lf(x) + 48.0/35.0*ssd_cube_lf(x) + s*(x - 5.0/6.0))/ \
		(1.0 + 6.0/5.0*x + 48.0/35.0*ssd_sqr_lf(x) + s);
		h = m/(5.0/6.0 + l + ksi);
		b = 11.0/9.0*h;
		GetRecursiveEta(&eta_int, b);
		rel_error = ssd_Absolute_lf(1.0 - eta_int_old/eta_int);
		eta_int_old = eta_int;
	}while((i++ < MAX_Eta_iterations) && (rel_error > Eta_relative_error));

	*eta = eta_int;

	return 0;
}

unsigned char GetEclipticAngle(double T, double *epsilon)
{
	*epsilon = 0.409319754 - 2.271109689e-4*T - 2.860400738e-8*ssd_sqr_lf(T) + 8.775127667e-9*ssd_cube_lf(T);
	return 0;
}

unsigned char Get_orbit(observation obs1, observation obs0, observation obs2, orbit *Ephemeris, FILE *outputfile)
{
	// а)
	double lambda1, mu1, ni1;
	double lambda0, mu0, ni0;
	double lambda2, mu2, ni2;
	
	fprintf(outputfile, "=========================New calculation=========================\n");
	fprintf(outputfile, "Obseravation	t1\tt0\tt2\n");
	fprintf(outputfile, "JD			%.10lf \t %.10lf \t %.10lf\n", obs1.JD, obs0.JD, obs2.JD);
	fprintf(outputfile, " RA[deg]		%.10lf \t %.10lf \t %.10lf\n",  obs1.RA*180.0/M_PI,  obs0.RA*180.0/M_PI,  obs2.RA*180.0/M_PI);
	fprintf(outputfile, "Dec[deg]		%.10lf \t %.10lf \t %.10lf\n", obs1.Dec*180.0/M_PI, obs0.Dec*180.0/M_PI, obs2.Dec*180.0/M_PI);
	fprintf(outputfile, " RA[rad]		%.10lf \t %.10lf \t %.10lf\n",  obs1.RA,  obs0.RA,  obs2.RA);
	fprintf(outputfile, "Dec[rad]		%.10lf \t %.10lf \t %.10lf\n", obs1.Dec, obs0.Dec, obs2.Dec);

	fprintf(outputfile, "cos_RA		%.10lf \t %.10lf \t %.10lf\n", cos(obs1.RA), cos(obs0.RA), cos(obs2.RA));
	fprintf(outputfile, "sin_RA		%.10lf \t %.10lf \t %.10lf\n", sin(obs1.RA), sin(obs0.RA), sin(obs2.RA));
	fprintf(outputfile, "cos_Dec		%.10lf \t %.10lf \t %.10lf\n", cos(obs1.Dec), cos(obs0.Dec), cos(obs2.Dec));



	lambda1	= cos(obs1.Dec)*cos(obs1.RA);
	mu1	= cos(obs1.Dec)*sin(obs1.RA);
	ni1	= sin(obs1.Dec);

	lambda0	= cos(obs0.Dec)*cos(obs0.RA);
	mu0	= cos(obs0.Dec)*sin(obs0.RA);
	ni0	= sin(obs0.Dec);

	lambda2	= cos(obs2.Dec)*cos(obs2.RA);
	mu2	= cos(obs2.Dec)*sin(obs2.RA);
	ni2	= sin(obs2.Dec);

	fprintf(outputfile, "lambda		%.10lf \t %.10lf \t %.10lf\n", lambda1, lambda0, lambda2);
	fprintf(outputfile, "mu			%.10lf \t %.10lf \t %.10lf\n", mu1, mu0, mu2);
	fprintf(outputfile, "ni			%.10lf \t %.10lf \t %.10lf\n", ni1, ni0, ni2);

	//XXX!!!FIXME!!!TODO!!!
	double X1, Y1, Z1;
	double X0, Y0, Z0;
	double X2, Y2, Z2;
	//GetSunGeocentricEquatorialRectangularCoordinates(obs1.JD, &X1, &Y1, &Z1);
	//GetSunGeocentricEquatorialRectangularCoordinates(obs0.JD, &X0, &Y0, &Z0);
	//GetSunGeocentricEquatorialRectangularCoordinates(obs2.JD, &X2, &Y2, &Z2);

	X1 = +0.259587; X0 = +0.008504; X2 = -0.258673;
	Y1 = +0.900143; Y0 = +0.932409; Y2 = +0.902079;
	Z1 = +0.390383; Z0 = +0.404379; Z2 = +0.391223;
	//XXX!!!FIXME!!!TODO!!!

	fprintf(outputfile, "X			%.10lf \t %.10lf \t %.10lf\n", X1, X0, X2);
	fprintf(outputfile, "Y			%.10lf \t %.10lf \t %.10lf\n", Y1, Y0, Y2);
	fprintf(outputfile, "Z			%.10lf \t %.10lf \t %.10lf\n", Z1, Z0, Z2);
	
	fprintf(outputfile, "l^2+m^2+n^2	%.10lf \t %.10lf \t %.10lf\n", ssd_sqr_lf(lambda1) + ssd_sqr_lf(mu1) + ssd_sqr_lf(ni1), \
									       ssd_sqr_lf(lambda0) + ssd_sqr_lf(mu0) + ssd_sqr_lf(ni0), \
									       ssd_sqr_lf(lambda2) + ssd_sqr_lf(mu2) + ssd_sqr_lf(ni2));

	double R0_sqr = ssd_sqr_lf(X0) + ssd_sqr_lf(Y0) + ssd_sqr_lf(Z0);
	double C = -(lambda0*X0 + mu0*Y0 + ni0*Z0);

	double  D = Determinant3x3(lambda0, lambda1, lambda2, \
				       mu0,     mu1,     mu2, \
				       ni0,     ni1,     ni2);
	double U1 = Determinant3x3(     X1, lambda1, lambda2, \
				        Y1,     mu1,     mu2, \
				        Z1,     ni1,     ni2);
	double U0 = Determinant3x3(     X0, lambda1, lambda2, \
				        Y0,     mu1,     mu2, \
				        Z0,     ni1,     ni2);
	double U2 = Determinant3x3(     X2, lambda1, lambda2, \
				        Y2,     mu1,     mu2, \
				        Z2,     ni1,     ni2);
	double L = lambda1 + lambda0 + lambda2 + X1 + X0 + X2;
	double M = mu1 + mu0 + mu2 + Y1 + Y0 + Y2;
	double N = ni1 + ni0 + ni2 + Z1 + Z0 + Z2;

	double M1 = Determinant3x3(L, lambda1, lambda2, \
				M,     mu1,     mu2, \
				N,     ni1,     ni2);
	double M2 = D + U1 + U0 + U2;

	fprintf(outputfile, "R0^2		%.10lf\n", R0_sqr);
	fprintf(outputfile, "C			%.10lf\n", C);
	fprintf(outputfile, "D			%.10lf\n", D);
	fprintf(outputfile, "U			%.10lf \t %.10lf \t %.10lf\n", U1, U0, U2);
	fprintf(outputfile, "L			%.10lf\n", L);
	fprintf(outputfile, "M			%.10lf\n", M);
	fprintf(outputfile, "N			%.10lf\n", N);
	fprintf(outputfile, "control:W		%.10lf = %.10lf\n", M1, M2);

	// б)
	double tau1 = Gauss_const*(obs2.JD - obs0.JD);
	double tau2 = Gauss_const*(obs0.JD - obs1.JD);
	double tau0 = Gauss_const*(obs2.JD - obs1.JD);

	double n1_0 = tau1/tau0;
	double n2_0 = tau2/tau0;
	double A1 = tau1*tau2*(1.0 + n1_0)/6.0;
	double A2 = tau1*tau2*(1.0 + n2_0)/6.0;
	
	fprintf(outputfile, "tau		%.10lf \t %.10lf \t %.10lf\n", tau1, tau0, tau2);
	fprintf(outputfile, "n1_0		%.10lf\n", n1_0);
	fprintf(outputfile, "n2_0		%.10lf\n", n2_0);
	fprintf(outputfile, "A1			%.10lf\n", A1);
	fprintf(outputfile, "A2			%.10lf\n", A2);

	// в)
	double rho0, r0;
	double B1 = (7.0*ssd_sqr_lf(tau0) - 3.0*ssd_sqr_lf(tau1))/60.0;
	double B2 = (7.0*ssd_sqr_lf(tau0) - 3.0*ssd_sqr_lf(tau2))/60.0;
	double P = (U0 - n1_0*U1 - n2_0*U2)/D;
	double Q = (A1*U1 + A2*U2)/D;
	double Ssqr = R0_sqr - C*C;
	double Qprim = (A1*B1*U1 + A2*B2*U2)/D;
	GetObjectGeoAndGelioCentricDistance(P, Q, Qprim, C, Ssqr, &rho0, &r0);

	fprintf(outputfile, "B1			%.10lf\n", B1);
	fprintf(outputfile, "B2			%.10lf\n", B2);
	fprintf(outputfile, "P			%.10lf\n", P);
	fprintf(outputfile, "Q			%.10lf\n", Q);
	fprintf(outputfile, "S^2		%.10lf\n", Ssqr);
	fprintf(outputfile, "Qprim		%.10lf\n", Qprim);
	fprintf(outputfile, "rho0		%.10lf\n", rho0);
	fprintf(outputfile, "r0			%.10lf\n", r0);

	// г)
	double n1 = n1_0 + A1/(ssd_cube_lf(r0))*(1 + B1*ssd_cube_lf(r0));
	double n2 = n2_0 + A2/(ssd_cube_lf(r0))*(1 + B2*ssd_cube_lf(r0));

	fprintf(outputfile, "n1			%.10lf\n", n1);
	fprintf(outputfile, "n2			%.10lf\n", n2);
	
	double det_system_1;
	double det_system_2;
	double det_system_3;

	det_system_1 = Determinant2x2(n1*lambda1, n2*lambda2, \
				          n1*mu1,     n2*mu2);
	det_system_2 = Determinant2x2(n1*lambda1, n2*lambda2, \
				          n1*ni1,     n2*ni2);
	det_system_3 = Determinant2x2(n1*mu1, n2*mu2, \
				      n1*ni1, n2*ni2);

	double a,b,c,d,e,f;
	if((det_system_1 >= det_system_2) && (det_system_1 >= det_system_3))
	{
		a = n1*lambda1; b = n2*lambda2; c = rho0*lambda0 + n1*X1 - X0 + n2*X2;
		d = n1*mu1;     e = n2*mu2;     f = rho0*mu0 + n1*Y1 - Y0 + n2*Y2;
	}
	else if((det_system_2 >= det_system_1) && (det_system_2 >= det_system_3))
	{
		a = n1*lambda1; b = n2*lambda2; c = rho0*lambda0 + n1*X1 - X0 + n2*X2;
		d = n1*ni1;     e = n2*ni2;     f = rho0*ni0 + n1*Z1 - Z0 + n2*Z2;
	}
	else
	{
		a = n1*mu1; b = n2*mu2; c = rho0*mu0 + n1*Y1 - Y0 + n2*Y2;
		d = n1*ni1; e = n2*ni2; f = rho0*ni0 + n1*Z1 - Z0 + n2*Z2;
	}

	fprintf(outputfile,"%.10lf*rho1 + %.10lf*rho2 = %0.10lf\n", a, b, c);
	fprintf(outputfile,"%.10lf*rho1 + %.10lf*rho2 = %0.10lf\n", d, e, f);

	double rho1, rho2;
	if(a != 0.0)
	{
		rho2 = (a*f - c*d)/(a*e - b*d);
	}
	else if(d != 0.0)
	{
		rho2 = (c*d - a*f)/(b*d - a*e);
	}
	else
	{
		;
	}

	if(b != 0.0)
	{
		rho1 = (b*f - c*e)/(b*d - a*e);
	}
	else if(e != 0.0)
	{
		rho1 = (c*e - b*f)/(a*e - b*d);
	}
	else
	{
		;
	}

	fprintf(outputfile, "rho1		%.10lf\n", rho1);
	fprintf(outputfile, "rho2		%.10lf\n", rho2);
	// д)
	double x1 = lambda1*rho1 - X1;
	double x0 = lambda0*rho0 - X0;
	double x2 = lambda2*rho2 - X2;

	double y1 = mu1*rho1 - Y1;
	double y0 = mu0*rho0 - Y0;
	double y2 = mu2*rho2 - Y2;
	
	double z1 = ni1*rho1 - Z0;
	double z0 = ni0*rho0 - Z1;
	double z2 = ni2*rho2 - Z2;

	double r1 = sqrt(ssd_sqr_lf(x1) + ssd_sqr_lf(y1) + ssd_sqr_lf(z1));
	       r0 = sqrt(ssd_sqr_lf(x0) + ssd_sqr_lf(y0) + ssd_sqr_lf(z0));
	double r2 = sqrt(ssd_sqr_lf(x2) + ssd_sqr_lf(y2) + ssd_sqr_lf(z2));

	fprintf(outputfile, "x			%.10lf \t %.10lf \t %.10lf\n", x1, x0, x2);
	fprintf(outputfile, "y			%.10lf \t %.10lf \t %.10lf\n", y1, y0, y2);
	fprintf(outputfile, "z			%.10lf \t %.10lf \t %.10lf\n", z1, z0, z2);
	fprintf(outputfile, "r			%.10lf \t %.10lf \t %.10lf\n", r1, r0, r2);
	// е)
	double t1_0 = obs1.JD - 0.0057756 * rho1;
	double t0_0 = obs0.JD - 0.0057756 * rho0;
	double t2_0 = obs2.JD - 0.0057756 * rho2;

	fprintf(outputfile, "t			%.10lf \t %.10lf \t %.10lf\n", t1_0, t0_0, t2_0);

	tau1 = Gauss_const*(t2_0 - t0_0);
	tau2 = Gauss_const*(t0_0 - t1_0);
	tau0 = Gauss_const*(t2_0 - t1_0);

	fprintf(outputfile, "tau		%.10lf \t %.10lf \t %.10lf\n", tau1, tau0, tau2);

	n1_0 = tau1/tau0;
	n2_0 = tau2/tau0;

	fprintf(outputfile, "n1_0		%.10lf\n", n1_0);
	fprintf(outputfile, "n2_0		%.10lf\n", n2_0);

	double eta1, eta0, eta2;

	GetEtaValue(&eta1, tau1, r0, r2, x0, y0, z0, x2, y2, z2);
	GetEtaValue(&eta0, tau0, r1, r2, x1, y1, z1, x2, y2, z2);
	GetEtaValue(&eta2, tau2, r1, r0, x1, y1, z1, x0, y0, z0);

	fprintf(outputfile, "eta		%.10lf \t %.10lf \t %.10lf\n", eta1, eta0, eta2);

	n1 = n1_0*eta0/eta1;
	n2 = n2_0*eta0/eta2;

	fprintf(outputfile, "n1			%.10lf\n", n1);
	fprintf(outputfile, "n2			%.10lf\n", n2);

	double g,h,i,j,k,l;
	double Det, Det_rho0, Det_rho1, Det_rho2;

	a = -lambda0; b = n1*lambda1; c = n2*lambda2; d = n1*X1 - X0 + n2*X2;
	e = -mu0; f = n1*mu1; g = n2*mu2; h = n1*Y1 - Y0 + n2*Y2;
	i = -ni0; j = n1*ni1; k = n2*ni2; l = n1*Z1 - Z0 + n2*Z2;

	Det = Determinant3x3(a, b, c, \
			     e, f, g, \
			     i, j, k);
	Det_rho0 = Determinant3x3(d, b, c, \
				  h, f, g, \
				  e, j, k);
	Det_rho1 = Determinant3x3(a, d, c, \
				  e, h, g, \
				  i, e, k);
	Det_rho2 = Determinant3x3(a, b, d, \
				  e, f, h, \
				  i, j, e);
	if(Det != 0.0)
	{
		rho1 = Det_rho1/Det;
		rho0 = Det_rho0/Det;
		rho2 = Det_rho2/Det;
	}

	fprintf(outputfile, "Det		%.10lf\n", Det);
	fprintf(outputfile, "Det_rho1		%.10lf\n", Det_rho1);
	fprintf(outputfile, "Det_rho0		%.10lf\n", Det_rho0);
	fprintf(outputfile, "Det_rho2		%.10lf\n", Det_rho2);

	fprintf(outputfile,"%.10lf*rho0 + %.10lf*rho1 + %.10lf*rho2 = %0.10lf\n", a, b, c, d);
	fprintf(outputfile,"%.10lf*rho0 + %.10lf*rho1 + %.10lf*rho2 = %0.10lf\n", e, f, g, h);
	fprintf(outputfile,"%.10lf*rho0 + %.10lf*rho1 + %.10lf*rho2 = %0.10lf\n", i, j, k, k);
	fprintf(outputfile, "rho		%.10lf \t %.10lf \t %.10lf\n", rho1, rho0, rho2);

	x1 = lambda1*rho1 - X1;
	x0 = lambda0*rho0 - X0;
	x2 = lambda2*rho2 - X2;
	
	y1 = mu1*rho1 - Y1;
	y0 = mu0*rho0 - Y0;
	y2 = mu2*rho2 - Y2;
	
	z1 = ni1*rho1 - Z0;
	z0 = ni0*rho0 - Z1;
	z2 = ni2*rho2 - Z2;

	r1 = sqrt(ssd_sqr_lf(x1) + ssd_sqr_lf(y1) + ssd_sqr_lf(z1));
	r0 = sqrt(ssd_sqr_lf(x0) + ssd_sqr_lf(y0) + ssd_sqr_lf(z0));
	r2 = sqrt(ssd_sqr_lf(x2) + ssd_sqr_lf(y2) + ssd_sqr_lf(z2));


	double B = ssd_sqr_lf(y1*z2 - y2*z1) + ssd_sqr_lf(z1*x2 - z2*x1) + ssd_sqr_lf(x1*y2 - x2*y1);
	double p = ssd_sqr_lf(B*eta0/tau0);
	double f1 = p - r1;
	double f2 = p - r2;
	double I = x1*x2 + y1*y2 + z1*z2;
	double g1 = (f1*I - f2*ssd_sqr_lf(r1))/B;
	double g2 = (f1*ssd_sqr_lf(r2) - f2*I)/B;
	double e1 = sqrt( (ssd_sqr_lf(f1) + ssd_sqr_lf(g1))/ssd_sqr_lf(r1) );
	double e2 = sqrt( (ssd_sqr_lf(f2) + ssd_sqr_lf(g2))/ssd_sqr_lf(r2) );

	e = e1;	//FIXME

	a = p/(1.0 - ssd_sqr_lf(e));

	printf("p = %lf\n");

	double a_r1_e = (a - r1)*sqrt(1.0 - ssd_sqr_lf(e));
	double a_r2_e = (a - r2)*sqrt(1.0 - ssd_sqr_lf(e));
	double tg_E1 = g1/( a_r1_e );
	double tg_E2 = g2/( a_r2_e );
	double E1, E2;

	if( a_r1_e >= 0.0 )
	{
		E1 = atan(tg_E1);
	}
	else
	{
		E1 = atan(tg_E1) + M_PI;
	}

	if( a_r2_e >= 0.0 )
	{
		E2 = atan(tg_E2);
	}
	else
	{
		E2 = atan(tg_E2) + M_PI;
	}

	M1 = E1 - e*sin(E1);
	M2 = E2 - e*sin(E2);

	n1 = (M2 - M1)/(t2_0 - t1_0);
	n2 = Gauss_const/ssd_cube_lf(sqrt(a));

	double n = n1;	//FIXME

	double Kx = (ssd_sqr_lf(r1) - x1*I)/B;
	double Ky = (ssd_sqr_lf(r1) - y1*I)/B;
	double Kz = (ssd_sqr_lf(r1) - z1*I)/B;
	       C = f1/ssd_sqr_lf(r1)/e;
	double S = g1/ssd_sqr_lf(r1)/e;
	double Px = x1*C - Kx*S;
	double Py = y1*C - Ky*S;
	double Pz = z1*C - Kz*S;
	double Qx = x1*S - Kx*C;
	double Qy = x1*S - Ky*C;
	double Qz = x1*S - Kz*C;

	double Rx = (y1*z1 - y2*z1)/B;
	double Ry = (z1*x2 - z2*z1)/B;
	double Rz = (x1*y2 - x2*y1)/B;

	double epsilon;
	double T = (t0_0 - 2415020.0)/36525.0;
	GetEclipticAngle(T, &epsilon);

	double sin_epsilon = sin(epsilon);
	double cos_epsilon = cos(epsilon);

	double Rx_bar = Rx;
	double Ry_bar = Ry*cos_epsilon + Rz*sin_epsilon;
	double Rz_bar = Rz*cos_epsilon - Ry*sin_epsilon;
	       i = acos(Rz_bar);
	double sin_i = sin(i);
	double tan_Omega_big = -Rx_bar/Ry_bar;
	double Omega_big;

	if( (sin_i >= 0.0 && -Ry_bar >= 0.0) || (sin_i < 0.0 && -Ry_bar < 0.0) )
	{
		Omega_big = atan( tan_Omega_big );
	}
	else
	{
		Omega_big = atan( tan_Omega_big ) + M_PI;
	}

	double tan_omega_small = (Pz*cos_epsilon - Py*sin_epsilon) / (Qz*cos_epsilon - Qy*sin_epsilon);
	double omega_small;
	double QzQy = Qz*cos_epsilon - Qy*sin_epsilon;

	if( (sin_i >= 0.0 && QzQy >= 0.0) || (sin_i < 0.0 && QzQy < 0.0) )
	{
		omega_small = atan( tan_omega_small );
	}
	else
	{
		omega_small = atan( tan_omega_small ) + M_PI;
	}

	(*Ephemeris).SemiMajorAxis = a;
	(*Ephemeris).Eccentricity = e;
	(*Ephemeris).Inclination = i;


}



















