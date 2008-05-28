#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "Camera.h"

/*******************************************************************************
 * TYPE DEFINITIONS
 ******************************************************************************/
typedef struct {
    Camera* camera;
    int width, height; // In pixels
    int x, y; // Position, in pixels from the bottom left of the window
    int ortho;
    float left, right, top, bottom;
} Viewport;

/**
 * Creates a new viewport structure, and initializes it to the given values.
 */
Viewport* Viewport_new( int x, int y, int width, int height );
/**
 * Deletes a viewport structure.
 */
void Viewport_delete( Viewport* viewport );

/**
 * Applies a viewport's settings to the current OpenGL context.
 * 
 * This includes modifying the current model/view matrix to match that of the
 * viewport's camera. 
 */
void Viewport_apply( Viewport* viewport );

#endif /*VIEWPORT_H_*/
