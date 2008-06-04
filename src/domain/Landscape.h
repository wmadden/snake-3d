#ifndef LANDSCAPE_H_
#define LANDSCAPE_H_

#include <vector>

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
    
    float northBound, southBound, EastBound, WestBound;
    float minHeight, maxHeight;
    float worldWidth, // The east-west distance across the world
          worldDepth, // The north-south distance across the world
          gridDivisionWidth, // The north-south distance between grid points
          gridDivisionDepth; // The east-west distance between grid points
    
    vector<float>** colorMap;
    vector<float>** normalMap;
    vector<float>** pointMap;
};

}

#endif /*LANDSCAPE_H_*/
