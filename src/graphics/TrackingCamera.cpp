#include "TrackingCamera.h"

namespace graphics
{

TrackingCamera::TrackingCamera( Object* subject )
{
    setSubject(subject);
}

TrackingCamera::~TrackingCamera()
{
}

void TrackingCamera::setSubject( Object* subject )
{
    if( subject == NULL )
    {
        throw "Tracking camera must have a subject.";
    }
    
    this->subject = subject;
}

void TrackingCamera::update( float delta )
{
    // For now, just give the camera the perspective of its subject
    
}


}
