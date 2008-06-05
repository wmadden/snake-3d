#ifndef LANDSCAPE_H_
#define LANDSCAPE_H_

#include <vector>
#include "Point.h"

using std::vector;

namespace domain
{

class Landscape
{
public:
	Landscape( int gridWidth = 257,
               float southBound = -1.0, float westBound = -1.0,
               float depth = 2.0, float height = 2.0 );
	virtual ~Landscape();
	
	void regenerate();

protected:
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

}

#endif /*LANDSCAPE_H_*/
