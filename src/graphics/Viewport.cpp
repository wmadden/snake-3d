#include "Viewport.h"
#include <GL/glut.h>

namespace graphics
{

Viewport::Viewport( int width, int height,
                    int x, int y,
                    float near_z, float far_z,
                    float fovy )
{
    this->x = x;
    this->y = y;
    this->nearZ = near_z;
    this->farZ = far_z;
    this->fovy = fovy;
    this->width = width;
    this->height = height;
}

Viewport::~Viewport()
{
}

int Viewport::getWidth()
{
    return width;
}
int Viewport::getHeight()
{
    return height;
}
void Viewport::setWidth( int width )
{
    this->width = width;
}
void Viewport::setHeight( int height )
{
    this->height = height;
}

int Viewport::getX()
{
    return x;
}
int Viewport::getY()
{
    return y;
}
void Viewport::setX( int X )
{
    x = X;
}
void Viewport::setY( int Y )
{
    y = Y;
}

void Viewport::apply()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport( x, y, width, height );

    gluPerspective( fovy,
                    width / ((float) height), // Force float division
                    nearZ,
                    farZ );
}


}
