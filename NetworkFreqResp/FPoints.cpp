#include <string.h>
#include <math.h>
#include "FPoints.h"

FPoints::FPoints()
{
	//ctor
	this->m_b = 0.0;
	this->m_char = 0;
	this->m_i = 0;
	this->m_m = 0.0;
	this->m_Max=0.0;
	this->m_Min = 0.0;
	this->m_n = 0;
	this->m_pNext = 0;
	this->m_pYaxis = 0;
	this->m_pYscaled = 0;
	this->m_pName = 0;
}

FPoints::FPoints(int NumOfPoints,int PlotChar)
{
	this->m_b = 0.0;
	this->m_char = PlotChar;
	this->m_i = 0;
	this->m_m = 0.0;
	this->m_Max=0.0;
	this->m_Min = 0.0;
	this->m_n = NumOfPoints;
	this->m_pNext = 0;
	this->m_pYaxis = new double[m_n];
	this->m_pYscaled = 0;
	this->m_pName = 0;
}

void FPoints::ResetIndex(void)
{
    ///--------------------------------------------
    /// ResetIndex
    ///
    ///     This function resets the index pointer
    /// to the spot in the points array where a
    /// new data point is to be stored.
    ///
    ///--------------------------------------------
    m_i = 0;
	this->m_Max=0.0;
	this->m_Min = 0.0;
}

FPoints::~FPoints()
{
	//dtor
	if(m_pYaxis) delete[] m_pYaxis;
	if(m_pYscaled) delete[] m_pYscaled;
	if(m_pName) delete[] m_pName;
}

int FPoints::AddPoint(double y)
{
	///---------------------------------------------------
	/// AddPoint
	///		This function adds a point
	///	value to a set of points.  It starts
	/// at position 0 and auto increments
	/// The index can be reset using the
	/// ResetIndex method
	///
	/// Parameters:
	///		y.........value to set current point
	/// return value:
	///	returns true if OK,
	/// returns false if all points are filled
	///----------------------------------------------------
	int rv;
	m_pYaxis[m_i] = y;
	++m_i;
	if(m_i < m_n) rv = 1;
	else rv = 0;
	return rv;
}

void FPoints::Scale(int YLines,double Min,double Max)
{
	///----------------------------------------------------------
	/// Scale
	///		This function scales the collected
	///	data so that it files the graph.  The
	/// parameter YLines is the number of
	/// columns possible on the display.
	/// for instance 80 columns.
	/// This method does not destroy
	/// the original data, rather it puts the
	/// data into an array of scaled data.
	///
	///	Parameters:
	///		YLines........Number of display columns
	///     Max...........Maximum Value of Y axis
	///     Min...........Minimum Value of Y axis
	///------------------------------------------------------------
	int i;
	m_Min = Min;
	m_Max = Max;
	///---------------------------------------
	/// we want Min and Max to be integer vals
	///---------------------------------------

	this->m_m =(double)YLines/ (m_Max- m_Min);
	m_b = (double)YLines - m_m * m_Max;
	if(this->m_pYscaled==0)
		m_pYscaled = new double[m_n];
	for(i=0;i<m_n;++i)
		m_pYscaled[i] = m_m * m_pYaxis[i] + m_b;
}

void FPoints::Scale(int YLines)
{
	///----------------------------------------------------------
	/// Scale
	///		This function scales the collected
	///	data so that it files the graph.  The
	/// parameter YLines is the number of
	/// columns possible on the display.
	/// for instance 80 columns.
	/// This method does not destroy
	/// the original data, rather it puts the
	/// data into an array of scaled data.
	///
	///	Parameters:
	///		YLines........Number of display columns
	///------------------------------------------------------------
	int i;
	MinMax();
	m_Min = floor(m_Min);
	m_Max = ceil(m_Max);
	///---------------------------------------
	/// we want Min and Max to be integer vals
	///---------------------------------------

	this->m_m =(double)YLines/ (m_Max- m_Min);
	m_b = (double)YLines - m_m * m_Max;
	if(this->m_pYscaled==0)
		m_pYscaled = new double[m_n];
	for(i=0;i<m_n;++i)
		m_pYscaled[i] = m_m * m_pYaxis[i] + m_b;
}

void FPoints::MinMax(void)
{
	///-------------------------------------------------
	/// MinMax
	///		This function finds the
	///	minimum and maximum values
	/// of the plot points.
	///------------------------------------------------
	m_Max = this->m_pYaxis[0];
	m_Min = this->m_pYaxis[0];
	int i;

	for(i=0;i<m_n;++i)
	{
		if(this->m_Max <this->m_pYaxis[i]) m_Max =this->m_pYaxis[i];
		if(this->m_Min > this->m_pYaxis[i]) m_Min = this->m_pYaxis[i];
	}
}

void FPoints::SetName(const char *name)
{
	///--------------------------------------------------
	/// SetName
	///		Sets the name of the point
	///	set.
	///	parameters:
	///		name.....name to use
	///-------------------------------------------------
	if( m_pName) delete[] m_pName;
	m_pName = new char[strlen(name)+1];
	strcpy(m_pName,name);
}
