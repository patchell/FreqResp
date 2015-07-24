///------------------------------------------------------------
/// Old School plotting routines
///
/// Copyright (c) 2015 by James Patchell
///------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "CPlotPoints.h"
#include "FPoints.h"

CPlotPoints::CPlotPoints()
{
	//ctor
	m_pH = 0;
	m_pE = 0;
}

CPlotPoints::~CPlotPoints()
{
	//dtor
}

void CPlotPoints::AddPoints(FPoints *pP)
{
	///---------------------------------------------------------
	/// AddPoints
	///		This function is used to add a set
	///	of points to the plot.  It should be
	/// noted that the first set of points to
	/// add IS the X axis, all the rest are the
	/// Y axis.
	///	parameters:
	///		pP......pointer to set of points to add
	///--------------------------------------------------------
	if(this->m_pH)
	{
		m_pE->SetNext(pP);
		m_pE = pP;
	}
	else
	{
		m_pH = pP;
		m_pE = pP;
	}
}

void CPlotPoints::CSV(FILE *pO)
{
	///-----------------------------------------------------
	/// CSV
	///		outputs the data in a comma
	///	separated format.
	///	parameters:
	///		pO.................File to write data to
	///----------------------------------------------------
	int i,Npoints;
	FPoints *pP;
	 pP = m_pH;

	Npoints = pP->GetTotalPoints();
	while(pP)
	{
		fprintf(pO,"%s",pP->GetName());
		pP = pP->GetNext();
		if(pP == NULL)
			fprintf(pO,"\n");
		else
			fprintf(pO,",");
	}
	 for(i=0;i<Npoints;++i)
	 {
		pP = m_pH;
		while(pP)
		{
			fprintf(pO,"%e",pP->GetVal(i) );
			pP = pP->GetNext();
			if(pP == NULL)
				fprintf(pO,"\n");
			else
				fprintf(pO,",");
		}
	 }
}

int CPlotPoints::PlotChar(int x, int y)
{
	///--------------------------------------------------------
	///	PlotChar
	///		This function is used to figure
	///	out which character to print to make
	/// something that sort of looks like
	/// graph paper.
	///
	/// parameters:
	///		x.......where we are on the X axis
	///		y......where we are on the Y axis
	///	return value:
	///		returns the appropriate character
	///----------------------------------------------------------
	int Space;

    ///-----------------------------------
    /// Not a multiple of 10
    ///-----------------------------------
	if (x%10 && y%10) Space = ' ';
	///x not a multiple, y is a multiple
	else if ((x%10) && !(y%10)) Space = '|';
	///x and y both multiples
	else if (!(x%10) && !(y%10)) Space = '+';
	///x is multiple, y is not multiple
	else if (!(x%10) && (y%10)) Space = '-';
	return Space;
}

void CPlotPoints::Plot(FILE *pO,const char *pTitle,int AutoScale)
{
	///----------------------------------------------------------------------------
	/// Plot
	///		This function plots all of the axis.  The first
	///	axis in the list is the x axis, and all of the
	///	following lists of points are the various functions
	/// of the x axis.  The scale for each Y axis is
	/// printed reight below the title line.
	///
	///	parameters:
	///		pO.........pointer to the file to write data to
	///		pTitle....pointer to the tile of plot
	///     AutoScale.Auto Scale Plots if true
	///----------------------------------------------------------------------------
	int TotalPoints;
	int j,k;
	char *lab = new char[256];
	FPoints *pP = m_pH;	///fget pointer to X axis points
	TotalPoints = pP->GetTotalPoints();
	///print title
	if(pTitle)
	{
		int len = strlen(pTitle);
		int spaces = (m_Hieght - len)/2;
		int extra = (m_Hieght - len)%2;
		for(j=0;j<(spaces-3);++j)
			lab[j] = '*';
		for(;j<spaces;++j) lab[j] = ' ';
		strcpy(&lab[j],pTitle);
		j += len;
		for(k=0;k<3;++j,++k) lab[j] = ' ';
		for(k=0;k<(spaces-3 + extra);++k,++j) lab[j] = '*';
		lab[j] = 0;
		fprintf(pO,"%s\n",lab);
	}
	///Scale Data
	int i=0;
	pP = pP->GetNext();	//get first Y axis
	///-----------------------------------------
	/// If no hight has been specified use 80
	/// as the default
	///-----------------------------------------
	if(this->m_Hieght==0) m_Hieght = 80;
	///-----------------------------------------
	/// Even if we are not going to auto scale
	/// we still do this loop because this is
	/// where we also count the number of axis
	/// to plot.
	///-----------------------------------------
	while(pP)
	{
		if(AutoScale) pP->Scale(m_Hieght);
		pP = pP->GetNext();
		++i;	///count the number of axis
	}
	int TotalAxis = i;
	///Axis lableling
   double *pAxisInc = new double[TotalAxis];
   pP = m_pH;
   pP = pP->GetNext();	///first Y axis
   i = 0;
   ///---------------------------------
   /// Calculate the increment for
   /// each Y axis
   ///---------------------------------
   while(pP)
   {
		pAxisInc[i++] = (pP->GetMax() - pP->GetMin())/(double(m_Hieght-1)/10.0);
		pP = pP->GetNext();
   }
   pP = m_pH;
   pP = pP->GetNext();	///first Y axis
   i = 0;
   int axis = 0;
   ///----------------------------------
   /// Calculate the total number of
   /// Y axis labels.
   ///----------------------------------
    int YaxisLabels = (m_Hieght/10) + 1;
    ///---------------------------------
    /// Actually do the labeling for
    /// the Y axis
    ///---------------------------------
   while(pP)
   {

		int j,k;
		for(j=0,i=0;j<YaxisLabels;++j)
		{
			i += sprintf(&lab[i],"%6.2lf",pP->GetMin() + pAxisInc[axis] * double(j));
			if(j<YaxisLabels-1)
				for(k=i;k%10;++k)
					lab[i++] = ' ';
		}
		sprintf(&lab[i],"(%c)%s\n",pP->GetDispChar(),pP->GetName());
		fprintf(pO,"%s",lab);
		++axis;
		pP = pP->GetNext();
   }
	///----------------------------------------------------
	/// Draw X Axis and plot points
	///----------------------------------------------------
	/// Create an array of pointer to the Point Pointers
	FPoints** axOrder = new FPoints*[TotalAxis];
	pP = m_pH;  ///get the head of the points list
	pP = pP->GetNext(); /// Get first Y axis
	for(i=0;i<TotalAxis;++i,pP=pP->GetNext() )
		axOrder[i] = pP;    ///fill up the array
	for(k=0;k<TotalPoints;++k)	///plot all points
	{
		SortPoints(axOrder,TotalAxis,k);	///sort points
		///draw plot for this X point
		int flag = 1;
		for(i=0,j=0;(i<TotalAxis) && flag;++i)
		{
			for(;(j<int(axOrder[i]->GetScaledVal(k))) && (j<m_Hieght);++j)
			{
				lab[j] = PlotChar(k,j);
			}
			///if we haven't run out of plot room
			///plot the plot character for that point
			if(j<m_Hieght) lab[j++] = axOrder[i]->GetDispChar();
			///and if j is equal to the plot height
			///we are done.
            if(j>= m_Hieght) flag = 0;
		}
		///if there is still room, finish up for this
		/// x axis point
		for(;j<m_Hieght;++j)
			lab[j] = PlotChar(k,j);
		///add X axis value
		sprintf(&lab[j],"  %8.6e",m_pH->GetVal(k));
		///output to the print device
		fprintf(pO,"%s\n",lab);
	}
	delete[] axOrder;   ///delete allocated variables
	delete[] lab;
}

void CPlotPoints::SortPoints(FPoints **ppP, int n, int index)
{
	///----------------------------------------------------------
	/// SortPoints
	///		This function uses a very simple
	///	algorithm to sort the points to be
	/// plotted.  We don't really need anything
	/// very fancy since we are not going to
	/// be sorting any large lists.
	///
	/// parameters:
	///		ppP......pointer to an array of FPoints
	///		n...........Number of Y axis
	///		index...which X axis point we are on
	///-------------------------------------------------------------
	int i,j;

	for(i=0;i<(n-1);++i)
	{
		for(j=i+1;j<n;++j)
		{
			if(ppP[i]->GetScaledVal(index) > ppP[j]->GetScaledVal(index))
			{
				///then swap
				FPoints *pT = ppP[i];
				ppP[i] = ppP[j];
				ppP[j] = pT;
			}
		}
	}
}

void CPlotPoints::PrintPoints(FILE *pO)
{
	///------------------------------------------------------------
	/// PrintPoints
	///		Outputs the set of points.
	/// parameters:
	///		pO........pointer to output device
	///------------------------------------------------------------
	FPoints *pP;
	 pP = m_pH;
    int i;

	int Npoints = pP->GetTotalPoints();
	fprintf(pO,"Item\t");
	while(pP)
	{
		fprintf(pO,"%s\t",pP->GetName());
		pP = pP->GetNext();
		if(pP == NULL)
			fprintf(pO,"\n");
		else
			fprintf(pO,"\t");
	}
	 for(i=0;i<Npoints;++i)
	 {
	 	fprintf(pO,"%4d\t",i);
		pP = m_pH;
		while(pP)
		{
			fprintf(pO,"%3.9e",pP->GetVal(i) );
			pP = pP->GetNext();
			if(pP == NULL)
				fprintf(pO,"\n");
			else
				fprintf(pO,"\t");
		}
	 }
}


