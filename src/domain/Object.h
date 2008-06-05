#ifndef OBJECT_H_
#define OBJECT_H_

#include "Point.h"

namespace domain
{

class Object
{
public:
	Object();
	virtual ~Object();
	
	Point getPosition();
	Point getVelocity();
	float getRadius();
	void setVelocity( float x, float y, float z );
	void setPosition( float x, float y, float z );
	void setRadius( float radius );
	
protected:
    Point position;
    float radius;
    Point velocity;
};

}

#endif /*OBJECT_H_*/
