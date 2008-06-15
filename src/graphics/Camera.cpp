#include "Camera.h"

namespace graphics
{

Camera::Camera( float* position, float* forward, float* up )
{
    if( position )
    {
        this->position[0] = position[0];
        this->position[1] = position[1];
        this->position[2] = position[2];
    }
    else
    {
        this->position[0] = this->position[1] = this->position[2] = 0;
    }
    
    if( forward )
    {
        this->forward[0] = forward[0];
        this->forward[1] = forward[1];
        this->forward[2] = forward[2];
    }
    else
    {
        this->forward[0] = this->forward[1] = this->forward[2] = 0;
    }

    if( up )
    {
        this->up[0] = up[0];
        this->up[1] = up[1];
        this->up[2] = up[2];
    }
    else
    {
        this->up[0] = this->up[1] = this->up[2] = 0;
    }
}

Camera::~Camera()
{
}

float* Camera::getPosition()
{
    return position;
}

void Camera::setPosition( float x, float y, float z )
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
}

float* Camera::getForwardVector()
{
    return forward;
}

void Camera::setForwardVector( float i, float j, float k )
{
    forward[0] = i;
    forward[1] = j;
    forward[2] = k;
}

float* Camera::getUpVector()
{
    return up;
}

void Camera::setUpVector( float i, float j, float k )
{
    up[0] = i;
    up[1] = j;
    up[2] = k;
}

}
