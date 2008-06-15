#ifndef CAMERA_H_
#define CAMERA_H_

#include <stdlib.h>

namespace graphics
{

class Camera
{
public:
	Camera( float* position = NULL, float* forward = NULL, float* up = NULL );
	virtual ~Camera();

	float* getPosition();
	void setPosition( float x, float y, float z );
	float* getForwardVector();
	void setForwardVector( float i, float j, float k );
	float* getUpVector();
	void setUpVector( float i, float j, float k );
	
protected:
    float position[3];
    float forward[3];
    float up[3];
};

}

#endif /*CAMERA_H_*/
