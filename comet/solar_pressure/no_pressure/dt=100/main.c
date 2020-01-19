#include <math.h>
#include <stdio.h>
#include <time.h>


#define dt0 100			// секунд
#define max_time 100		// количество Юпитерианских лет; 1 год Юпитера = 11.86 Земных лет
#define  G 6.672e-11		// гравитационная постоянная
#define au 1.495978707e+11	// астрономическая единица
#define  M 1.98911e+30		// масса солнца
#define Jup_year 3.7439461e+7	// юпитерианский год в секундах/10
#define P_0 3.84e26		// мощность излучения Солнца

#define file "output.dat"	// выходной файл
#define file2 "jupiter.dat"

void calculate_orbit_param(double x, double y, double vx, double vy, double *a, double *e);
double ssd_sqrt(double x); // квадратный корень
double ssd_abs(double x);  // абсолютное значение
double sqr(double x);
double cube(double x);
double radius(double x,double y);
double non_grav_x(double x, double y, double vx, double vy, double R);
double non_grav_y(double x, double y, double vx, double vy, double R);
double new_dt(double c[4][7]);


int main(void)
{
				//     x             y     vx     vy          R
	double   comet[4][7] = { { 35.0823105*au, 0.0*au, 0.0,  911.5178, 0.0	},	// r =    100 m
				 { 35.0823105*au, 0.0*au, 0.0,  911.5178, 0.0	},	// r =    500 m
				 { 35.0823105*au, 0.0*au, 0.0,  911.5178, 0.0	},	// r =  1'000 m
				 { 35.0823105*au, 0.0*au, 0.0,  911.5178, 0.0	} };	// r = 15'000 m
	double    jupiter[5] =   {-4.95155843*au, 0.0*au, 0.0,-13705.212, 1.8986e+27 };	
	double orb_param[4][2];
	double jup_orb_param[2];
	double x;
	double ax,ay;
	double dt;
	int time=0;
	int i;
	time_t a,b;


	FILE *f = fopen(file, "w");
	if(f == NULL)
	{
		printf("Невозможно открыть или записать в файл\n");
		return -1;
	}
	
	FILE *g = fopen(file2, "w");
	if(g == NULL)
	{
		printf("Невозможно открыть или записать в файл\n");
		return -1;
	}
	
	
	
	for(i=0;i<4; i++)
	{
		calculate_orbit_param( comet[i][0], comet[i][1], comet[i][2], comet[i][3], &orb_param[i][0], &orb_param[i][1]);
	}
	fprintf(f,"%d %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf\n",time,\
	orb_param[0][0]/au,orb_param[0][1],radius(jupiter[0]-comet[0][0],jupiter[1]-comet[0][1])/au,\
	orb_param[1][0]/au,orb_param[1][1],radius(jupiter[0]-comet[1][0],jupiter[1]-comet[1][1])/au,\
	orb_param[2][0]/au,orb_param[2][1],radius(jupiter[0]-comet[2][0],jupiter[1]-comet[2][1])/au,\
	orb_param[3][0]/au,orb_param[3][1],radius(jupiter[0]-comet[3][0],jupiter[1]-comet[3][1])/au);
	
	calculate_orbit_param( jupiter[0], jupiter[1], jupiter[2], jupiter[3], &jup_orb_param[0], &jup_orb_param[1] );
	fprintf(g,"%d %.8lf %.8lf\n", time, jup_orb_param[0]/au, jup_orb_param[1]);
	fflush(f);
	fflush(g);
	
	
	dt = dt0*new_dt(comet);
	
	a=clock();
	
	for(time=0; time<max_time; time++)
	{
		for(x=0.0; x<= Jup_year; x+= dt)
		{
			
			for(i=0; i<4; i++)
			{
				ax=-G*M*comet[i][0]/cube(radius(comet[i][0],comet[i][1]));
				ay=-G*M*comet[i][1]/cube(radius(comet[i][0],comet[i][1]));
				
				ax += non_grav_x(comet[i][0],comet[i][1],comet[i][2],comet[i][3],comet[i][4]);
				ay += non_grav_y(comet[i][0],comet[i][1],comet[i][2],comet[i][3],comet[i][4]);
				
				ax += G*jupiter[4]*(jupiter[0]-comet[i][0])/cube(radius(jupiter[0]-comet[i][0],jupiter[1]-comet[i][1]));
				ay += G*jupiter[4]*(jupiter[1]-comet[i][1])/cube(radius(jupiter[0]-comet[i][0],jupiter[1]-comet[i][1]));
				
				comet[i][0]+=comet[i][2]*dt;
				comet[i][1]+=comet[i][3]*dt;
				
				comet[i][2]+=ax*dt;
				comet[i][3]+=ay*dt;
				
				
				ax=-G*M*jupiter[0]/cube(radius(jupiter[0],jupiter[1]));
				ay=-G*M*jupiter[1]/cube(radius(jupiter[0],jupiter[1]));
				jupiter[0]+=jupiter[2]*dt;
				jupiter[1]+=jupiter[3]*dt;
				jupiter[2]+=ax*dt;
				jupiter[3]+=ay*dt;
				
			}
			dt = dt0*new_dt(comet);
			//printf("%lf %lf\n", dt, x);
		
			
		}
		printf("%d\n", time+1);
		
		for(i=0;i<4; i++)
		{
			calculate_orbit_param( comet[i][0], comet[i][1], comet[i][2], comet[i][3], &orb_param[i][0], &orb_param[i][1]);
		}
		fprintf(f,"%d %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf %.8lf\n",time,\
		orb_param[0][0]/au,orb_param[0][1],radius(jupiter[0]-comet[0][0],jupiter[1]-comet[0][1])/au,\
		orb_param[1][0]/au,orb_param[1][1],radius(jupiter[0]-comet[1][0],jupiter[1]-comet[1][1])/au,\
		orb_param[2][0]/au,orb_param[2][1],radius(jupiter[0]-comet[2][0],jupiter[1]-comet[2][1])/au,\
		orb_param[3][0]/au,orb_param[3][1],radius(jupiter[0]-comet[3][0],jupiter[1]-comet[3][1])/au);
		
		calculate_orbit_param( jupiter[0], jupiter[1], jupiter[2], jupiter[3], &jup_orb_param[0], &jup_orb_param[1] );
		fprintf(g,"%d %.8lf %.8lf\n", time, jup_orb_param[0]/au, jup_orb_param[1]);

		
		fflush(f);
		fflush(g);
		
	}
	
	b=clock();
	
	printf("\n%lf\n", (double)(b-a)/CLOCKS_PER_SEC);
	
	
	
	fclose(f);
	
	return 0;
}


void calculate_orbit_param(double x, double y, double vx, double vy, double *a, double *e)
{
	double        R = ssd_sqrt(x*x + y*y);
	double vsquared = vx*vx + vy*vy;
	double        h = (x*vy - y*vx);
	
	*a = 1/( 2/R - vsquared/G/M );
	*e = ssd_sqrt( 1 - h*h/G/M/(*a) );
}

double ssd_abs(double x)
{
	if (x<0)
	{
		return -(x);
	}
	else return x;
}

double ssd_sqrt(double x) // квадратный корень
{
	double x1,x0=x;

	x1=(x0+x/x0)/2;

	while( ssd_abs(x1-x0)>(1e-37) )
	{
		x0=x1;
		x1=(x0+x/x0)/2;
	}

	return x1;

}

double sqr(double x)
{
	return x*x;
}

double cube(double x)
{
	return x*x*x;
}

double radius(double x,double y)
{
	return ssd_sqrt(sqr(x)+sqr(y));
}

double non_grav_x(double x, double y, double vx, double vy, double R)
{
	double r = radius(x,y);
	double v = radius(vx,vy);
	double ax;
	
	ax  = R/cube(r)*x;
	
	return ax;
	
}

double non_grav_y(double x, double y, double vx, double vy, double R)
{
	double r = radius(x,y);
	double v = radius(vx,vy);
	double ay;
	
	ay  = R/cube(r)*y;
	
	return ay;
	
}

double new_dt(double c[4][7])
{
	double maxv = ssd_sqrt(sqr(c[0][2])+sqr(c[0][3]));
	int i=0,j=0;
	
	for(j=1; j<4; j++)
	{
		if( ssd_sqrt(sqr(c[j][2])+sqr(c[j][3])) > maxv )
		{
			maxv = ssd_sqrt(sqr(c[j][2])+sqr(c[j][3]));
			i = j;
		}
	}
	//printf("%E\n", (double)dt0/sqr(ssd_sqrt(sqr(c[i][2])+sqr(c[i][3]))/1000.0) );
	return dt0/sqr( ssd_sqrt(sqr(c[i][2])+sqr(c[i][3]))/1000.0 );

	
}























