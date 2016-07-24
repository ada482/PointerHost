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
		//pr(host.get());
		host.set(i);
		//pr(host.get());
	}
};

int main()
{
	char hold;
	std::cout <<"SINGLE" << std::endl;
	std::cin >> hold;
	stuff(-1);
	std::cout <<"LOOP" << std::endl;
	std::cin >> hold;
	for(long i = 0; i < 1000000; i++)
	{
		//std::cout<<"LOOP" <<std::endl;
		stuff(i);
	};
	std::cout <<"DONE" << std::endl;
	std::cin >> hold;



	
	return 0;
}