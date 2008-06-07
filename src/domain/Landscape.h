#ifndef LANDSCAPE_H_
#define LANDSCAPE_H_

#include <vector>
#include "Point.h"

using std::vector;

namespace domain
{

enum Direction
{
    DIRECTION_NORTH, DIRECTION_SOUTH, DIRECTION_EAST, DIRECTION_WEST
};

class Landscape
{
public:
	Landscape( int grid_width,
               float x, float y, float z,
               float width, float depth, float height );
	virtual ~Landscape();
	
	Point& operator ()( int row, int column );
	Point& operator ()( float x, float z );
	vector<float> getColour( int row, int column);
	vector<float> getNormal( int row, int column);
	
	int getGridWidth();
	float getWidth();
	float getHeight();
	float getDepth();
	
	float getBoundary( Direction direction );
	
	void generate( int grid_width,
	               float x, float y, float z,
	               float width, float depth, float height );
	void regenerate();

protected:
    Point position;
    
    int gridWidth;
    float gridDivisionWidth, // The north-south distance between grid points
          gridDivisionDepth; // The east-west distance between grid points
    
    float northBound, southBound, eastBound, westBound;
    float minHeight, maxHeight;
    
    float width,  // The east-west distance across the world
          depth,  // The north-south distance across the world
          height; // The distance between the highest point and lowest point
    
    vector<float>** colorMap;
    vector<float>** normalMap;
    vector<Point>** pointMap;
};

} // End namespace `domain'

#endif /*LANDSCAPE_H_*/
