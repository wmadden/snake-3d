#ifndef BODY_H_
#define BODY_H_

#include "Object.h"

namespace domain
{

class Body;

class Body : public domain::Object
{
public:
	Body();
	virtual ~Body();

	Body* getNext();
	Body* getPrev();
	
	Body* pushAhead( Body* body );
	Body* pushBehind( Body* body );
	
	void pop();
protected:
	Body *next, *prev;
};

}

#endif /*BODY_H_*/
