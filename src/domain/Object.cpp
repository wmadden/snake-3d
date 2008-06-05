#include "Object.h"

namespace domain
{

Object::Object()
{
}

Object::~Object()
{
}

Point Object::getPosition()
{
    return position;
}

Point Object::getVelocity()
{
    return velocity;
}

float Object::getRadius()
{
    return radius;
}

void Object::setVelocity( float x, float y, float z )
{
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void Object::setPosition( float x, float y, float z )
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void Object::setRadius( float radius )
{
    this->radius = radius;
}


} // End namespace `domain'
