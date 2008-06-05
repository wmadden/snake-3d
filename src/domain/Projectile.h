#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "Object.h"

namespace domain
{

class Projectile : public domain::Object
{
public:
	Projectile();
	virtual ~Projectile();
};

}

#endif /*PROJECTILE_H_*/
