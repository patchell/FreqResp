///--------------------------------------------------
/// This class implements a class that
/// represents a Matrix.  One of the features
/// of this class is a much cleaner implementation
/// of the matrix index.  C++ provides the []
/// operator for doing this, but it is very
/// clumsy to use to say the least.  A better way
/// is to over ride the () to do this.  This
/// allows us to easily pass a Matrix without
/// having to worry about the dimensions
/// of the matrix.
///---------------------------------------------------
#include "CMatrix.h"

CMatrix::CMatrix(int rows, int columns)
{
	///------------------------------------------------------------
	/// Constructor
	///		Construct a complex matrix
	///	parameters:
	///		columns...Number of Colums (Width)
	///		rows..........Number of Rows(Height)
	///------------------------------------------------------------
    m_col = columns;		///Columns
    m_row = rows;			///Rows
	m_Y = new Complex[m_col*m_row];	///create data array
	m_Ytransfered =0;
	int i;
	for(i=0;i<(m_col*m_row);++i) m_Y[i] = Complex(0.0,0.0);
}

CMatrix::~CMatrix()
{
	///-------------------------------------------------------
	/// Destructor
	///
	/// OK, there is a bit of a kludge in here
	///---Problem:
	/// If you declare a variable as a local
	/// in a function, that is returned, well
	/// bad things will happen when this
	/// destructor is run when it deletes
	/// m_Y.
	/// Solution:
	/// I added the variable m_Ytransfered
	/// that is set by any of the copy
	/// operators.  This is because we
	/// just copy the pointer, which is faster
	/// that creating a new pointer and then
	/// making a copy of all the data.
	/// Don't know of a better solution at
	/// this time.
	///-------------------------------------------------------------
	if(m_Y && !m_Ytransfered)
	{
		delete[] m_Y;
	}
}

CMatrix::CMatrix(const CMatrix& other)
{
	//copy ctor
	m_row = other.m_row;
	m_col = other.m_col;
	m_Y = other.m_Y;
	this->m_Ytransfered = 1;
}

Complex CMatrix::Det()
{
	///----------------------------------------------------
	///	MatrixDeterminant
	/// Calculate the determinant of
	/// A matrax.
	///
	/// Translated from C which was
	/// Translated from Fortran from
	/// Page 68 of Computer Aided Circuit Design
	///
	/// parameters: NONE
	///
	///	Returns:
	///		Value of determinant
	/// Does not destroy
	///-----------------------------------------------
	Complex Swap,Pivot,Pm;
	int i,j,k;
	double Amax;
	Complex VDet;
	int nr,nc;
	CMatrix Y(m_row,m_col);

	for(i=0;i<m_col;++i)	///rows
		for(j=0;j<m_row;++j)	///columns
			Y(i,j) = (*this)(i,j);	///make copy
	VDet = Complex(1.0,0.0);		///initial value of det
	for(i=0;i<(m_col-1);++i)	///rows
	{
		Amax = 0.0;
		for(j=i;j<m_row;++j)	///columns
		{
			for(k=i;k<m_row;++k)	///columns
			{
				if(Y(j,k).Abs() >= Amax)
				{
					nr = j;	///row
					nc=k;	///column
					Amax =Y(j,k).Abs();
				}
			}
			if(nc != i)
			{
				for(j=i;j<m_row;++j)
				{
					Swap = Y(j,i);
					Y(j,i) = Y(j,nc);
					Y(j,nc) = -Swap;
				}
			}	///end of if(nc != i)
			if(nr!=i)
			{
				for(j=i;j<m_row;++j)	///colums
				{
					Swap = Y(i,j);
					 Y(i,j) = Y(nr,j);
					Y(nr,j) = -Swap;
				}
			}	///end of if(nr != i)
			Pivot = Y(i,i);
			VDet = VDet * Pivot;
			for(j=i+1;j<m_row;++j)	///columns
			{
				Pm =  Y(j,i)/Pivot;
				for(k=i+1;k<m_row;++k)
				{
					Y(j,k) =  Y(j,k) -  Y(i,k) * Pm;
				}
			}
		}
	}
	VDet = VDet *  Y(m_col-1,m_row-1);
	return VDet;
}

///other operations
CMatrix CMatrix::operator~()
{
	///------------------------------------------------
	///	MatrixInvert
	///		Inverts a Matrix of order N
	/// using the Gauss Jordan
	/// Algorythm.
	///--------------------------------------------------
	CMatrix Z(m_col,m_row);
	Complex Piv,x,Pm;
	int i,j,k;
	if(m_col != m_row)
		fprintf(stderr,"Matrix MUST be sqaure\n");
	else
	{
		///Initialize array A
		/// we want a matrix with the same
		/// number of rows
		///but twice as many columns
		CMatrix A(m_col,m_row*2);
		for(i=0;i<m_row;++i)	///rows
		{
			for(j=0;j<m_col;++j)	///columns
			{
				A(i,j) = (*this)(i,j);
			}
		}
		///Create Identity Matrix
		for(i=0;i<m_col;++i)	///rows
		{
			int n = m_row *2;
			for(j=m_row;j<n;++j)		///columns
			{
				if((i+m_row) ==( j)) A(i,j) = Complex(1.0,0.0);
				else  A(i,j) = Complex(0.0,0.0);
			}
		}
		///------------------------------------------------
		/// Everything is set up now, start
		/// the inversion
		///------------------------------------------------
		for(i=0;i<m_row;++i)	///rows
		{
			Piv = A(i,i);
			for(j=0;j<m_col;++j)	///columns
			{
				if(i != j)
				{
					Pm =  A(j,i)/Piv;
					for(k=i+1;k<(m_col+i+1);++k)
					{
						A(j,k)= A(j,k) - A(i,k)*Pm;
					}
				}
			}
		}
		for(i=0;i<m_col;++i)	///rows
		{
			x = A(i,i);
			for(j=0;j<m_row;++j)	///columns
				Z(i,j) = A(i,j+m_row)/x;
		}
	}
	return Z;
}

CMatrix CMatrix::operator*(const CMatrix &B)
{
	///----------------------------------------------------
	///	Matrix Multiply
	///
	/// paramter:
	///		B.......reference to the value
	///			that is getting multiplied
	///			by "this" one.
    /// Algorythm
    ///		[AB](i,j) = Sum(k=1 to m)A(i,k) * B(k,j)
    ///		The number of columns of A must be the
    ///		The same as the number of rows
    ///		of B
	///--------------------------------------------------
   int i,j,k;
    ///---------------------------------------------------
    /// Is the number of rows of this
    /// equal to the number of columns
    /// of B
    ///---------------------------------------------------
   	CMatrix Y(this->m_row,B.m_col);

    if(this->m_row == B.m_col)
	{
		///Zero Y
		for(i=0;i<Y.m_row;++i)	///rows
			for(j=0;j<Y.m_col;++j)
				Y(i,j) = Complex(0.0,0.0);
		///perform multiply
		for(i=0;i<this->m_col;++i)	///go across rows of A (columns)
		{
			for(j=0;j<B.m_row;++j)	///go down columns of B (rows)
			{
				for(k=0;k<B.m_col;++k)
				{
					Y(i,j) = Y(i,j) + (*this)(i,k) * B(k,j);
				}
			}
		}
	}
	else
		fprintf(stderr,"Matrix Mult:Error Rows != Columns\n");
 	return Y;
}

void CMatrix::Print(FILE *pO,const char *pTitle)	//print matrix
{
    int i,j;

	fprintf(pO,"--------- %s ---------\n",pTitle);
	fprintf(pO,"Rows=%d  Col=%d\n",m_col,m_row);
    for(i=0;i<m_row;++i)	///rows
    {
		for(j=0;j<m_col;++j)	///columns
			(*this)(i,j).print("%5.2lf+j%5.2lf ",pO);
		fprintf(pO,"\n");
	}
	fprintf(pO,"-------------------------------\n");

}

Complex &CMatrix::operator()(int i,int j)
{
	///------------------------------------------------------
	/// Index Operator
	///	This is the indexing operator.  This is
	/// what makes this class so cool.  The
	/// only bad thing is that we can't use the
	/// [] operator.  Still, we get very readable
	/// code and none of the problems
	/// associated with using A[][] that is built
	/// into C++.
	///
	///	parameters:
	///		i...........row
	///		j...........column
    ///		returns:
    ///			the (i,j) th element of the matrix
	///------------------------------------------------------
	///	m_col.............Number of Columns
	/// m_row..............Number of Rows
	///
	/// Calculate the index.
	/// Index = Column + Row * Nmber of columns
	///------------------------------------------------------
	int maxindex = m_row * m_col;
	///-------------------------------------------------------
	/// To computer the index, we add the
	/// column number (j) to the product of
	/// row number (i) time the number of
	/// columns, which is also the same as
	/// the number of elements per row
	///------------------------------------------------------
	int index = j + i*m_col;
	if(index >= maxindex) fprintf(stderr,"ERROR:Matrix Index i=%d  j=%d m=%d  n=%d  max=%d Inx=%d\n",i,j,m_col,m_row,maxindex,index);
	return m_Y[index];
}

const Complex &CMatrix::operator()(int i,int j) const
{
	///------------------------------------------------------------
	/// Operator()
    ///		parameters:
    ///			i........row
    ///			j........column
    ///		returns:
    ///			the (i,j) th element of the matrix
    ///---------------------------------------------------------------
	int maxindex = m_row * m_col;
	int index = j + i*m_col;
	if(index >= maxindex) fprintf(stderr,"ERROR:Matrix Index i=%d  j=%d m=%d  n=%d max=%d Index=%d\n",i,j,m_col,m_row,maxindex,index);
	return m_Y[index];
}

CMatrix &CMatrix::operator=(CMatrix &a)
{
	///------------------------------------------------------
	/// Operator=
	/// This function handles the assignment
	/// of the matrix type.  Most important
	/// is the transfer of the matrix data.
	/// parameter:
	///		a...........reference to object to copy to "this"
	///-------------------------------------------------------
	m_col = a.m_col;
	m_row = a.m_row;
	m_Y = a.m_Y;
	///---------------------------------------
	/// indicate that the matrix
	/// data (pointer to a Complex
	/// array) has been transfered
	/// so the data won't get
	/// deleted.
	///---------------------------------------
	a.SetTransfered();
	return *this;
}

CMatrix CMatrix::operator=(CMatrix a)
{
	///------------------------------------------------------
	/// Operator=
	/// This function handles the assignment
	/// of the matrix type.  Most important
	/// is the transfer of the matrix data.
	/// parameter:
	///		a...........object to copy to "this"
	///-------------------------------------------------------
	m_col = a.m_col;
	m_row = a.m_row;
	m_Y = a.m_Y;
	///---------------------------------------
	/// indicate that the matrix
	/// data (pointer to a Complex
	/// array) has been transfered
	/// so the data won't get
	/// deleted.
	///---------------------------------------
	a.SetTransfered();
	return *this;
}

void CMatrix::Zero(void)
{
	///------------------------------------------------------
	/// Zero
	///	Very simple, just sets all the elements
	/// of the matrix to zero
	///-----------------------------------------------------
	int i,j;

	for(i=0;i<m_row;++i)
		for(j=0;j<m_col;++j)
			(*this)(i,j) = Complex(0.0,0.0);
}

Complex CMatrix::Cofactor(int r,int c)
{
	///-------------------------------------------------------------
	/// Cofactor
	/// Compute the cofactor(r,c) of a matrix
	/// we do this by removing row r and
	/// column c from the matrix, and then
	/// computing the determinant of the
	/// resulting matrix.  This removal
	/// process is called taking the minor
	///
	/// parameters:
	///		r........row to remove
	///		c.......column to remove
	/// returns:
	/// determinant of the minor matrix
	///--------------------------------------------------------------
	Complex Cofac;

	CMatrix M(m_row-1,m_col-1);
	M = Minor(r,c);
	Cofac = M.Det();
	if((r+c)%2)	///odd
		Cofac = Cofac * Complex(-1.0,0.0);
	return Cofac;
}

CMatrix CMatrix::Minor(int r,int c)
{
	///--------------------------------------------------
	/// Minor
	///		Take the minor of a Matrix
	/// We do this by removing row r
	///	and column c
	///
	///	parameters:
	///		r.........row to remove
	///		c.........column to remove
	/// returns
	///		The minor matrix
	///---------------------------------------------------
	CMatrix YC(this->m_row-1,this->m_col-1);
	int i,j,ii,jj;

	for(i=0,ii=0;i<m_row;++i)
	{
		if(i!=r)
		{
			for(j=0,jj=0;j<m_col;++j)
			{
				if(j!=c)
				{
					YC(ii,jj) = (*this)(i,j);
					++jj;
				}
			}
			++ii;
		}
	}
	YC.SetTransfered();
	return YC;
}

