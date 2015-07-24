#ifndef CPLOTPOINTS_H
#define CPLOTPOINTS_H

class FPoints;

class CPlotPoints
{
	int m_Hieght;	///how many points in the Y direction
	FPoints *m_pH,*m_pE;	///collection of points sets
	int PlotChar(int i, int j);
	void SortPoints(FPoints **ppP, int n, int incx);
public:
	CPlotPoints();
	virtual ~CPlotPoints();
	void AddPoints(FPoints *pP);
	void CSV(FILE *pO);
	void Plot(FILE *pO,const char *pTitle,int Autoscale=1);
	void PrintPoints(FILE *pO);
	void SetHieght(int h){m_Hieght = h;}
	int GetHieght(void){return m_Hieght;}
};

#endif // CPLOTPOINTS_H
