#ifndef EDIBLE_H_
#define EDIBLE_H_

#include "Object.h"

namespace domain
{

class Edible : public domain::Object
{
public:
	Edible();
	virtual ~Edible();
	
	float getValue();
	void setValue( float value );

protected:
    float value;
};

}

#endif /*EDIBLE_H_*/
