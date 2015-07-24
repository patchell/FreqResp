#ifndef FPOINTS_H
#define FPOINTS_H


class FPoints
{
	int m_char;		///character to display while ploting
	double *m_pYaxis;	///y axis coordinates
	double *m_pYscaled;	///y axis coordinates scaled
	int m_n;			///total number of points
	int m_i;	///index into point set
	double m_m;		///scale factor
	double m_b;		///offset factor
	double m_Min,m_Max;	///min and max of points
	FPoints *m_pNext;	///next set of points
	void MinMax(void);
	char *m_pName;		///name of Axis
public:
	FPoints();
	FPoints(int NumOfPoints,int PlotChar);
	virtual ~FPoints();
	int GetIndex(void){return m_i;}
	void ResetIndex(void);
	int AddPoint(double y);
	void Scale(int YLines);
	void Scale(int YLines,double Min,double Max);
	double Getm(void){return m_m;}
	double Getb(void){return m_b;}
	double GetMin(void){return m_Min;}
	double GetMax(void){return m_Max;}
	double GetScaledVal(int i){return m_pYscaled[i]; }
	double GetVal(int i){return m_pYaxis[i]; }
	int GetDispChar(void){return m_char;}
	int GetTotalPoints(void){return m_n;}
	FPoints *GetNext(void){return m_pNext;}
	void SetNext(FPoints *pN){m_pNext = pN;}
	char *GetName(void){return m_pName;}
	void SetName(const char *name);
};

#endif // FPOINTS_H
