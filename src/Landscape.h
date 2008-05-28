#ifndef LANDSCAPE_H_
#define LANDSCAPE_H_

/*******************************************************************************
 * TYPE DEFINITIONS
 ******************************************************************************/
// Definition of a `Height`
typedef float Point[3];

// Definition of a `Color` (used gay American spelling for consistency)
typedef float Color[4];

// Definition of a `Normal`, an array of floats for X, Y and Z
typedef float Normal[3];

/* Definition of colour, height and normal maps, which are just two
 * dimensional arrays of their respective types. */
typedef Color** ColorMap;
typedef Point** PointMap;
typedef Normal** NormalMap;

// The Landscape structure
typedef struct {
    PointMap pointMap; // Positions at each point
    ColorMap colorMap; // Colour values of each point
    NormalMap normalMap; // Normals at each point
    
    int gridWidth; // The width of the landscape grid, in points
    
    // The boundaries of the landscape
    float northBound, // The boundary in the positive Z direction
          eastBound, // The boundary in the positive X direction
          southBound, // The boundary in the negative Z direction
          westBound; // The boundary in the negative X direction
    
    float minHeight, // The minimum height of any point in the landscape
          maxHeight;// The maximum height of any point in the landscape
    
    // The dimensions of the game world
    float worldWidth, // The east-west distance across the world
          worldDepth, // The north-south distance across the world
          gridDivisionWidth, // The north-south distance between grid points
          gridDivisionDepth; // The east-west distance between grid points
} Landscape;


/*******************************************************************************
 * CONSTRUCTORS/DESTRUCTORS
 ******************************************************************************/
/**
 * Creates and initializes a Landscape structure.
 */
Landscape* Landscape_new( int grid_size,
                          float min_height, float max_height,
                          float south_bound, float west_bound,
                          float world_width, float world_depth );
/**
 * Frees a landscape structure.
 */
void Landscape_delete( Landscape* landscape );

/*******************************************************************************
 * LANDSCAPE FUNCTIONS
 ******************************************************************************/
/**
 * Given a landscape structure, generates the landscape.
 */
void Landscape_generate( Landscape* landscape );
/**
 * Retrieves the point at grid coordinates (row, column).
 */
Point* Landscape_getPoint( Landscape* landscape, int row, int column );
/**
 * Sets the point at grid coordinates (row, column).
 */
void Landscape_setPoint( Landscape* landscape, int row, int column,
                         float X, float Y, float Z );
/**
 * Gets the height at a position, in real-space, X/Z coordinates.
 */
float Landscape_getHeight( Landscape* landscape, float X, float Z );

int Landscape_getColumn( Landscape* landscape, float Z);

float Landscape_getHeight( Landscape* landscape, float X, float Z );

#endif /*LANDSCAPE_H_*/
