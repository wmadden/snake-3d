#include "Landscape.h"

namespace domain
{

Landscape::Landscape( int gridWidth,
                      float southBound, float westBound,
                      float depth, float height )
{
    // Create point map
    pointMap = new vector<float>*[gridWidth];
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
