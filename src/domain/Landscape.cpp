#include "Landscape.h"

namespace domain
{

Landscape::Landscape( int grid_width,
                      float south_bound, float west_bound,
                      float east_bound, float north_bound )
{
    gridWidth = grid_width;
    southBound = south_bound;
    westBound = west_bound;
    eastBound = east_bound;
    northBound = north_bound;
    depth = northBound - southBound;
    width = eastBound - westBound;
    
    // Create point map
    pointMap = new vector<Point>*[gridWidth];
    // Create color map
    colorMap = new vector<float>*[gridWidth];
    // Create normal map
    normalMap = new vector<float>*[gridWidth];
    // Allocate the memory
    for( int row = 0; row < gridWidth; row++ )
    {
        pointMap[row] = new vector<float>[gridWidth];
        colorMap[row] = new vector<float>[gridWidth];
        normalMap[row] = new vector<float>[gridWidth];
    }
    
    
    // Generate landscape
}

Landscape::~Landscape()
{
}

}
