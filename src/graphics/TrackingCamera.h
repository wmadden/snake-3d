#ifndef TRACKINGCAMERA_H_
#define TRACKINGCAMERA_H_

#include "Camera.h"
#include "domain/Object.h"

using domain::Object;

namespace graphics
{

class TrackingCamera : public graphics::Camera
{
public:
	TrackingCamera( Object* subject );
	virtual ~TrackingCamera();
	
	void setSubject( Object* );
	void update( float delta );

protected:
    Object* subject;
};

}

#endif /*TRACKINGCAMERA_H_*/
