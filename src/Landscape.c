#include "Landscape.h"
#include <stdlib.h>
#include "maths.h"
#include <math.h>

/*******************************************************************************
 * GLOBALS AND CONSTANTS
 ******************************************************************************/
#define MINGRIDSIZE 1
#define ROUGHNESS 0.5

#define THRES_SNOW 0.9
#define THRES_MOUNT 0.6


/*******************************************************************************
 * INTERNAL FUNCTION PROTOTYPES
 ******************************************************************************/
Normal* Normal_new( float X, float Y, float Z );
Point* Point_new( float X, float Y, float Z );
Color* Color_new( float, float, float, float );
ColorMap* ColorMap_new( int width );
PointMap* PointMap_new( int width );
NormalMap* NormalMap_new( int width );
float bounded_random( float max, float min );
float displace( float rand_effect_size );
void fractalIteration( int Ax, int Ay, int fsize,float rand_effect_size,
                       Landscape* landscape );
void set_point( Landscape* landscape, int row, int column,
                float X, float Y, float Z );
/**
 * A convenience function to set the height of a point
 */
void set_height( Landscape* landscape, int row, int column,
                 float height );
void set_color( Landscape* landscape, int row, int column,
                float height );
void set_normal( Landscape* landscape, int row, int column,
                 float X, float Y, float Z );
/**
 * Set the normal at grid point (X, Z)
 * 
 * Preconditions:
 *   1. Left, top, bottom, right will be NULL if there is no point.
 */
void calculate_vertex_normal( Landscape* landscape, int row, int column );
void calculate_normal( Point P1, Point P2, Point Centre, Point* normalvector );
void calcAverageNormal2( float n1[3], float n2[3], float result[3] );
void calcAverageNormal4( float n1[3], float n2[3], float n3[3], float n4[3],
                         float result[3] );
void normaliseVector( float normalvector[3] );
float displace(float rand_effect_size);

/*******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/
Landscape* Landscape_new( int grid_width,
                          float min_height, float max_height,
                          float south_bound, float west_bound,
                          float world_width, float world_depth )
{
    Landscape* landscape = (Landscape*)malloc( sizeof(Landscape) );
    
    landscape->gridWidth = grid_width;
    landscape->pointMap = *PointMap_new(grid_width);
    landscape->colorMap = *ColorMap_new(grid_width);
    landscape->normalMap = *NormalMap_new(grid_width);
    
    /* Calculate world dimensions */
    landscape->worldWidth = world_width;
    landscape->worldDepth = world_depth;
    
    landscape->southBound = south_bound;
    landscape->westBound = west_bound;
    landscape->eastBound = west_bound + world_width;
    landscape->northBound = south_bound + world_depth;
    
    landscape->minHeight = min_height;
    landscape->maxHeight = max_height;
    
    // Fencepost problem
    landscape->gridDivisionWidth = world_width / (float)(grid_width - 1);
    landscape->gridDivisionDepth = world_depth / (float)(grid_width - 1);
    
    return landscape;
}

void Landscape_delete( Landscape* landscape )
{
    if( landscape == NULL )
        return;
    
    if( landscape->colorMap != NULL )
        // TODO: ColorMap_delete( landscape->colorMap );
    if( landscape->pointMap != NULL )
        // TODO: PointMap_delete( landscape->pointMap );
    if( landscape->normalMap != NULL )
        // TODO: NormalMap_delete( landscape->normalMap );
    
    free( landscape );
}

void Landscape_generate( Landscape* landscape )
{
    int fsize, size;
    float range,rand_effect_size,cornerA,cornerB,cornerC,cornerD,midheight;
   
    range = landscape->maxHeight - landscape->minHeight;
    fsize = landscape->gridWidth - 1;
    // The current full size of the fractal iteration
    size = fsize/2;                        
    // The new size of the fractal iteration
    rand_effect_size = range;
    
    cornerA = bounded_random(landscape->maxHeight,landscape->minHeight);
    cornerB = bounded_random(landscape->maxHeight,landscape->minHeight);
    cornerC = bounded_random(landscape->maxHeight,landscape->minHeight);
    cornerD = bounded_random(landscape->maxHeight,landscape->minHeight);

    midheight = ((cornerA+cornerB+cornerC+cornerD)/4.0f)
                + (displace(rand_effect_size));

    // Assigns the corners from bottom left as A anticlockwise to form square
    // ABCD

    set_height( landscape, 0, 0, cornerA );
    set_height( landscape, fsize, 0, cornerB );
    set_height( landscape, fsize, fsize, cornerC );
    set_height( landscape, 0, fsize, cornerD );
   
    if (size >= MINGRIDSIZE)
    {  
        // Mid-point calculations
        set_height( landscape, size, size,
                    midheight );
    
        set_height( landscape, size, 0,
                    (cornerA + cornerB + midheight) / 3.0f
                        + (displace(rand_effect_size)) );
        
        set_height( landscape, fsize, size,
                    (cornerB + cornerC + midheight) / 3.0f
                        + (displace(rand_effect_size)) );
        
        set_height( landscape, size, fsize,
                    (cornerD + cornerC + midheight)/3
                        + (displace(rand_effect_size)) );
        set_height( landscape, 0, size,
                    (cornerA + cornerD + midheight)/3 
                        + (displace(rand_effect_size)) );
       
        rand_effect_size *= ROUGHNESS;
    
        fractalIteration(0,0,       size,   rand_effect_size, landscape); 
        // -x, -y   quad
        fractalIteration(size,0,    size,   rand_effect_size, landscape); 
        // x, -y    quad
        fractalIteration(0,size,    size,   rand_effect_size, landscape);
        // -x, y    quad
        fractalIteration(size,size, size,   rand_effect_size, landscape); 
        // x, y     quad
    }

    return;
}

Point* Landscape_getPoint( Landscape* landscape, int row, int column )
{
    if( row < 0 || row >= landscape->gridWidth ||
        column < 0 || column >= landscape->gridWidth )
    {
        return NULL;
    }
    
    return &(landscape->pointMap[row][column]);
}

int Landscape_getRow( Landscape* landscape, float X){
    return (X - landscape->westBound) / (float)landscape->gridDivisionWidth;
}

int Landscape_getColumn( Landscape* landscape, float Z){
    return (Z - landscape->southBound) / (float)landscape->gridDivisionWidth;
}

float Landscape_getHeight( Landscape* landscape, float X, float Z )
{
    int row, col;
        
    // row should be the Row not the column, but it wasn't working right
    // useing the column is a hacky fix, 
    // it works provided that landscape->westBound == landscape->southBound
    row = Landscape_getColumn(landscape, X);
    col = Landscape_getColumn(landscape, Z);
        
    return landscape->pointMap[row][col][1];
}

/*******************************************************************************
 * INTERNAL FUNCTIONS
 ******************************************************************************/
/**
 * Sets the a point in the pointmap.
 */
void set_point( Landscape* landscape, int row, int column,
                float X, float Y, float Z )
{
    // Set the point
    landscape->pointMap[row][column][0] = X;
    landscape->pointMap[row][column][1] = Y;
    landscape->pointMap[row][column][2] = Z;
    
    // Set the colour
    set_color( landscape, row, column, Y );
}

/**
 * Sets the a colour in the colourmap, given its height.
 */
void set_color( Landscape* landscape, int x, int z,
                float height)
{
    float R, G, B;
    float brightness = 1.0f;
    
    float a = 3.5f,
          b = 0.4f,
          c = 0.14f;
    
    height -= landscape->minHeight;
    height /= landscape->maxHeight * 1.25;
    
    brightness = 1.0;
    
    G = brightness * (a * pow( (height - b), 2 ) + 0.25);
    R = brightness * (a * pow( (height - b), 2 ) + c);
    B = (height > b ? brightness : (brightness - 1 / (height + 0.8))) * a * pow( (height - b), 2 );
    
    landscape->colorMap[x][z][0] = R;
    landscape->colorMap[x][z][1] = G;
    landscape->colorMap[x][z][2] = B;
}

/**
 * Sets a normal in the normalmap.
 */
void set_normal( Landscape* landscape, int row, int column,
                 float X, float Y, float Z )
{
    landscape->normalMap[row][column][0] = X;
    landscape->normalMap[row][column][1] = Y;
    landscape->normalMap[row][column][2] = Z;
}

// Produces a random value between +ve and -ve of input
float displace(float rand_effect_size)
{
    return bounded_random(rand_effect_size,-rand_effect_size);
}

void fractalIteration(int Ax, int Ay, int fsize,float rand_effect_size, Landscape* landscape)
{
    int size = fsize/2;
    float cornerA =
        ( *Landscape_getPoint(landscape, Ax, Ay) )[1];
    float cornerB = 
        ( *Landscape_getPoint(landscape, Ax + fsize, Ay) )[1];
    float cornerC =
        ( *Landscape_getPoint(landscape, Ax + fsize, Ay + fsize) )[1];
    float cornerD =
        ( *Landscape_getPoint(landscape, Ax, Ay+fsize) )[1];
    
    float midheight = ((cornerA+cornerB+cornerC+cornerD)/4.0f);// + (displace(rand_effect_size)); // Mid-point calculations - Diamond Step
    
    // Set the height of the centre point
    set_height( landscape, Ax + size, Ay + size,
                midheight );

    if( landscape->pointMap[Ax+size][Ay][1] == 0 )
    {
        set_height( landscape, Ax + size, Ay,
                   (cornerA+cornerB+midheight)/3 + (displace(rand_effect_size)) );
    }
    if( landscape->pointMap[Ax+fsize][Ay+size][1] == 0 )
    {
        set_height( landscape, Ax+fsize, Ay+size,
                   (cornerB+cornerC+midheight)/3 + (displace(rand_effect_size)) );
    }
    if( landscape->pointMap[Ax+size][Ay+fsize][1] == 0 )
    {
        set_height( landscape, Ax+size,Ay+fsize,
                   (cornerC+cornerD+midheight)/3 + (displace(rand_effect_size)) );
    }
    if( landscape->pointMap[Ax][Ay+size][1] == 0 )
    {
        set_height( landscape, Ax, Ay+size,
                   (cornerA+cornerD+midheight)/3 + (displace(rand_effect_size)) );
    }

    if (size >= MINGRIDSIZE )
    {
        rand_effect_size *= ROUGHNESS;
        fractalIteration(Ax,Ay,             size,   rand_effect_size, landscape);  // -x, -y    quad
        fractalIteration(Ax+size,Ay,        size,   rand_effect_size, landscape);  // x, -y quad
        fractalIteration(Ax,Ay+size,        size,   rand_effect_size, landscape);  // -x, y quad
        fractalIteration(Ax+size,Ay+size,   size,   rand_effect_size, landscape);  // x, y      quad
    }
    else
    {
        /* Calculate normals */
        // Always calculate the top-left point's normal
        calculate_vertex_normal( landscape, Ax, Ay );
        
        // If we're on the right, calculate the top right point's normal
        if( Ax + fsize == landscape->gridWidth - 1 )
        {
            calculate_vertex_normal( landscape, Ax + 1, Ay );
        }
        
        // If we're on the bottom, calculate the bottom left point's normal
        if( Ay + fsize == landscape->gridWidth - 1 )
        {
            calculate_vertex_normal( landscape, Ax, Ay + 1 );
        }
        
        // If we're on the bottom-right, calculate all points
        if( Ax + fsize == landscape->gridWidth - 1 && Ay + fsize == landscape->gridWidth - 1 )
        {
            calculate_vertex_normal( landscape, Ax + 1, Ay + 1 );
        }
    }
}

void calculate_vertex_normal( Landscape* landscape, int row, int column )
{
    Point newNormal, n1, n2, n3, n4;
    
    Point *position,
          *left, *right, *top, *bottom;
    
    position = Landscape_getPoint( landscape, row, column );
    left = Landscape_getPoint( landscape, row, column - 1 );
    right = Landscape_getPoint( landscape, row, column + 1 );
    top = Landscape_getPoint( landscape, row + 1, column );
    bottom = Landscape_getPoint( landscape, row - 1, column );
    
    if (left == NULL)
    {
        if (top == NULL)
        {
            // handle the top-left corner normal case - DONE
            calculate_normal( *bottom, *right, *position, &newNormal);
        }
        else if (bottom == NULL)
        {
            // handle the bottom-left corner normal case
            calculate_normal( *right, *top, *position, &newNormal );
        }
        else
        {
            // handle the left edge case
            calculate_normal(*bottom,*right,*position,&n1);
            calculate_normal(*right,*top,*position,&n2);
            normaliseVector(n1);
            normaliseVector(n2);
            calcAverageNormal2(n1,n2,newNormal);
        }
    }
    else if (right == NULL)
    {
        if (top == NULL)
        {
            // handle the top-right corner normal case
            calculate_normal( *left, *bottom, *position, &newNormal );
        }
        else if (bottom == NULL)
        {
            // handle the bottom-right corner normal case
            calculate_normal(*top,*left,*position,&newNormal);
        }
        else
        {
            // handle the right edge case
            calculate_normal(*left,*bottom,*position,&n1);
            calculate_normal(*top,*left,*position,&n2);
            normaliseVector(n1);
            normaliseVector(n2);
            calcAverageNormal2(n1,n2,newNormal);
        }
    }
    else if (top == NULL)
    {
        // handle the top edge case
        calculate_normal(*bottom,*right,*position,&n1);
        calculate_normal(*left,*bottom,*position,&n2);
        normaliseVector(n1);
        normaliseVector(n2);
        calcAverageNormal2(n1,n2,newNormal);
    }
    else if (bottom == NULL)
    {
        // handle the bottom edge case
        calculate_normal(*right,*top,*position,&n1);
        calculate_normal(*top,*left,*position,&n2);
        normaliseVector(n1);
        normaliseVector(n2);
        calcAverageNormal2(n1,n2,newNormal);
    }
    else
    {
        // handle the normal case
        calculate_normal(*right,*top,*position,&n1);
        calculate_normal(*bottom,*right,*position,&n2);
        calculate_normal(*left,*bottom,*position,&n3);
        calculate_normal(*top,*left,*position,&n4);
        normaliseVector(n1);
        normaliseVector(n2);
        normaliseVector(n3);
        normaliseVector(n4);
        calcAverageNormal4(n1,n2,n3,n4,newNormal);
    }
    
    set_normal( landscape, row, column,
                (*position)[0] + newNormal[0],
                (*position)[1] + newNormal[1],
                (*position)[2] + newNormal[2] );
}

// Produce the normal for 3 given points
void calculate_normal( Point P1, Point P2, Point Centre, Point* normalvector )
{
    float v1[3] = { P1[0]-Centre[0],P1[1]-Centre[1],P1[2]-Centre[2] };
    float v2[3] = { P2[0]-Centre[0],P2[1]-Centre[1],P2[2]-Centre[2] };

    cross_product(v1, v2, normalvector);
}

void calcAverageNormal2(float n1[3],float n2[3],float result[3])
{
    result[0] = (n1[0]+n2[0])/2;
    result[1] = (n1[1]+n2[1])/2;
    result[2] = (n1[2]+n2[2])/2;
}
void calcAverageNormal4(float n1[3],float n2[3],float n3[3],float n4[3], float result[3])
{
    result[0] = (n1[0]+n2[0]+n3[0]+n4[0])/4;
    result[1] = (n1[1]+n2[1]+n3[1]+n4[1])/4;
    result[2] = (n1[2]+n2[2]+n3[2]+n4[2])/4;
}



Color* Color_new( float R, float G, float B, float A )
{
    Color* color = (Color*)malloc( sizeof(Color) );
    
    (*color)[0] = R;
    (*color)[1] = G;
    (*color)[2] = B;
    (*color)[3] = A;
    
    return color;
}

Point* Point_new( float X, float Y, float Z )
{
    Point* point = (Point*)malloc( sizeof(Point) );
    
    (*point)[0] = X;
    (*point)[1] = Y;
    (*point)[2] = Z;
    
    return point;
}

Normal* Normal_new( float X, float Y, float Z )
{
    Normal* normal = (Normal*)malloc( sizeof(Normal) );
    
    (*normal)[0] = X;
    (*normal)[1] = Y;
    (*normal)[2] = Z;
    
    return normal;
}

ColorMap* ColorMap_new( int width )
{
    int i;
    // Create the map
    ColorMap* colormap = (ColorMap*)malloc( sizeof(ColorMap) );
    
    // Create the rows
    *colormap = (Color**)malloc( sizeof(Color*) * width );
    for( i = 0; i < width; i++ )
    {
        // Create the columns
        (*colormap)[i] = (Color*)malloc( sizeof(Color) * width );
    }
    
    return colormap;
}

PointMap* PointMap_new( int width )
{
    int i;
    // Create the map
    PointMap* pointmap = (PointMap*)calloc( sizeof(PointMap), 1 );
    
    // Create the rows
    *pointmap = (Point**)calloc( sizeof(Point*), width );
    for( i = 0; i < width; i++ )
    {
        // Create the columns
        (*pointmap)[i] = (Point*)calloc( sizeof(Point), width );
    }
    
    return pointmap;
}

NormalMap* NormalMap_new( int width )
{
    int i;
    // Create the map
    NormalMap* normalmap = (NormalMap*)malloc( sizeof(NormalMap) );
    
    // Create the rows
    *normalmap = (Normal**)malloc( sizeof(Normal*) * width );
    for( i = 0; i < width; i++ )
    {
        // Create the columns
        (*normalmap)[i] = (Normal*)malloc( sizeof(Normal) * width );
    }
    
    return normalmap;
}

void set_height( Landscape* landscape, int row, int column,
                 float height )
{
    float X, Z;
    // Calculate the X and Z values
    X = landscape->westBound + row * landscape->gridDivisionWidth;
    Z = landscape->southBound + column * landscape->gridDivisionDepth;
    
    set_point( landscape, row, column, X, height, Z );
}
