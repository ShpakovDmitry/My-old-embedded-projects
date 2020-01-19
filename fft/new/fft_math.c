// implementation of math functions used in
// Cooley-Tooky FFT algorithm and Discrete Fourier transformation
//
// Dmitry 2019 (c)

#include "fft_math.h"
#include <math.h>

complex_alg ComplexAlgSet(double Re, double Im)						// creates and returns struct "Re + Im * i"
{
	complex_alg z = {Re, Im};
	return z;
}

complex_exp ComplexExpSet(double Mag, double Phi)					// creates and returns struct "A*exp(i*phi)"
{
	complex_exp z = {Mag, Phi};
	return z;
}

complex_alg ComplexExpToComplexAlg(complex_exp z)					// A*exp(i*phi) -> Re + Im * i
{
	complex_alg y = {z.Mag*cos(z.Phi), z.Mag*sin(z.Phi)};
	return y;
}

complex_exp ComplexAlgToComplexExp(complex_alg z)					// Re + Im * i  -> A*exp(i*phi)
{
	complex_exp y = {ComplexAlgMag(z), atan2(z.Im, z.Re)};
	return y;
}

complex_alg ComplexAlgConjugate(complex_alg z)						// Re + Im * i -> Re - Im * i
{
	complex_alg y = {z.Re, -z.Im};
	return y;
}

complex_alg ComplexAlgSum(complex_alg A, complex_alg B)				// A + B
{
	complex_alg z = {A.Re + B.Re, A.Im + B.Im};
	return z;
}

complex_alg ComplexAlgSub(complex_alg A, complex_alg B)				// A - B
{
	complex_alg z = {A.Re - B.Re, A.Im - B.Im};
	return z;
}
complex_alg ComplexAlgMul(complex_alg A, complex_alg B)				// A * B
{
	complex_alg z = {A.Re*B.Re - A.Im*B.Im, A.Re*B.Im + A.Im*B.Re};
	return z;
}
complex_alg ComplexAlgDiv(complex_alg A, complex_alg B)				// A / B,  |B| != 0
{
	return ComplexAlgMul(ComplexAlgMul(A, ComplexAlgConjugate(B)), ComplexAlgSet(1.0/ComplexAlgMag2(B), 0.0) );
}

complex_alg ComplexAlgMulComplexExp(complex_alg A, complex_exp B)	// (Re + Im * i) * ( A*exp(i*phi) )
{
	return ComplexAlgMul(A, ComplexExpToComplexAlg(B));
}

complex_alg ComplexAlgMulByConst(complex_alg A, double C)			// C*A -> C*Re + C*Im *i		C e R
{
	return ComplexAlgMul(A, ComplexAlgSet(C, 0.0));
}

double ComplexAlgMag2(complex_alg z)
{
	return z.Re*z.Re + z.Im*z.Im;
}

double ComplexAlgMag(complex_alg z)
{
	return sqrt( ComplexAlgMag2(z) );
}

double ValueAbsolute(double num)
{
	if(num < 0.0)
		return (-num);
	return num;
}

