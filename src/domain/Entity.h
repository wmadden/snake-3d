#ifndef ENTITY_H_
#define ENTITY_H_

#include "Object.h"

namespace domain
{

class Entity : public domain::Object
{
public:
	Entity();
	virtual ~Entity();
};

}

#endif /*ENTITY_H_*/
