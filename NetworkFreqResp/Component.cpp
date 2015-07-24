#include <stdio.h>
#include "Component.h"
#include "CMatrix.h"

Component::Component()
{
	Type = 0;
	Value = 0.0;
	Node1 = 0;
	Node2 = 0;
	Node3 = 0;
	Node4 = 0;
	Next = 0;
	Prev = 0;
}

Component::Component(char T, double V,int N1,int N2, int N3,int N4)
{
	Type = T;
	Value = V;
	Node1 = N1;
	Node2 = N2;
	Node3 = N3;
	Node4 = N4;
	Next = 0;
	Prev = 0;
}

Component::~Component()
{
	//dtor
}

Component::Component(const Component& other)
{
	//copy ctor
}

void Component::Set(char T, double V,int N1,int N2, int N3,int N4)
{
	Type = T;
	Value = V;
	Node1 = N1;
	Node2 = N2;
	Node3 = N3;
	Node4 = N4;
}

void Component::Print(FILE *pO)
{
	switch(this->Type)
	{
		case 'R': case 'C': case 'L':
			fprintf(pO,"%c::%lf  %d %d\n",this->Type,Value,Node1,Node2);
			break;
		case 'G':	case 'I':	case 'O':
			fprintf(pO,"%c::%d\n",this->Type,Node1);
			break;
		case 'S':
			fprintf(pO,"Sweep:%7.3lfHz  Decades=%d  Steps/Dec=%d\n",Value,Node1,Node2);
			break;
	}
}

void Component::AddCompoentsToMatrix(CMatrix Y,double w)
{
	Component *pC = this;;
	Complex a;
	int flag = 0;

	while(pC)
	{
		switch(pC->GetType())
		{
			case 'C':
				a = Complex(pC->GetValue(),0.0 ) * Complex(0.0,w);	///farads
				flag = 1;
				break;
			case 'L':
				a = Complex(0.0,pC->GetValue() *w);	///henrys
				a = Complex(1.0,0.0)/a;
				flag = 1;
				break;
			case 'R':
				a = Complex(1.0/pC->GetValue(),0.0);	///ohms
				flag = 1;
				break;
			case 'A':		///VCCS (active element)
				a = Complex(pC->GetValue(),0.0);	//Gm
				flag = 2;
				break;
			default:
				flag = 0;
				break;
		}
		if(flag == 1)
		{
			int i = pC->GetNode1();
			int j = pC->GetNode2();
			Y(i,i) = Y(i,i) + a;
			Y(j,j) = Y(j,j) + a;
			Y(i,j) = Y(i,j) - a;
			Y(j,i) = Y(j,i) - a;
			flag = 0;
		}
		else if (flag == 2)	///active element
		{
			int i = pC->GetNode3();	///- outut node
			int j = pC->GetNode4();	/// + outut node
			int k = pC->GetNode1();	///+ input node
			int l = pC->GetNode2();	///- input node
			Y(i,k) = Y(i,k) + a;
			Y(j,l) = Y(j,l) + a;
			 Y(j,k) = Y(j,k) -a;
			 Y(i,l) = Y(i,l) - a;
			flag = 0;
		}
		pC = pC->GetNext();
	}
}

int Component::GetOutputNode( )
{
	int rV = 0;	///failed
	int loop = 1;
	Component *pCL = this;

	while(pCL && loop)
	{
		if(pCL->GetType() == 'O')
		{
			rV = pCL->GetNode1();
			loop = 0;
		}
		else pCL = pCL->GetNext();
	}
	return rV;
}

int Component::GetInputNode()
{
	Component *pCL = this;
	int rV = 0;	///failed
	int loop = 1;

	while(pCL && loop)
	{
		if(pCL->GetType() == 'I')
		{
			rV = pCL->GetNode1();
			loop = 0;
		}
		else pCL = pCL->GetNext();
	}
	return rV;
}

int Component::GetSweepParams( double &StartFreq,int &Decades,int &Steps)
{
	int rV = 1;	///failed
	Component *pCL = this;

	while(pCL && rV)
	{
		if(pCL->GetType() == 'S')
		{
			StartFreq = pCL->GetValue();
			Decades = pCL->GetNode1();
			Steps = pCL->GetNode2();
			rV = 0;	///success
		}
		else pCL = pCL->GetNext();
	}
	return rV;
}

int Component::GetGroundNode( )
{
	Component *pCL = this;
	int rV = 0;	///failed
	int loop = 1;

	while(pCL && loop)
	{
		if(pCL->GetType() == 'G')
		{
			rV = pCL->GetNode1();
			loop = 0;
		}
		else pCL = pCL->GetNext();
	}
	return rV;
}

int Component::GetTotalNodes()
{
	Component *pCL = this;
	int rV = 0;	///failed
	int loop = 1;

	while(pCL && loop)
	{
		if(pCL->GetType() == 'N')
		{
			rV = pCL->GetNode1();
			loop = 0;
		}
		else pCL = pCL->GetNext();
	}
	return rV;
}

void PrintComponentList(Component *pL,FILE *pO)
{
	Component *pC = pL;

	while(pC)
	{
		pC->Print(pO);
		pC = pC->GetNext();
	}
}
