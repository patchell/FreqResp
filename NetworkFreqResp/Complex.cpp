#include <stdio.h>
#include <math.h>
#include "Complex.h"

double Pi=0.0;

Complex::Complex()
{
	if(Pi == 0.0) Pi = 4.0 * atan(1);
	R = 0.0;
	I = 0.0;
}

Complex::Complex(double r, double i)
{
	if(Pi == 0.0) Pi = 4.0 * atan(1);
	R = r;
	I = i;
}

Complex::~Complex()
{

}

void Complex::print(FILE *o)
{
	fprintf(o,"%lf+j%lf\n",R,I);
}

void Complex::print(const char *format,FILE *o)
{
	fprintf(o,format,R,I);
}

double Complex::Abs(void)
{
    ///------------------------------
    /// Compute the absolute
    /// value of a complex number
    ///------------------------------
	return sqrt(I*I + R*R);
}

Complex Complex::operator-()
{
    ///------------------------------
    /// operator- (Negate Operator)
    /// compute the negative of a
    /// complex number
    ///------------------------------
	Complex c;
	c.Set(-R,-I);
	return c;
}

Complex Complex::operator+(const Complex &a)
{
    ///-----------------------------------
    /// Operator+ (Add Operator)
    /// Add another complex value to this
    /// value and return it
    ///
    /// parameters:
    ///     a......value to add to this
    ///
    /// returns:
    ///     sum of the complex numbers
    ///------------------------------------
	Complex c;
	c.R = this->R + a.R;
	c.I = this->I + a.I;
	return c;
}

Complex Complex::operator-(const Complex &a)
{
    ///------------------------------------
    /// operator- (Subtract Operator)
    /// Subtract a complex value from this
    /// value.
    ///
    /// parameters:
    ///     a.....value to subtract from this
    ///
    /// returns:
    ///     difference between this and a
    ///--------------------------------------
	Complex c;
	c.R = this->R - a.R;
	c.I = this->I - a.I;
	return c;
}

Complex Complex::operator*(const Complex &a)
{
    ///--------------------------------------
    /// operator * (Multiplication operator)
    ///     This function multiplies this
    /// by another value
    ///
    /// parameters:
    ///     a.....value to multiply this by
    ///
    /// returns:
    ///     complex product of this and a
    ///--------------------------------------
	Complex c;
	c.R = this->R * a.R - this->I * a.I;
	c.I = this->I * a.R + this->R * a.I;
	return c;
}

Complex Complex::operator /(const Complex &a)
{
	///---------------------------------------------
	/// operator/	(Divide Operator)
	///		This function performs the divide
	///	operation on two complex values
	/// performs this/a
	///
	/// parameters:
	///		a......reference to the divisor
	///	returns:
	///		Quotient of this/a
	///---------------------------------------------
	Complex c;
	double mag = a.R * a.R + a.I * a.I;
	c.R =( this->R * a.R + this->I * a.I)/mag;
	c.I = (this->I * a.R - this->R * a.I)/mag;
	return c;
}

double Complex::Mag(void)
{
	///--------------------------------------------
	/// Mag
	///		Calulates the magnitude of a complex nmber
	///	returns:
	///		magnitude (absolute value)
	///--------------------------------------------
	return sqrt((R * R) +(I* I));
}

double Complex::Phase(double &LastPhase,int &N)
{
    /*****************************************
    ** Phase
    **
    ** Calculates the phase based on the
    ** Real and Imaginary parts of a value
    ** and a history of where the phase has
    ** been.
    **
    ** parameter:
  **  LastPhase....reference to previous uncorected phase value
    **  N......reference to the number of times phase has crossed 180
    **
    ** returns corrected phase value in radianas
    **
    **********************************************/
    double phase;

    ///-------------------------------------------------------
    /// Calculate the initial phase
    ///-------------------------------------------------------
    if((I > 0) && (R > 0)) phase =atan2(I,R);
    else if ((I > 0) && (R < 0)) phase = Pi - atan2(I,-R);
    else if ((I < 0) && (R > 0)) phase = - atan2(-I,R);
    else if ((I < 0) && (R < 0)) phase = -Pi + atan2(-I,-R);
    else if((I == 0) && (R < 0)) phase = Pi;
    else if ((I ==0) && (R > 0)) phase = 0.0;
    else if ((I < 0) && (R == 0)) phase = -Pi/2.0;
    else if ((I>0)&& (R == 0)) phase = Pi/2.0;
    else /*((I == 0) && (R == 0))*/
        fprintf(stderr,"Phase indeterminate for Magnitude = 0\n");
    ///-------------------------------------------------------
    /// In order to differentiate the phase to get time
    /// delay, we need to make the phase continuous
    /// So now we do that by keeping track on the number of
    /// times we cross over +/-Pi radians.  Not sure who
    /// created this code, it was given to us by a TA at UCSB
    ///-------------------------------------------------------
    if(fabs(phase - LastPhase) > (2.0 * Pi * 280.0 / 360.0))
    {
        if(phase < 0.0) N += 1;
        else if (phase > 0) N -= 1;
    }
//    printf("LastPhase=%lf   phase=%lf  Counts=%d  Correction=%lf\n",LastPhase,phase,N,Pi * 2.0 * (double)N);
    LastPhase = phase;
    return phase + Pi * 2.0 * (double)N;
}


