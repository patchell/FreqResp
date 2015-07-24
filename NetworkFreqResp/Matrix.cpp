/***************************************************************
** Matrix.cpp
**
** these are function for dealing with Matrixs
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "Complex.h"

void PrintMatrix2040(Complex A[20][40],FILE *pO,int M, int N)
{
    int i,j;

    for(i=0;i<M;++i)	///rows
    {
		for(j=0;j<N;++j)	///columns
			A[i+1][j+1].print("%5.2lf+j%5.2lf ",pO);
		fprintf(pO,"\n");
	}
	fprintf(pO,"-------------------------------\n");
}

void PrintMatrix(Complex A[20][20],FILE *pO,int M, int N)
{
    int i,j;

    for(i=0;i<M;++i)	///rows
    {
		for(j=0;j<N;++j)	///columns
			A[i+1][j+1].print("%5.2lf+j%5.2lf ",pO);
		fprintf(pO,"\n");
	}
	fprintf(pO,"-------------------------------\n");
}

int MatrixInvert(Complex Y[20][20],int N, Complex Z[20][20])
{
	///------------------------------------------------
	///	MatrixInvert
	///		Inverts a Matrix of order N
	/// using the Gauss Jordan
	/// Algorythm.
	/// parameters:
	///		Y..........Input Matrix
	///		N..........Order of Matrix
	///		Z..........Output Matrix
	/// Returns
	///		0 on success
	///--------------------------------------------------
	int rV = 0;
	Complex A[20][40];		///This is NOT a good practice
	Complex Piv,x,Pm;
	int i,j,k;

	///Initialize array A
	for(i=1;i<=N;++i)
	{
		for(j=1;j<=N;++j)
			A[i][j] = Y[i][j];
	}
    for(i=1;i<=N;++i)
	{
		for(j=N+1;j<=2*N;++j)
		{
			if(i+N == j)A[i][j].Set(1.0,0.0);
			else A[i][j].Set(0.0,0.0);
		}
	}
	printf("-----------Invert Initialized-------------------\n");
	PrintMatrix2040(A,stdout,N,2*N);
	///------------------------------------------------
	/// Everything is set up now, start
	/// the inversion
	///------------------------------------------------
    for(i=1;i<=N;++i)
	{
		Piv = A[i][i];
		printf("Pivot=");Piv.print(stdout);
		for(j=1;j<=N;++j)
		{
			if(i != j)
			{
				Pm = A[j][i]/Piv;
				printf("Pm=");Pm.print(stdout);
				for(k=i+1;k<=N+i;++k)
				{
					A[j][k]= A[j][k] - A[i][k]*Pm;
					printf("A(%d,%d) -= A(%d,%d)*Pm\n",j-1,k-1,i-1,k-1);
				}
			}
		}
		PrintMatrix2040(A,stdout,N,2*N);
	}
	printf("-----------Invert Intermediate-------------------\n");
	PrintMatrix2040(A,stdout,N,2*N);
	for(i=1;i<=N;++i)
	{
		x = A[i][i];
		for(j=1;j<=N;++j)
			Z[i][j] = A[i][j+N]/x;
	}
	return rV;
}

int MatrixDeterminant(Complex Y[20][20],Complex *VDet,int N)
{
	///----------------------------------------------------
	///	MatrixDeterminant
	/// Calculate the determinant of
	/// A matrax.
	///
	/// Translated from Fortan
	/// Page 68 of Computer Aided Ciruit Design
	///
	/// parameters:
	///		Y.......Matrix to find det of
	///		Vdet....Pointer to Calculated Determant
	///		N.......degree of Matrix
	///
	///	Returns:
	///		0 on success.
	///-----------------------------------------------
	Complex Swap,Pivot,Pm;
	int i,j,k;
	double Amax;
	int nr,nc;

	VDet->Set(1.0,0.0);		///initial value of det
	for(i=1;i<N;++i)
	{
		Amax = 0.0;
		for(j=i;j<=N;++j)
		{
			for(k=i;k<=N;++k)
			{
				if(Y[j][k].Abs() >= Amax)
				{
					nr = j;
					nc=k;
					Amax =Y[j][k].Abs();
				}
			}
			if(nc != i)
			{
				for(j=i;j<=N;++j)
				{
					Swap = Y[j][i];
					Y[j][i] = Y[j][nc];
					Y[j][nc] = -Swap;
				}
			}	///end of if(nc != i)
			if(nr!=i)
			{
				for(j=i;j<=N;++j)
				{
					Swap = Y[i][j];
					Y[i][j] = Y[nr][j];
					Y[nr][j] = -Swap;
				}
			}	///end of if(nr != i)
			printf("----------------- Swapped -------------------\n");
			PrintMatrix(Y,stdout,N,N);
			Pivot = Y[i][i];
			printf("Pivot:");
			Pivot.print(stdout);
			*VDet = *VDet * Pivot;
			for(j=i+1;j<=N;++j)
			{
				Pm = Y[j][i]/Pivot;
				for(k=i+1;k<=N;++k)
				{
					printf("i=%d  j=%d  k=%d \n ",i-1,j-1,k-1);
					Pm.print(stdout);
					Y[j][k] = Y[j][k] - Y[i][k] * Pm;
				}
			}
			printf("----------------- After Swapped -------------------\n");
			PrintMatrix(Y,stdout,N,N);
		}
	}
	*VDet = *VDet * Y[N][N];
	return 0;
}

int MatrixMult(Complex A[20][20],int Am,int An,Complex B[20][20],int Bm, int Bn,Complex Y[20][20])
{
    ///----------------------------------------------------
    /// MatrixMult
    ///			Multiply two matrix's and put
    ///		the result in a third
    ///
    /// Algorythm
    ///		[AB](i,j) = Sum(k=1 to m)A(i,k) * B(k,j)
    ///		The number of columns of A must be the
    ///		The same as the number of rows
    ///		of B
    ///
    ///	parameters:
    ///		A...........First Matrix to Multiply
    ///		Am........Number of columns of A
    ///		An.........Number of rows of A
    ///		B............Second Matrix to Multiply
    ///		Bm.......Number of columns of B
    ///		Bn........Number of rows of B
    ///		Y..........Output Matrix
    ///-------------------------------------------------------------
    int i,j,k,m;
    int rV = 0;
    if(Am == Bn)
	{
		m = Bn;
		///Zero Z
		for(i=1;i<=Am;++i)
			for(j=1;j<Bn;++j)
				Y[i][j].Set(0.0,0.0);
		///perform multiply
		for(i=1;i<=Am;++i)	///go across rows of A
		{
			for(j=1;j<=Bn;++j)	///go down columns of B
			{
				for(k=1;k<=m;++k)
				{
					Y[i][j] =  Y[i][j] + A[i][k] * B[k][j];
				}
			}
		}
	}
	else
		rV = 1;	///error
	return rV;
 }
