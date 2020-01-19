#include <math.h>
#include <stdio.h>


#define dt0 400			// секунд
#define  G 6.672e-11		// гравитационная постоянная
#define au 1.495978707e+11	// астрономическая единица
#define  M 1.98911e+30		// масса солнца
#define Jup_year 3.7439461e+7	// юпитерианский год в секундах/10

#define file "output.dat"	// выходной файл

void calculate_orbit_param(double x, double y, double vx, double vy, double *a, double *e); // нахождение орбиты по двумерным координатам и скоростям
double ssd_sqrt(double x); // квадратный корень
double ssd_abs(double x);  // абсолютное значение
double sqr(double x); // квадрат числа
double cube(double x); // куб числа
double radius(double x,double y); // длина двумерного вектора
double non_grav_x(double x, double y, double vx, double vy, double R, double T); // проекция негравитационного ускорения на ось x 
double non_grav_y(double x, double y, double vx, double vy, double R, double T); // проекция негравитационного ускорения на ось y
double new_dt(double c[4][7]); // вычисление нового шага интегрирования


int main(void)
{
	// задаём начальные условия для кометы
				//     x             y     vx     vy          m                R                   T
	double   comet[4][7] = { { 35.0823105*au, 0.0*au, 0.0,  911.5178, 5.0000e+15,  0.0*au/sqr(86400.0), 0.0*au/sqr(86400.0)  },
				 { 35.0823105*au, 0.0*au, 0.0,  911.5178, 5.0000e+15, 1e-9*au/sqr(86400.0), 0.0*au/sqr(86400.0)  },
				 { 35.0823105*au, 0.0*au, 0.0,  911.5178, 5.0000e+15,  0.0*au/sqr(86400.0), 1e-9*au/sqr(86400.0) },
				 { 35.0823105*au, 0.0*au, 0.0,  911.5178, 5.0000e+15, 1e-9*au/sqr(86400.0), 1e-9*au/sqr(86400.0) } };
	
	// задаём начальные условия для Юпитера
				//     x             y     vx     vy          m                R                   T
	double    jupiter[5] =   {-4.95155843*au, 0.0*au, 0.0,-13705.212, 1.8986e+27 };	
	double orb_param[4][2];
	//double jup_orb_param[2];
	double x;
	double ax,ay;
	double dt;
	int time=0;
	int i;
	time_t a,b;
	struct tm *aa;
	a=time(&);
	aa=localtime(&a);
	char asd[24],*ccc;
	ccc=asctime(aa);
	for(i=0;i<24;i++)
	{
		asd[i]=*(ccc+i);
	}
	printf("%s\n", asd);
















	FILE *f = fopen(file, "w");
	if(f == NULL)
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
	
	
	dt = dt0*new_dt(comet);
	

	
	for(time=0; time<70; time++)
	{
		for(x=0.0; x<= Jup_year; x+= dt)
		{
			
			for(i=0; i<4; i++)
			{
				ax=-G*M*comet[i][0]/cube(radius(comet[i][0],comet[i][1]));
				ay=-G*M*comet[i][1]/cube(radius(comet[i][0],comet[i][1]));
				
				ax += non_grav_x(comet[i][0],comet[i][1],comet[i][2],comet[i][3],comet[i][5],comet[i][6]);
				ay += non_grav_y(comet[i][0],comet[i][1],comet[i][2],comet[i][3],comet[i][5],comet[i][6]);
				
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
		

		
		fflush(f);
		
	}
	
	
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

double non_grav_x(double x, double y, double vx, double vy, double R, double T)
{
	double r = radius(x,y);
	double v = radius(vx,vy);
	double ax;
	
	ax  = R/cube(r)/ssd_sqrt(ssd_sqrt(r))*x;
	ax += T/sqr(r)/ssd_sqrt(ssd_sqrt(r))*vx/v;
	
	return ax;
	
}

double non_grav_y(double x, double y, double vx, double vy, double R, double T)
{
	double r = radius(x,y);
	double v = radius(vx,vy);
	double ay;
	
	ay  = R/cube(r)/ssd_sqrt(ssd_sqrt(r))*y;
	ay += T/sqr(r)/ssd_sqrt(ssd_sqrt(r))*vy/v;
	
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























