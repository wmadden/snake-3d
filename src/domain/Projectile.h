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

	float getBlastRadius();
	void setBlastRadius( float radius );
	float getDamage();
	void setDamage( float damage );
	
protected:
    float blastRadius;
    float damage;
};

}

#endif /*PROJECTILE_H_*/
