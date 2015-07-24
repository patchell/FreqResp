#ifndef COMPONENT_H
#define COMPONENT_H

class CMatrix;

class Component
{
	char Type;
	int Node1;
	int Node2;
	int Node3;
	int Node4;
	double Value;
	Component *Next;
	Component *Prev;
public:
	Component();
	Component(char T, double V,int N1,int N2, int N3=0,int N4=0);
	virtual ~Component();
	Component(const Component& other);
	bool IsType(char T){return Type == T;}
	char GetType(void){return Type;}
	int GetNode1(void){return Node1;}
	int GetNode2(void){return Node2;}
	int GetNode3(void){return Node3;}
	int GetNode4(void){return Node4;}
	double GetValue(void){return Value;}
	void Set(char T, double V,int N1,int N2=0, int N3=0,int N4=0);
	Component *GetNext(void){return Next;}
	Component *GetPrev(void){return Prev;}
	void SetNext(Component *n){Next = n;}
	void SetPrev(Component *p){Prev = p;}
	void AddCompoentsToMatrix(CMatrix Y,double w);
	int GetOutputNode( void);
	int GetGroundNode( void);
	int GetInputNode(void);
	int GetTotalNodes(void);
	int GetSweepParams( double &StartFreq,int &Decades,int &Steps);
	void Print(FILE *pO);
protected:
private:
};

extern void PrintComponentList(Component *pL,FILE *pO);

#endif // COMPONENT_H
