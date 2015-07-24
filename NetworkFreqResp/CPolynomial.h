#ifndef CPOLYNOMIAL_H
#define CPOLYNOMIAL_H

#define MAX_ORDER   10

class Complex;

class CPolynomial
{
    int m_order;        ///order of polynomical
    double m_pCoef[MAX_ORDER+1];    ///coeficients
    char m_VarSym;      ///symbol to use for variable
public:
    CPolynomial();
    CPolynomial(int Order);
    virtual ~CPolynomial();
    Complex Eval(double s);
    void Set(int i, double v){m_pCoef[i] = v;}
    double Get(int i){return m_pCoef[i];}
    void SetSymbol(char c){m_VarSym = c;}
    char GetSymbol(void){return m_VarSym;}
    int GetOrder(void){return m_order;}
    void Init(int order);
    void Print(FILE *pO);
    ///--------------------------------------
    /// polynomial operations
    ///--------------------------------------
    CPolynomial operator *(const CPolynomial &P);
    CPolynomial operator *=(const CPolynomial &P);
    CPolynomial &operator =(CPolynomial P);
};

#endif // CPOLYNOMIAL_H
