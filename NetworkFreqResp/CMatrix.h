#ifndef CMATRIX_H
#define CMATRIX_H

#include "Complex.h"

///----------------------------------------------------
/// CMatrix
///
/// Implementation of a two
/// dimensional complex number
/// matrix
///-----------------------------------------------------

class CMatrix
{
public:
	CMatrix(){m_Y=0;m_row=0;m_col=0;m_Ytransfered=0;}
	CMatrix(int rows,int columns);
	virtual ~CMatrix();
	CMatrix(const CMatrix& other);
	///Indexing operations
	Complex &operator()(int i,int j);
	const Complex &operator()(int i,int j) const;
	///Compute determinant
	Complex Det();
	///other operations
	CMatrix operator~();	//Matrix Invert
	CMatrix operator*(const CMatrix &a);
	CMatrix &operator=(CMatrix &a);
	CMatrix operator=(CMatrix a);
	Complex Cofactor(int row,int col);
	 CMatrix Minor(int row,int col);
	void Print(FILE *pO,const char *pT);	//print matrix
	void SetTransfered(void){m_Ytransfered =1;}
	void Zero(void);
protected:
private:
		int m_Ytransfered;	///this is sort of a kluge.
		int m_col;	//number of columns was m_m
		int m_row;	//number of rows was m_n
		Complex *m_Y;	// pointer to matrix members
};

#endif // CMATRIX_H
