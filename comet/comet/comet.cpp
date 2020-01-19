#include <iostream>
#include <math.h>

#include "const.h"		// <--- here are constants declarations
#include "classes.cpp"		// <--- here defined classes: planet,comet
#include "JDTime.cpp"		// <--- procedures dealing with JD and Gregorian date
//#include "functions.cpp"	// <--- here defined needed functions


using namespace std;

int main(void)
{
	planet a(1.8986e+27, 1.0, 0.0, 0.0, 0.0, 30000.0, 0.0);
	
	double x;
	
	x = sqrt(2);
	
	cout << x << endl;
	
	return 0; 
}
