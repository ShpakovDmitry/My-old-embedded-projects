#include <stdio.h>

#define  AU 1.495978707e+11
#define   G 6.67256e-11
#define   M 1.98911e+30
#define  dt 1
#define eps 1e+1

#define  a 17.8341443
#define  e 0.9671429
#define  C 4.241e+27
//#define  C 3.67e+27
//#define  C 1.077e+28
//#define  C 4.25e+27
//#define  C 1.34e+28


double absolute(double x);
double sqrt(double x);
double sqr(double x);
double cube(double x);
double radius(double x,double y);


int main(void)
{
	double x=a*AU*(1-e),y=0,vx=0,vy=sqrt( G*M/a/AU*(1+e)/(1-e) ),ax=0,ay=0,m=0,time=0;
	char flag=0;
	long int q=0;
	
	//printf("%lf\n",vy);
	
	while(flag != 1)
	{
		
		ax=-G*M*x/cube(sqrt(sqr(x)+sqr(y)));
		ay=-G*M*y/cube(sqrt(sqr(x)+sqr(y)));
		
		x+=vx*dt;
		y+=vy*dt;
		
		vx+=ax*dt;
		vy+=ay*dt;
		
		m+=C/(sqr(x)+sqr(y))*dt;
		
		time+=dt;
		
		/*if( (radius(x,y)!=a*AU) && (flag==0) )
		{
			flag=2;
			printf("OK\n");
		}
		if( (flag==2) && (radius(x,y)<a*AU-eps) )
		{
			flag=3;
			printf("OK2\n");
		}
		
		if( ( radius(x,y)>=a*AU-eps && radius(x,y)<=a*AU+eps  ) && flag==3)
		{
			flag=1;
		}*/
		
		/*if((long int)time/86400/365==q)
		{
			printf("%3.5lf  %3.5lf %3.5lf\n",time/86400/365,radius(x,y)/AU,m);
			q++;
			if(q>=79)
			{
				flag=1;
			}
		}*/
		
		if(radius(x,y)>=4.5*AU)
		{
			flag=1;
		}
		
		
	}
	
	printf("    radius: %lf\n",radius(x,y)/AU);
	printf("totat time: %lf\n",time/86400);
	printf("        dm: %lf\n",m);

	return 0;
	
	
}





//****************************************************************************************************************************************************
double absolute(double x)
{
	if (x<0)
	{
		return -(x);
	}
	else return x;
}

double sqrt(double x)
{
	double x1,x0=x;

	x1=(x0+x/x0)/2;

	while( absolute(x1-x0)>(1e-37) )
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
	return sqrt(sqr(x)+sqr(y));
}




