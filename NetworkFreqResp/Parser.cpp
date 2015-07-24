/***************************************
** These function parse the input file
***************************************/

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "Component.h"

static int UnGetBuffer=0;
static int Line;
static char LexBuff[256];
static int LexBuffIndex=0;
static int LookAhead=0;

//********************************************
// Accept
//
// This function compares the token you want
// versus the token that is current.  If they
// match, then we get another token.
// If not, then just return.
//
// parameter:
//	token.....this is the token we want
//
// return value:
//	returns 1 if we find the token we wanted
//	returns 0 if we don't get the token we want
//**********************************************

int Accept(int token,FILE *pI)
{
	int rv;

	if(token == LookAhead)
	{
		LookAhead = Lex(pI);
		rv = 1;
	}
	else
		rv = 0;
	return rv;
}

//**********************************************
// Expect
//
// This function is used to check that we get
// the token that we Expect.
//
// parameters:
//	token....this is the token we Expect
// return value:
//	reutns 1 if we got what we Expected
//	reutnrs 0 if we did not get what we Expected
//*********************************************

int Expect(int token,FILE *pI)
{
	int rv;

	if((rv = Accept(token,pI))==0)\
	{
		fprintf(stderr,"*** ERROR *** Expect:UnExpected Token %d instead of %d\n",token,LookAhead);
		exit(1);
	}
	return rv;
}


int yygetC(FILE *in)
{
    int rV;

    if(UnGetBuffer)
    {
        rV = UnGetBuffer;
        UnGetBuffer = 0;
    }
    else
    {
        rV = fgetc(in);
    }
    return rV;
}

bool IsNumberDigit(int c)
{
	bool rV = FALSE;
	switch(c)
	{
		case '0':  case '1':	case '2':
		case '3':	case '4':	case '5':
		case '6':	case '7':	case '8':
		case '9':	case 'e':	case 'E':
		case '-':	case '+':	case '.':
			rV = TRUE;
			break;
		default:
			rV = FALSE;
	}
	return rV;
}

int Lex(FILE *in)
{
    int rV=0;
    int c;
    int loop = 1;

    while(loop)
    {
		c = yygetC(in);
        switch(c)
        {
            case ' ':   /// white space
                break;
            case '\n':
                Line++;
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            	LexBuffIndex = 0;
            	rV = TOKEN_NUMBER;
            	LexBuff[LexBuffIndex++] = c;
            	while(IsNumberDigit(c = yygetC(in)))
				{
					LexBuff[LexBuffIndex++] = c;
					if(c == 'e' || c == 'E' || c == '.') rV = TOKEN_FLOAT;
				}
				UnGetBuffer = c;
				LexBuff[LexBuffIndex] = 0;
				loop = 0;	//found token
                break;
			case 'C':
				rV = TOKEN_CAPACITOR;
				loop = 0;
				break;
			case 'L':
				rV = TOKEN_INDUCTOR;
				loop = 0;
				break;
			case 'R':
				rV = TOKEN_RESISTOR;
				loop = 0;
				break;
			case 'N':
				rV = TOKEN_NODES;
				loop = 0;
				break;
			case 'I':
				rV = TOKEN_INPUT;
				loop = 0;
				break;
			case 'G':
				rV = TOKEN_GROUND;
				loop = 0;
				break;
			case 'O':
				rV = TOKEN_OUTPUT;
				loop = 0;
				break;
			case 'S':
				rV = TOKEN_SWEEP;
				loop = 0;
				break;
			case 'A':		///could not come up with
				rV = TOKEN_VCCS;	///a better letter
				loop = 0;	///for a VCCS element
				break;
			case EOF:
				rV = TOKEN_EOF;
				loop = 0;
				break;
			default:
				fprintf(stderr,"Unknown Token %c\n",c);
				break;

        }
    }
    return rV;
}

Component *MakeNewComponent(Component **ppL, Component **ppE)
{
	Component *pNC = new Component();
	if(*ppL)
	{
		(*ppE)->SetNext(pNC);
		*ppE = pNC;
	}
	else
	{
		*ppL = pNC;
		*ppE = pNC;
	}
	return pNC;
}

int Parse(FILE *in,Component **ppList,Component **ppEnd)
{
	///-------------------------------------------------
	/// Parse
	///		This function reads in the Circuit File
	/// and creates a list of components
	///
	/// parameters:
	///		in..........pointer to input circuit file
	///		ppList.......Pointer to component list
	///		ppEnd.......Pointer to the End of List pointer
	/// Return Value
	///		Returns Negative on fail
	///		Returns number of Nodes on Success
	///--------------------------------------------------
    Line = 1;
    int loop;
	int Nodes = 0;
	Component *pC;
	int N1,N2,N3,N4;
	double v;

	LexBuff[0] = 0;

    LookAhead = Lex(in);
    loop = 1;
    while(loop)
	{
		switch(LookAhead)
		{
			case TOKEN_CAPACITOR:
				pC = MakeNewComponent(ppList,ppEnd);
				Accept(TOKEN_CAPACITOR,in);
				N1 = atoi(LexBuff);
				Expect(TOKEN_NUMBER,in);
				N2 = atoi(LexBuff);
				Expect(TOKEN_NUMBER,in);
				v = atof(LexBuff);
				Expect(TOKEN_FLOAT,in);
				pC->Set('C',v,N1,N2);
				break;
			case TOKEN_INDUCTOR:
				pC = MakeNewComponent(ppList,ppEnd);
				Accept(TOKEN_INDUCTOR,in);
				N1 = atoi(LexBuff);
				Expect(TOKEN_NUMBER,in);
				N2 = atoi(LexBuff);
				Expect(TOKEN_NUMBER,in);
				v = atof(LexBuff);
				Expect(TOKEN_FLOAT,in);
				pC->Set('L',v,N1,N2);
				break;
			case TOKEN_NODES:
				pC = MakeNewComponent(ppList,ppEnd);
				Accept(TOKEN_NODES,in);
				Nodes = atoi(LexBuff);
				pC->Set('N',0.0,Nodes);
				Expect(TOKEN_NUMBER,in);
				break;
			case TOKEN_RESISTOR:
				pC = MakeNewComponent(ppList,ppEnd);
				Accept(TOKEN_RESISTOR,in);
				N1 = atoi(LexBuff);
				Expect(TOKEN_NUMBER,in);
				N2 = atoi(LexBuff);
				Expect(TOKEN_NUMBER,in);
				v = atof(LexBuff);
				Expect(TOKEN_FLOAT,in);
				pC->Set('R',v,N1,N2);
				break;
			case TOKEN_INPUT:
			case TOKEN_OUTPUT:
			case TOKEN_GROUND:
				N2 = LookAhead;
				pC = MakeNewComponent(ppList,ppEnd);
				Accept(LookAhead,in);
				N1 = atoi(LexBuff);
				Expect(TOKEN_NUMBER,in);
				switch(N2)
				{
					case TOKEN_INPUT:
						pC->Set('I',0.0,N1);
						break;
					case TOKEN_OUTPUT:
						pC->Set('O',0.0,N1);
						break;
					case TOKEN_GROUND:
						pC->Set('G',0.0,N1);
						break;
				}
				break;
			case TOKEN_SWEEP:
				pC = MakeNewComponent(ppList,ppEnd);
				Accept(TOKEN_SWEEP,in);
				v = atof(LexBuff);		///start frequency
				Expect(TOKEN_FLOAT,in);
				N1 = atoi(LexBuff);	///number of decades
				Expect(TOKEN_NUMBER,in);
				N2 = atoi(LexBuff);	///steps per decade
				Expect(TOKEN_NUMBER,in);
				pC->Set('S',v,N1,N2);
				break;
			case TOKEN_VCCS:
				pC = MakeNewComponent(ppList,ppEnd);
				Accept(TOKEN_VCCS,in);
				N1 = atoi(LexBuff);	///+ input node
				Expect(TOKEN_NUMBER,in);
				N2 = atoi(LexBuff);	///- input node
				Expect(TOKEN_NUMBER,in);
				N3 = atoi(LexBuff);	///- output node
				Expect(TOKEN_NUMBER,in);
				N4 = atoi(LexBuff);	///+output node
				Expect(TOKEN_NUMBER,in);
				v = atof(LexBuff);		///trans conductance
				Expect(TOKEN_FLOAT,in);
				pC->Set('A',v,N1,N2,N3,N4);
				break;
			case TOKEN_EOF:
				loop = 0;
				break;
			default:
				fprintf(stderr,"Unexpected Token %d\n",LookAhead);
				exit(1);
				break;
		}
	}
    return Nodes;
}
