#ifndef COMPLEX_H_INCLUDED
#define COMPLEX_H_INCLUDED

#include <stdio.h>

extern double Pi;

class Complex {
private:
	double R;	///Real part
	double I;	///imaginary part
public:
	Complex();
	Complex(double r, double i);
	virtual ~Complex();
	void print(FILE *o=stdout);
	void print(const char *format,FILE *o=stdout);
	void Set(double r, double i){R=r;I=i;}
	double GetReal(void){return R;}
	double GetImaginary(void){return I;}
	double Abs(void);
	Complex operator+(const Complex &a);
	Complex operator-(const Complex &a);
	Complex operator-();
	Complex operator*(const Complex &a);
	Complex operator /(const Complex &a);
	double Mag(void);
	double Phase(double &LastPhase,int &N);
};

#endif // COMPLEX_H_INCLUDED
