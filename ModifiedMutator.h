#ifndef MODIFIEDMUTATOR_H
#define MODIFIEDMUTATOR_H

#include <functional>


class ModifiedTemplate
{
protected:
	std::function<void()> ObjectModificationLambda = NULL;
	void ObjectWasModified()
	{
		if (ObjectModificationLambda != NULL)
		{
			ObjectModificationLambda();
		}
	};
public:
	void SetObjectModificationLambda(std::function<void()> &in)
	{
		ObjectModificationLambda=in;
	};
	void SetObjectModificationLambda(std::function<void()> in)
	{
		ObjectModificationLambda=in;
	};
	void DropObjectModificationLambda()
	{
		ObjectModificationLambda=NULL;
	};
	std::function<void()> GetObjectModificationLambda()
	{
		return ObjectModificationLambda;
	};
};


#endif

