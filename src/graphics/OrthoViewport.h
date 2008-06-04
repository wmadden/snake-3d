#ifndef ORTHOVIEWPORT_H_
#define ORTHOVIEWPORT_H_

#include "Viewport.h"

namespace graphics
{

class OrthoViewport : public graphics::Viewport
{
public:
    OrthoViewport( int width, int height, 
                   float top, float bottom,
                   float left, float right,
                   int x = DEFAULT_X,
                   int y = DEFAULT_Y,
                   float near_z = DEFAULT_NEAR_Z,
                   float far_z = DEFAULT_FAR_Z );
	virtual ~OrthoViewport();

	float getTop();
	float getLeft();
	float getBottom();
	float getRight();
	
	void setTop( float top );
	void setLeft( float left );
	void setBottom( float bottom );
	void setRight( float right );
	
	void apply();
	
protected:
    float top, left, bottom, right;
};

}

#endif /*ORTHOVIEWPORT_H_*/
