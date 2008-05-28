#include "maths.h"
#include <stdlib.h>
#include <math.h>

#define SMALL_NUM 0.000001f

void multiplyScalar(float scalar,float* vector, float* result);

float distance_between_points( Point point1, Point point2 )
{
    float x, y, z;
    
    x = point1[0] - point2[0];
    y = point1[1] - point2[1];
    z = point1[2] - point2[2];
    
    x = pow(x, 2);
    y = pow(y, 2);
    z = pow(z, 2);
    
    return sqrt( x + y + z );
}

float bounded_random( float min, float max )
{
//    return 0;// TESTING
    return min + (max - min) * ((float)rand() / (float)RAND_MAX);
}

void cross_product( Point v1, Point v2, Point* product )
{
    (*product)[0] = v1[1]*v2[2] - v1[2]*v2[1];
    (*product)[1] = v1[2]*v2[0] - v1[0]*v2[2];
    (*product)[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

void normaliseVector(float normalvector[3])
{
    float norm = sqrt(  pow(normalvector[0],2) 
                        + pow(normalvector[1],2) 
                        + pow(normalvector[2],2)
                        );
    normalvector[0] = normalvector[0] / norm;
    normalvector[1] = normalvector[1] / norm;
    normalvector[2] = normalvector[2] / norm;
}

/* This function code borrowed from:
http://geometryalgorithms.com/Archive/algorithm_0106/algorithm_0106.htm#dist3D_Line_to_Line()
*/
float dist_line_to_line( float* L1P0,float* L1P1,float* L2P0, float* L2P1)
{
	float    u[3] = {L1P1[0] - L1P0[0],
					L1P1[1] - L1P0[1],
					L1P1[2] - L1P0[2]	}; // Vector u
    float	 v[3] = {L2P1[0] - L2P0[0],
					L2P1[1] - L2P0[1],
					L2P1[2] - L2P0[2]	}; // Vector v
    float    w[3] = {L1P0[0] - L2P0[0],
					L1P0[1] - L2P0[1],
					L1P0[2] - L2P0[2]	}; // Vector w
	float dP[3],
		temp1[3],
		temp2[3],
		temp3[3];

    float    a = dot(u,u);        // always >= 0
    float    b = dot(u,v);
    float    c = dot(v,v);        // always >= 0
    float    d = dot(u,w);
    float    e = dot(v,w);
    float    D = a*c - b*b;       // always >= 0
    float    sc, tc;


    // compute the line parameters of the two closest points
    if (D < SMALL_NUM) {         // the lines are almost parallel
        sc = 0.0;
        tc = (b>c ? d/b : e/c);   // use the largest denominator
    }
    else {
        sc = (b*e - c*d) / D;
        tc = (a*e - b*d) / D;
    }

    // get the difference of the two closest points
    	// implements  w + (sc*u) - (tc*v)

	multiplyScalar(sc,u,temp1);
	multiplyScalar(-tc,v,temp2);
	addVector(temp1,temp2,temp3);
	addVector(temp3,w,dP);
	
    return sqrt(  pow(dP[0],2) 
                        + pow(dP[1],2) 
                        + pow(dP[2],2)
                        );   // return the closest distance
}

float dot(float* v1, float* v2) 
{
	int i;
    float dp = 0.0f;
    for (i=0;i<3;i++)
        dp += v1[i] * v2[i];
    return dp;
}

void addVector(float* v1,float* v2,float* result)
{
	result[0] = v1[0]+v2[0];
	result[0] = v1[1]+v2[1];
	result[0] =	v1[2]+v2[2];
}

void multiplyScalar(float scalar,float* vector, float* result)
{
	result[0] = vector[0]*scalar;
	result[1] =	vector[1]*scalar;
	result[2] =	vector[2]*scalar;
}

float dist_point_to_line( float* point, float* line1, float* line2 )
{
    float s, a, b, c, result;
    
    a = abs( distance_between_points( point,  line1) );
    b = abs( distance_between_points( line1,  line2) );
    c = abs( distance_between_points( point,  line2) );
    
    s = (a + b + c) / 2.0;
    
    result = (2 * sqrt(s * (s-a) * (s-b) * (s-c))) / c;
    
    return result;
}
