#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"
#include "CPolynomial.h"
#include "FPoints.h"
#include "CPlotPoints.h"

double PoleZeroFreq[10];
///--------------------------------------------------
/// magnitude and phase of a transfer function
/// based on the book
/// "Digital Computation in Basic Circuit Theory"
/// By Lawrence P. Huelsman
/// Translated into C++ by James Patchell
///---------------------------------------------------

int EvaluatePoints(FPoints *pP,double Min, double Max)
{
    int i;
    int InSpecCount=0;

    for(i=0;i<pP->GetTotalPoints();++i)
    {
        if((pP->GetVal(i) >= Min) && (pP->GetVal(i) <= Max))
           InSpecCount++;
    }
    printf("Total Points in Spec=%d\n",InSpecCount);
    return InSpecCount;
}

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

double Delay(double f, double p)
{
    static double lastf=0.0, lastp=0.0;
    static int flag=0;
    double rV;
    double NormalizedDeltaF;

    if(flag)
    {
        NormalizedDeltaF = (f - lastf)/((f + lastf)/2.0);
        rV = (p-lastp)/NormalizedDeltaF;
    }
    else
    {
        rV = 0.0;
        flag = 1;
    }
    lastp = p;
    lastf = f;
    return rV;
}

void CreatePhaseshiftNetwork(CPolynomial **ppN,CPolynomial **ppD,double StartPoleZero, double Ratio, int nPoles )
{
    int i;
    CPolynomial *N1,*N2,*D1,*D2;

    for(i=0;i<nPoles;++i,StartPoleZero *= Ratio)
    {
        PoleZeroFreq[i] = StartPoleZero/(2.0 *Pi);   ///radian frequency
        if(i==0)
        {
            N1 = new CPolynomial(1);
            D1 = new CPolynomial(1);
            N1->Set(0,PoleZeroFreq[i]);
            N1->Set(1,-1.0);
            D1->Set(0,PoleZeroFreq[i]);
            D1->Set(1,1.0);
            D2 = new CPolynomial(1);
            N2 = new CPolynomial(1);
        }
        else
        {
            ///Numberator
            N2->Set(0,-PoleZeroFreq[i]);
            N2->Set(1,1.0);
            *N1 *= *N2;
            ///Denominator
            D2->Set(0,PoleZeroFreq[i]);
            D2->Set(1,1.0);
            *D1 *= *D2;
       }
    }
    delete D2;
    delete N2;
    *ppN = N1;
    *ppD = D1;
}

int PhaseShifterCal(double PoleSpread,double SpacingRatio,int Points,int PointsPerDecade,FPoints *fP,FPoints *mP,FPoints *p1P,FPoints *p2P,FPoints *dP)
{
    CPolynomial *N1,*D1,*N2,*D2;
    double f,w,dB,ph1,ph2,Last1,Last2;
    Complex Num1,Num2,Den1,Den2,M1,M2;
    int PhaseCrossings2,PhaseCrossings1;
    int i;

    N1 = new CPolynomial;
    D1 = new CPolynomial;
    N2 = new CPolynomial;
    D2 = new CPolynomial;
    N1->Init(0);
    D1->Init(0);
    N2->Init(0);
    D2->Init(0);
    CreatePhaseshiftNetwork(&N1,&D1,10.0,PoleSpread,7);
    CreatePhaseshiftNetwork(&N2,&D2,10.0/(PoleSpread/SpacingRatio),PoleSpread,7);
    for(i=0;i<Points;++i)
    {
        f = LogFreq(1.0,PointsPerDecade,i);
        w = f/(2.0*Pi);
        Num1 = N1->Eval(w);
        Den1 = D1->Eval(w);
        M1 = Num1/Den1;

        Num2 = N2->Eval(w);
        Den2 = D2->Eval(w);
        M2 = Num2/Den2;

        dB = 20.0 * log10(M1.Mag());
        ph1 = M1.Phase(Last1,PhaseCrossings1);
        ph2 = M2.Phase(Last2,PhaseCrossings2);
        fP->AddPoint(f);
        mP->AddPoint(dB);
        p1P->AddPoint(ph1/Pi*180);
        p2P->AddPoint(ph2/Pi*180);
        dP->AddPoint((ph1-ph2)/Pi*180);
    }
    delete N1;
    delete N2;
    delete D1;
    delete D2;
    return EvaluatePoints(dP,89.0,91.0);
}

int main()
{
    int Points = 241;
    int Decades = 6;
    int PointsPerDecade = (Points-1)/Decades;
    int PlotHieght = 91;
    double ph1,Last1=0.0;
    double ph2,Last2=0.0;
    double w = .01;
    int i;
    double f;
    int PhaseCrossings1= 0;
    int PhaseCrossings2=0;
    CPolynomial *N1,*D1,*N2,*D2;
    Complex Num1,Num2,Den1,Den2,M1,M2;
    FPoints MagP(Points,'M'),PhaseP(Points,'P'),DelayP(Points,'D'),FreqP(Points,0),PhaseP2(Points,'Q');
    CPlotPoints Plots;
    double SpacingRatio = 2.23;
    double PoleSpread = 5.055;

    Pi = atan(1) * 4.0;
    N1 = new CPolynomial();
    D1 = new CPolynomial();
    N2 = new CPolynomial();
    D2 = new CPolynomial();

    MagP.SetName("Mag");
    PhaseP.SetName("Phase");
    PhaseP2.SetName("Phase2");
    DelayP.SetName("Diff");
    double dB;
    int loop = 1;
    do
    {
        PhaseCrossings1= 0;
        PhaseCrossings2=0;
        Last1 = 0.0;
        Last2 = 0.0;
        FreqP.ResetIndex();
        MagP.ResetIndex();
        PhaseP.ResetIndex();
        PhaseP2.ResetIndex();
        DelayP.ResetIndex();
        N1->Init(0);
        D1->Init(0);
        N2->Init(0);
        D2->Init(0);
        CreatePhaseshiftNetwork(&N1,&D1,10.0,PoleSpread,7);
        CreatePhaseshiftNetwork(&N2,&D2,10.0/(PoleSpread/SpacingRatio),PoleSpread,7);
        for(i=0;i<Points;++i)
        {
            f = LogFreq(1.0,PointsPerDecade,i);
            w = f/(2.0*Pi);
            Num1 = N1->Eval(w);
            Den1 = D1->Eval(w);
            M1 = Num1/Den1;

            Num2 = N2->Eval(w);
            Den2 = D2->Eval(w);
            M2 = Num2/Den2;

            dB = 20.0 * log10(M1.Mag());
            ph1 = M1.Phase(Last1,PhaseCrossings1);
            ph2 = M2.Phase(Last2,PhaseCrossings2);
            FreqP.AddPoint(f);
            MagP.AddPoint(dB);
            PhaseP.AddPoint(ph1/Pi*180);
            PhaseP2.AddPoint(ph2/Pi*180);
            DelayP.AddPoint((ph1-ph2)/Pi*180);
///            printf("%2d:Mag=%lf  Phase1=%lf Phase2 = %lf Freq=%lf Diff=%6.3lf\n",i,dB,ph1/Pi*180,ph2/Pi*180,f,(ph1-ph2)/Pi*180);
        }
        EvaluatePoints(&DelayP,89.0,91.0);
        printf("Spacing Ration = %lf\n\n",SpacingRatio);
        SpacingRatio += 0.01;
        loop--;
    }while(loop);
    Plots.AddPoints(&FreqP);
///    Plots.AddPoints(&MagP);
    Plots.AddPoints(&PhaseP);
    Plots.AddPoints(&PhaseP2);
    Plots.AddPoints(&DelayP);
    Plots.SetHieght(PlotHieght);
    MagP.Scale(PlotHieght);
    DelayP.Scale(PlotHieght);
    ///we are not going to auto scale the phase plots
    PhaseP2.Scale(PlotHieght,-1260.0,0);
    PhaseP.Scale(PlotHieght,-1260.0,0.0);
    Plots.Plot(stdout,"90 Degree Phase Shift",0);
    return 0;
}
