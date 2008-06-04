#include "OrthoViewport.h"
#include <GL/glut.h>

namespace graphics
{

OrthoViewport::OrthoViewport( int width, int height, 
                              float top, float bottom,
                              float left, float right,
                              int x,
                              int y,
                              float near_z,
                              float far_z )
             : Viewport( width, height, x, y, near_z, far_z )
{
    this->left = left; this->right = right;
    this->top = top; this->bottom = bottom;
}

OrthoViewport::~OrthoViewport()
{
}

void OrthoViewport::apply()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport( x, y, width, height );

    glOrtho( left, right, bottom, top, nearZ, farZ );
}

}
