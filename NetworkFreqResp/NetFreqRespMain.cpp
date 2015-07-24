#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Complex.h"
#include "Component.h"
#include "Parser.h"
#include "CMatrix.h"
#include "CPlotPoints.h"
#include "FPoints.h"

Component *CompList = 0, *CompEnd;	///list of components

double LogFreq(double StartFreq,int NperDec,int i)
{
    /*************************************
    *** calculates out a frequency on a
    *** scale for a bode plot.
    ** parameters
    **  StartFreq.....starting freq in Hz
    **  NperDec.......Number of points per decade
    **  i.............Current itteration
    **
    ** returns frequency
    ********************************************/

    double f;

    f = StartFreq * pow(10,double(i)/double(NperDec));
    return f;
}

void FreqResponse(Component *pCL,CPlotPoints *pResp)
{
	FPoints *pPhase,*pMag,*pFreq;
	double LastPhase=0.0;
	int Crosses=0;
	int Steps,Decades,InputNode,OutPutNode,GroundNode;
	double StartFreq;
	int Npoints;	///number of points to collect
	int TotalNodes = CompList->GetTotalNodes();
	CMatrix Y(TotalNodes,TotalNodes);
	///-----------------------------------------------------------
	/// Get various items of interest
	/// such as input, output and ground
	/// nodes as well as frequency sweep
	/// parameters
	///-----------------------------------------------------------
	CompList->GetSweepParams(StartFreq,Decades,Steps);
	InputNode = CompList->GetInputNode();
	OutPutNode = CompList->GetOutputNode();
	GroundNode = CompList->GetGroundNode();
	int i;
	///-----------------------------------------------------------
	/// compute the number of iterations
	/// needed to make a complete frequency
	/// sweep
	///-----------------------------------------------------------
	Npoints = Steps * Decades +1;
	pPhase = new FPoints(Npoints,'P');
	pPhase->SetName("Phase");
	pMag = new FPoints(Npoints,'M');
	pMag->SetName("Magnitude");
	pFreq = new FPoints(Npoints,0);
	pFreq->SetName("Frequency");
	pResp->AddPoints(pFreq);
	pResp->AddPoints(pMag);
	pResp->AddPoints(pPhase);
	for(i=0;i< Npoints;++i)
	{
		double w;	///radian frequency
		Complex Denom,Numerator,NetworkResp;
		Y.Zero();		///zero main matrix
		///get the next frequency
		w = LogFreq(StartFreq,Steps,i);
		///make the entries into the main matrix
		CompList->AddCompoentsToMatrix(Y,w);
		CMatrix Y1;
		///eliminate row and column associated
		///with the ground node
		Y1 = Y.Minor(GroundNode,GroundNode);
		CMatrix Y2;
		///compute determinate of denominator
		///cofactor
		Denom = Y1.Cofactor(InputNode,InputNode);
		///compute determinate of numerator
		/// cofactor
		Numerator = Y1.Cofactor(InputNode,OutPutNode);
		///Computer ratio of transfer function
		NetworkResp = Numerator/Denom;
		pPhase->AddPoint(NetworkResp.Phase(LastPhase,Crosses) *180.0/Pi);
		pMag->AddPoint(log10(NetworkResp.Mag()) *20.0);
		pFreq->AddPoint(w);

	}
	return;
}

int main(int argc,  char *argv[])
{
	FILE *pI = fopen(argv[1],"r");
	FILE *pO = stdout;

	if(pI == NULL)
	{
		fprintf(stderr,"could not nopen File\n");
		exit(1);
	}
	///---------------------------------------------------------
	/// Read in circuit file and create a list
	/// of components
	///---------------------------------------------------------
	Parse(pI,&CompList,&CompEnd);
	PrintComponentList(CompList,pO);
	int Dec,Steps;
	double sf;
	CompList->GetSweepParams(sf,Dec,Steps);
	CPlotPoints BodePlot;
	BodePlot.SetHieght(Dec*Steps+1);
	FreqResponse(CompList,&BodePlot);
	BodePlot.Plot(stdout,"MagPhase");
	BodePlot.CSV(stdout);
	BodePlot.PrintPoints(stdout);
	fclose(pO);	/// final cleanup
	fclose(pI);
    return 0;
}
