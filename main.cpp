#include "PointerHost.h"
#include "ModifiedMutator.h"
#include <iostream>
#include <string>

class myInt : public ModifiedTemplate
{
private:
	int x;
public:
	void set(int temp)
	{
		x=temp;
		ObjectWasModified();
	};
	myInt()
	{
		set(0);
	};
	myInt(int &temp)
	{
		set(temp);
	};
	myInt(int temp)
	{
		set(temp);
	};
	operator int() const
	{
		return x;
	};
	int operator=(int &temp)
	{
		set(temp);
	};
	int operator=(int temp)
	{
		set(temp);
	};
	operator int()
	{
		return x;
	};
	
};

void pr(int i)
{
	std::cout<<i<< '\t';
};


void stuff(const int &i)
{
	PublicPointer<myInt> pb;
	PointerContainer<myInt> pc;
	{
		ControlPointer<myInt> host;
		pb.setPtr(host);
		pb.setMod([&](){/*std::cout<<"ROGUE CHANGED\t" << i <<std::endl;*/});
		pc.insert(pb);
		pc.insert(pb);
		pc.insert(pb);
		PublicPointer<myInt> tmpPP;
		tmpPP = pc[2];
		tmpPP = myInt(i);
		//pr(host.get());
		//host.set(i);
		//pr(host.get());
		//pr( (myInt)(tmpPP) );
	}
};

int main()
{
	char hold;
	std::cout <<"SINGLE" << std::endl;
	//std::cin >> hold;
	stuff(-1);
	std::cout <<"LOOP" << std::endl;
	std::cin >> hold;
	for(long i = 0; i < 100000000; i++)
	{
		if( i%1000000 == 0)
			std::cout << i << std::endl;
		//std::cout<<"LOOP" <<std::endl;
		stuff(i);
	};
	std::cout <<"DONE" << std::endl;
	std::cin >> hold;



	
	return 0;
}
