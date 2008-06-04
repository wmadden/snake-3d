#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#define DEFAULT_X 0
#define DEFAULT_Y 0
#define DEFAULT_NEAR_Z 0.1
#define DEFAULT_FAR_Z 100
#define DEFAULT_FOVY 100

namespace graphics
{

class Viewport
{
public:
    Viewport( int width, int height,
              int x = DEFAULT_X, int y = DEFAULT_Y,
              float near_z = DEFAULT_NEAR_Z, float far_z = DEFAULT_FAR_Z,
              float fovy = DEFAULT_FOVY );
    virtual ~Viewport();
	
	int getWidth();
	int getHeight();
	void setWidth( int width );
	void setHeight( int height );
	
	float getNearZ(); float getFarZ();
	void setNearZ( float near_z ); void setFarZ( float far_z );
	
	int getX();
	int getY();
	void setX( int X );
	void setY( int Y );
	
	virtual void apply();

protected:
    int x, y, width, height;
    float fovy;
    float nearZ, farZ;
};

}

#endif /*VIEWPORT_H_*/
