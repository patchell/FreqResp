#include <stdio.h>
#include <math.h>
#include "CPolynomial.h"
#include "Complex.h"

CPolynomial::CPolynomial()
{
    //ctor
    m_order = 0;
    int i;
    for(i=0;i<MAX_ORDER+1;++i)
        m_pCoef[i] = 0.0;
}

CPolynomial::CPolynomial(int O)
{
    //ctor
    int i;
    m_order = O;
    m_VarSym = 'S';
    for(i=0;i<MAX_ORDER+1;++i)
        m_pCoef[i] = 0.0;
}

void CPolynomial::Init(int O)
{
	///----------------------------------------
	/// Init
	///		This function initializes the Polynomial
	///	back to its initial state
	///
	///	parameters:
	///		O.......Order of polynomial
	///----------------------------------------
    int i;
    m_order = O;
    m_VarSym = 'S';
    for(i=0;i<MAX_ORDER+1;++i)
        m_pCoef[i] = 0.0;
}

CPolynomial::~CPolynomial()
{
    //dtor
}

Complex CPolynomial::Eval(double w)
{
	///---------------------------------------------
	/// Eval
	///		This function is used to evaluate
	///	a polynomial at a particular value
	///
	///	parameters:
	///		w.....complex value to evaluate poly at
	///	returns:
	///		Complex value of polynomial
	///---------------------------------------------
    Complex rVal;
    double re=0.0,im=0.0;
    int i;
    for(i=0;i<(m_order+1);++i)
    {
        switch(i%4)
        {
            case 0: ///real
                re += m_pCoef[i] * pow(w,(double)i);
                break;
            case 1: ///imaginary
                im += m_pCoef[i] * pow(w,(double)i);
                 break;
            case 2:///real
                re -= m_pCoef[i] * pow(w,(double)i);
                break;
            case 3:///imaginary
                im -= m_pCoef[i] * pow(w,(double)i);
               break;
        }
    }
    rVal.Set(re,im);
    return rVal;
}

CPolynomial CPolynomial::operator *=(const CPolynomial &P)
{
	///----------------------------------------------------
	/// operator*=	(Operator assign-Multiply)
	///		This function is called when we
	/// perform an assign multiply operation
	///
	/// parameters:
	///		P.......reference to polynomial to multil by
	/// returns:
	///		value is placed in *this and returned
	///	by value
	///-----------------------------------------------------
    CPolynomial rP;
    int NewOrder = m_order + P.m_order;
    rP.Init(NewOrder);
    int i,j;
    for(i=0;i<m_order+1;++i)
    {
        for(j=0;j<P.m_order+1;++j)
        {
            rP.Set(i+j,rP.Get(i+j) + m_pCoef[i] * P.m_pCoef[j]);
        }
    }
    *this = rP;
    return rP;
}

CPolynomial CPolynomial::operator *(const CPolynomial &P)
{
	///--------------------------------------------------
	/// operator*		(Multiplication operator)
	///		This function performs the multiply
	/// operation on "this" polinomial and another
	///
	///	parameters:
	///		P......polynomial to multiply by
	///	returns:
	///		product of "this" polyinomial and P
	///--------------------------------------------------
    CPolynomial rP;
    int NewOrder = m_order + P.m_order;
    rP.Init(NewOrder);
    int i,j;
    for(i=0;i<m_order+1;++i)
    {
        for(j=0;j<P.m_order+1;++j)
        {
            rP.Set(i+j,rP.Get(i+j) + m_pCoef[i] * P.m_pCoef[j]);
        }
    }
    return rP;
}

CPolynomial &CPolynomial::operator =(CPolynomial P)
{
	///--------------------------------------------------
	/// operator=		(assignment operator)
	///		This function does the things needed to
	///	accomplish an assignment operation.  We just
	///	transfer the data from one polynomial to This one.
	///
	///	parameters:
	///		P......polynomial to get data from
	///	returns:
	///		puts data into this and returns *this
	///--------------------------------------------------
    int i;
    for(i=0;i<MAX_ORDER+1;++i)
        m_pCoef[i] = P.m_pCoef[i];
    m_VarSym = P.m_VarSym;
    m_order = P.m_order;
    return *this;
}

void CPolynomial::Print(FILE *pO)
{
	///--------------------------------------------------
	///	Print
	///		This function is used to output the
	///	polynomial to a file
	///
	///	parameters:
	///		pO......File pointer to output device
	///--------------------------------------------------
    char *s = new char[2048];
    int i;
    int l=0;
    for(i=m_order;i>0;--i)
    {
        l+=sprintf(&s[l],"(%lf)%c^%d + ",m_pCoef[i],m_VarSym,i);
    }
    sprintf(&s[l],"(%lf)",m_pCoef[i]);
    fprintf(pO,"%s\n",s);
    delete[] s;
}
