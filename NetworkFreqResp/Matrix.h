#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

extern void PrintMatrix(Complex A[20][20],FILE *pO,int M,int N);
extern int MatrixInvert(Complex Y[20][20],int N, Complex Z[20][20]);
extern int MatrixDeterminant(Complex Y[20][20],Complex *VDet,int N);
extern int MatrixMult(Complex A[20][20],int Am,int An,Complex B[20][20],int Bm, int Bn,Complex Y[20][20]);


#endif // MATRIX_H_INCLUDED
