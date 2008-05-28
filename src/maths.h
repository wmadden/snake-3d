#ifndef MATHS_H_
#define MATHS_H_
/**
 * maths.h
 * 
 * This module defines useful maths functions.
 */

#include "Landscape.h"

/**
 * Calculates the distance between two points.
 */
float distance_between_points( Point point1, Point point2 );
/**
 * Generates a random number between min and max.
 */
float bounded_random( float min, float max );
/**
 * Calculates the cross product of two vectors.
 */
void cross_product( Point v1, Point v2, Point* product );
/**
 * Normalises a vector.
 */
void normaliseVector(float normalvector[3]);
/**
 * Gives the distance between two lines.
 */
float dist_line_to_line( float* L1P0,float* L1P1,float* L2P0, float* L2P1);
float dist_point_to_line( float* point, float* line1, float* line2 );
/**
 * Adds two vectors.
 */
void addVector(float* v1,float* v2,float* result);
/**
 * Dot product.
 */
float dot(float* v1, float* v2);

#endif /*MATHS_H_*/
