#include "Landscape.h"
#include "maths/maths.h"
#include <math.h>

#define COLOR_CHANNELS 3

using namespace maths;

namespace domain
{

Landscape::Landscape( int grid_width,
                      float x, float y, float z,
                      float width, float depth, float height,
                      float roughness )
{
    gridWidth = 0;
    pointMap = NULL;
    normalMap = NULL;
    colorMap = NULL;
    
    // Generate landscape
    generate( grid_width,
              x, y, z,
              width, depth, height,
              roughness);
}

Landscape::~Landscape()
{
    // Free allocated memory
    freeMaps();
}

Point Landscape::operator ()( int row, int column )
{
    return getPoint(row, column);
}

Point Landscape::operator ()( float x, float z )
{
    int row = x / gridDivisionWidth,
        column = z / gridDivisionDepth;
    
    return getPoint(row, column);
}

vector<int> Landscape::getClosestPoint( float x, float z )
{
    vector<int> coords(2);
    
    coords.push_back( (int)(x / gridDivisionWidth) );
    coords.push_back( (int)(z / gridDivisionDepth) );
    
    return coords;
}

void Landscape::setPointHeight( int row, int column, float height )
{
    /* Set the point's absolute height, given its relative height */
    (*(*pointMap)[row])[column].y = position.y + height;
    
    /* Update the point's colour */
    setColor( row, column, height );
    
    /* Update normals */
    // TODO
}

void Landscape::setColor( int row, int column, float height )
{
    /* Get the relative height of the point */
    height = (height - position.y) / this->height;
    
    float R, G, B;
    float a = 3.5f,
          b = 0.4f,
          c = 0.14f;
    
    G = a * pow( (height - b), 2 ) + 0.25;
    R = a * pow( (height - b), 2 ) + c;
    B = (height > b ? 1.0 : 0.8) * a * pow( (height - b), 2 );
    
    vector<float>* color = getColor( row, column );
    (*color)[1] = R;
    (*color)[2] = G;
    (*color)[3] = B;
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

Point Landscape::getPoint( int row, int column )
{
    return (*(*pointMap)[row])[column];
}

float Landscape::getPointHeight( int row, int column )
{
    return (*(*pointMap)[row])[column].y;
}

void Landscape::regenerate()
{
    generate( gridWidth,
              position.x, position.y, position.z,
              width, depth, height,
              roughness );
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
        
        // Initialize shit
        for( int column = 0; column < gridWidth; column++ )
        {
            setPointHeight( row, column, position.y );
            (*(*colorMap)[row])[column] = new vector<float>(COLOR_CHANNELS, 0);
        }
    }
}

void Landscape::generate( int grid_width,
                          float x, float y, float z,
                          float width, float depth, float height,
                          float roughness )
{
    /* Allocate the point-, normal- and colourmaps. */
    allocateMaps(grid_width);
    
    /* Set the properties of the landscape */
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
    
    this->roughness = roughness;
    
    /* Generate the landscape */
    generateTerrain();
    
    /* Generate normal vectors */
    // TODO
}

void Landscape::generateTerrain()
{
    /* Set up the four corners of the grid */
    setPointHeight( 0, 0, random(minHeight, maxHeight) );
    setPointHeight( 0, gridWidth, random(minHeight, maxHeight) );
    setPointHeight( gridWidth, 0, random(minHeight, maxHeight) );
    setPointHeight( gridWidth, gridWidth, random(minHeight, maxHeight) );
    
    /* Call the recursive generation function */
    plasmaFract( 0, 0, gridWidth, height * roughness );
}

void Landscape::plasmaFract( int row, int column, int division_width,
                             float displacement )
{
    /* Test recursion depth */
    if( division_width < 2 )
        return;
    
    /* Reduce displacement */
    displacement *= roughness;
    
    /* Find the centre point coordinates */
    int half_div_width = division_width / 2;
    int center_row = row + half_div_width,
        center_column = column + half_div_width;
    
    /* Generate midpoints */
    // Perform the diamond and square steps
    diamondStep( center_row, center_column, half_div_width, displacement );
    squareStep( row, center_column, half_div_width, displacement );
    
    /* Recurse */
    // Top left division
    plasmaFract( row, column, half_div_width, displacement );
    // Top right division
    plasmaFract( row, center_column, half_div_width, displacement );
    // Bottom left division
    plasmaFract( center_row, column, half_div_width, displacement );
    // Bottom right division
    plasmaFract( center_row, center_column, half_div_width, displacement );
}

void Landscape::displace( int row, int column, float displacement )
{
    setPointHeight( row, column,
                    getPointHeight(row, column) +
                    random(-displacement, displacement) );
}

void Landscape::diamondStep( int row, int column,
                             int width, float displacement )
{
    setPointHeight( row, column,
                    ( getPointHeight(row - width, column - width) + 
                      getPointHeight(row - width, column + width) +
                      getPointHeight(row + width, column - width) +
                      getPointHeight(row + width, column + width) ) / 4.0) ;
    displace( row, column, displacement );
}

void Landscape::squareStep( int row, int column,
                            int width, float displacement )
{
    float av_height = 0;
    int points = 0;
    
    if( row > 0 )
    {
        // Top
        av_height += getPointHeight( row - width, column );
        points++;
    }
    if( row < gridWidth )
    {
        // Bottom
        av_height += getPointHeight( row + width, column );
        points++;
    }
    
    if( column > 0 )
    {
        // Left
        av_height += getPointHeight( row, column - width );
        points++;
    }
    if( column < gridWidth )
    {
        // Right
        av_height += getPointHeight( row, column + width );
        points++;
    }
    
    av_height /= points;
    
    setPointHeight( row, column,
                    av_height ) ;
    displace( row, column, displacement );
}

} // End of namespace `domain'
