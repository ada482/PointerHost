#ifndef POINTERS_H
#define POINTERS_H
#include <deque>
#include <cassert>
#include <iterator>
#include <functional>


#include "ModifiedMutator.h"

template <class T> class ControlPointer;
template <class T> class PublicPointer;
template <class T> class PointerContainer;

template <class T> class ControlPointer
{
private:
	std::deque<PublicPointer<T>*> pubs;
	T data; 
	void SetModLambda()
	{
		data.DropObjectModificationLambda();
		data.SetObjectModificationLambda( [&](){this->valueModified(); } );
	};
public:
	ControlPointer()
	{
		SetModLambda();
	};
	ControlPointer(T &in)
	{
		data = in;
		SetModLambda();	
	};
	typename std::deque<PublicPointer<T>*>::iterator findRegisteredPtr(PublicPointer<T> &in)
	{
		typename std::deque<PublicPointer<T>*>::iterator ret;
		for (	ret = pubs.begin(); ( (ret != pubs.end()) && (*ret!=&in) ); ret++)
		{
			//Breaks if found
		}
		return ret;
	};
	void registerPtr(PublicPointer<T> &in)
	{
		pubs.push_back(&in);	
	};
	void unregisterPtr(PublicPointer<T> &in)
	{
		typename std::deque<PublicPointer<T>*>::iterator hold;
		hold = findRegisteredPtr(in);
		
		if(hold == pubs.end())
		{
			//TODO: Throw
		}
		else
		{
			pubs.erase(hold);
		}	
	};
	void valueModified()
	{
		if(pubs.size() < 1) 
			return;
		
		
		typename std::deque<PublicPointer<T>*>::iterator ret;
		
		for(ret = pubs.begin(); ret != pubs.end(); ret++)
		{
			(*ret)->doMod();
		}
	};
	void valueDeleted()
	{
		if(pubs.size() < 1) 
			return;
		
		typename std::deque<PublicPointer<T>*>::iterator ret = pubs.begin();

		while(ret != pubs.end() && (pubs.size() > 0))
		{
			(*ret)->doDel();
			//if(ret == pubs.begin())
			{
				//pubs.erase(ret);
			}
			ret = pubs.begin();

		}
	};
	T get()
	{
		return data;
	};
	void get(T& out)
	{
		out = data;
	};
	template <class X> void set(const X& in)
	{
		data = in;
	};
	void swap(T& in)
	{
		T temp = in;
		in = data;
		data = temp;
	};
	~ControlPointer()
	{
		valueDeleted();
	};
};

template <class T> class PublicPointer
{
	friend class PointerContainer<T>;
	
private:
	ControlPointer<T>* host;
	std::function<void()>  modFun;
	std::function<void()>  delFun;
public:
	void init()
	{
		host = NULL;
		modFun = NULL; 
		delFun = NULL; 
	};
	void unsetPtr()
	{
		host = NULL;
	};
	void unsetDel()
	{
		delFun = NULL;
	};
	void unsetMod()
	{
		modFun = NULL;
	};
	void setPtr(ControlPointer<T> &in)
	{
		if(host != NULL)
			host->unregisterPtr(*this);
		host = &in;
		if(host != NULL)
		{
			host->registerPtr(*this);
		}
	};
	void setDel(std::function<void()> &in)
	{
		delFun = in;
	};
	void setDel(std::function<void()> in)
	{
		delFun = in;
	};
	void setMod(std::function<void()> &in)
	{
		modFun = in;
	};
	void setMod(std::function<void()> in)
	{
		modFun = in;
	};
	ControlPointer<T>* getPtr()
	{
		return host;
	};
	std::function<void()> getDel()
	{
		return delFun;
	};
	std::function<void()> getMod()
	{
		return modFun;
	};
	void doDel()
	{
		if(delFun!=NULL)
		{
			delFun();
			unsetDel();
		}
		if(host != NULL)
		{
			host->unregisterPtr(*this);
			unsetPtr();
		}
	};
	void doMod()
	{
		if(modFun!=NULL)
			modFun();
	};
	
	
	PublicPointer()
	{
		init();
	};
	PublicPointer(ControlPointer<T> &in)
	{
		init();
		setPtr(in);
	};
	PublicPointer(ControlPointer<T> &inP, std::function<void()> &inD)
	{
		init();
		setPtr(inP);
		setDel(inD);
	}; 
	PublicPointer(ControlPointer<T> &inP, std::function<void()> &inD, std::function<void()> &inM)
	{
		init();
		setPtr(inP);
		setDel(inD);
		setMod(inM);
	}; 
	PublicPointer(const PublicPointer &old)
	{
		init();
		setPtr( *(old.host));
		modFun = old.modFun;
		delFun = old.delFun;
		
	};
	~PublicPointer()
	{
		doDel();
		if(host != NULL)
			host->unregisterPtr(*this);
		unsetPtr();
	};
};


template<class T> class PointerContainer
{
private:
	std::deque<PublicPointer<T>> data;

public:
	void init()
	{
		
	};
	typename std::deque<PublicPointer<T>>::iterator findPtr(long &in)
	{
		if (data.size() < 1 || data.size() <= in || in < 0)
			return data.end();
		std::iterator<std::random_access_iterator_tag,PublicPointer<T>> ret;
		ret = data.begin();
		return ret+in;			
	};
	
	typename std::deque<PublicPointer<T>>::iterator findPtr(const PublicPointer<T> &in)
	{
		typename std::deque<PublicPointer<T>>::iterator ret;
		for (ret = data.begin(); ((ret != data.end()) && (&(*ret) != &in) ); ret++)
		{
			
		}
		return ret;			
	};
	
	void hostDeleted(PublicPointer<T> &in)
	{
		typename std::deque<PublicPointer<T>>::iterator temp = findPtr(in);
		if(temp != data.end())
		{
			(*temp).unsetDel();
			(*temp).unsetMod();
			data.erase(temp);
		}
		else
		{
		}
	};
	
	/*
	
	TODO:
	          value_t& operator[](std::size_t idx)       { return mVector[idx]; };
const value_t& operator[](std::size_t idx) const { return mVector[idx]; };

	T& operator[](long &in)
	{
		assert(in>-1);
		assert(in<data.size());
		typename std::deque<PublicPointer<T>>::iterator it = findPtr(in);
		return (*(*it));
	};
	*/
	
	
	void insert(PublicPointer<T> &in)
	{
		typename std::deque<PublicPointer<T>>::iterator temp = findPtr(in);
		if(data.end() == temp)
		{
			data.push_back(in);
			temp = data.begin() + (data.size()-1);
			auto x = &(*temp);
			temp->setDel([&](){ 
				hostDeleted(*x); 
				});
		}
		else
		{
			//TODO: throw error
		}
		
	};
	
};







#endif