// Header file contains math function definitions used in
// Cooley-Tookey FFT algorithm and Discrete Fourier transformation
//
// Dmitry 2019 (c)

#ifndef _FFT_MATH_
#define _FFT_MATH_




#define PI_2 6.2831853071795864769252867665590
#define TRUE 1
#define FALSE 0

typedef struct		// structure to hold complex number in algebraic form
{
	double Re;
	double Im;
}complex_alg;		// z = Re + Im * i

typedef struct		// structure to hold complex number in exponential form
{
	double Mag;
	double Phi;
}complex_exp;		// z = A*exp(i*phi)   A = Mag;   phi = Phi


complex_alg ComplexAlgSet(double Re, double Im);					// creates and returns struct "Re + Im * i"
complex_exp ComplexExpSet(double Mag, double Phi);					// creates and returns struct "A*exp(i*phi)"

complex_alg ComplexExpToComplexAlg(complex_exp z);					// A*exp(i*phi) -> Re + Im * i
complex_exp ComplexAlgToComplexExp(complex_alg z);					// Re + Im * i  -> A*exp(i*phi)

complex_alg ComplexAlgSum(complex_alg A, complex_alg B);			// A + B
complex_alg ComplexAlgSub(complex_alg A, complex_alg B);			// A - B
complex_alg ComplexAlgMul(complex_alg A, complex_alg B);			// A * B
complex_alg ComplexAlgDiv(complex_alg A, complex_alg B);			// A / B

complex_alg ComplexAlgMulComplexExp(complex_alg A, complex_exp B);	// (Re + Im * i) * ( A*exp(i*phi) )

complex_alg ComplexAlgMulByConst(complex_alg A, double C);			// C*A -> C*Re + C*Im *i

double ComplexAlgMag2(complex_alg z);								// returns Re^2 + Im^2
double ComplexAlgMag(complex_alg z);								// returns sqrt( Re^2 + Im^2 )
double ValueAbsolute(double num);									// returns |num|, absolute value









#endif
