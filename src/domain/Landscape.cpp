#include "Landscape.h"

namespace domain
{

Landscape::Landscape( int grid_width,
                      float x, float y, float z,
                      float width, float depth, float height )
{
    gridWidth = 0;
    pointMap = NULL;
    normalMap = NULL;
    colorMap = NULL;
    
    // Generate landscape
    generate( grid_width,
              x, y, z,
              width, depth, height );
}

Landscape::~Landscape()
{
    // Free allocated memory
    freeMaps();
}

Point Landscape::operator ()( int row, int column )
{
    return (*(*pointMap)[row])[column];
}

Point Landscape::operator ()( float x, float z )
{
    int row = x / gridDivisionWidth,
        column = z / gridDivisionDepth;
    return (*(*pointMap)[row])[column];
}

vector<int> Landscape::getClosestPoint( float x, float z )
{
    vector<int> coords(2);
    
    coords.push_back( (int)(x / gridDivisionWidth) );
    coords.push_back( (int)(z / gridDivisionDepth) );
    
    return coords;
}

vector<float>* Landscape::getColor( int row, int column)
{
    return (*(*colorMap)[row])[column];
}

Point Landscape::getNormal( int row, int column)
{
    return (*(*normalMap)[row])[column];
}

int Landscape::getGridWidth()
{
    return gridWidth;
}

float Landscape::getWidth()
{
    return width;
}

float Landscape::getHeight()
{
    return height;
}

float Landscape::getDepth()
{
    return depth;
}

float Landscape::getBoundary( Direction direction )
{
    switch( direction )
    {
    case DIRECTION_EAST:
        return eastBound;
    case DIRECTION_NORTH:
        return northBound;
    case DIRECTION_SOUTH:
        return southBound;
    case DIRECTION_WEST:
        return westBound;
    }
    
    throw "Execution should never reach here.";
}

void Landscape::regenerate()
{
    generate( gridWidth,
              position.x, position.y, position.z,
              width, depth, height );
}

void Landscape::freeMaps()
{
    // Free allocated memory
    vector< vector<Point>* >::iterator pointit = pointMap->begin(),
                                      normit = normalMap->begin();
    vector< vector< vector<float>* >* >::iterator colorit = colorMap->begin();
    
    for( int row = 0; row < gridWidth; row++ )
    {
        delete *pointit;
        delete *normit;
        delete *colorit;
        
        pointit++;
        normit++;
        colorit++;
    }
}

void Landscape::allocateMaps( int grid_width )
{
    if( grid_width != gridWidth )
    {
        freeMaps();
        gridWidth = grid_width;
    }
    
    // Create point map
    pointMap = new vector< vector<Point>* >(gridWidth);
    // Create color map
    colorMap = new vector< vector< vector<float>* >* >(gridWidth);
    // Create normal map
    normalMap = new vector< vector<Point>* >(gridWidth);
    
    // Allocate the memory
    for( int row = 0; row < gridWidth; row++ )
    {
        pointMap->push_back( new vector<Point>(gridWidth) );
        colorMap->push_back( new vector< vector<float>* >(gridWidth) );
        normalMap->push_back( new vector<Point>(gridWidth) );
    }
}

void Landscape::generate( int grid_width,
                          float x, float y, float z,
                          float width, float depth, float height )
{
    allocateMaps(grid_width);
    
    this->width = width;
    this->depth = depth;
    this->height = height;
    
    this->position.x = x;
    this->position.y = y;
    this->position.z = z;
    
    southBound = z - depth / 2.0;
    westBound = x - width / 2.0;
    eastBound = westBound + width;
    northBound = southBound + depth;
    
    gridDivisionDepth = depth / gridWidth;
    gridDivisionWidth = width / gridWidth;
    
    minHeight = y - height / 2.0f;
    maxHeight = minHeight + height;
    
    // TODO: generate landscape
}



}
