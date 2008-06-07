#include "Landscape.h"

namespace domain
{

Landscape::Landscape( int grid_width,
                      float x, float y, float z,
                      float width, float depth, float height )
{
    // Create point map
    pointMap = new vector<Point>*[gridWidth];
    // Create color map
    colorMap = new vector<float>*[gridWidth];
    // Create normal map
    normalMap = new vector<float>*[gridWidth];
    // Allocate the memory
    /*for( int row = 0; row < gridWidth; row++ )
    {
        pointMap[row] = new vector<float>[gridWidth];
        colorMap[row] = new vector<float>[gridWidth];
        normalMap[row] = new vector<float>[gridWidth];
    }*/
    
    
    // Generate landscape
    generate( grid_width,
              x, y, z,
              width, depth, height );
}

Landscape::~Landscape()
{
    // Free allocated memory
}

Point& Landscape::operator ()( int row, int column )
{
    return pointMap[row][column];
}

Point& Landscape::operator ()( float x, float z )
{
    return pointMap[(int)(x / gridDivisionWidth)][(int)(z / gridDivisionDepth)];
}

vector<float> Landscape::getColour( int row, int column)
{
    return colorMap[row][column];
}

vector<float> Landscape::getNormal( int row, int column)
{
    return normalMap[row][column];
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
}

void Landscape::regenerate()
{
    generate( gridWidth,
              position.x, position.y, position.z,
              width, depth, height );
}

void Landscape::generate( int grid_width,
                          float x, float y, float z,
                          float width, float depth, float height )
{
    gridWidth = grid_width;
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
