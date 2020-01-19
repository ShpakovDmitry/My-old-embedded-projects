#include <stdio.h>
#include <time.h>

int main(void)
{
	clock_t a,b;
	register int i;
	
	a=clock();
	for(i=0;i<5e+8;i++)
	{}
	b=clock();
	printf("%lf\n", (double)(b-a)/CLOCKS_PER_SEC);
	
	return 0;
}
