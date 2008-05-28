#include "Viewport.h"
#include <stdlib.h>
#include <GL/glut.h>

/*******************************************************************************
 * GLOBALS AND CONSTANTS
 ******************************************************************************/
#define NEAR_Z 0.01f
#define FAR_Z 100.0f
#define FOVY 130.0f

/*******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/
Viewport* Viewport_new( int x, int y, int width, int height )
{
    // Use malloc instead of calloc, since we'll initialize all values anyway
    Viewport* viewport = (Viewport*)malloc( sizeof(Viewport) );
    
    // Initialize the viewport
    viewport->camera = Camera_new();
    viewport->x = x;
    viewport->y = y;
    viewport->width = width;
    viewport->height = height;
    viewport->ortho = 0;
    
    return viewport;
}

void Viewport_delete( Viewport* viewport )
{
    Camera_delete( viewport->camera );
    free(viewport);
}

void Viewport_apply( Viewport* viewport )
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport( viewport->x, viewport->y, viewport->width, viewport->height );

    if( viewport->ortho )
    {
        glOrtho( viewport->left, viewport->right,
                 viewport->bottom, viewport->top, NEAR_Z, FAR_Z );
    }
    else
    {
        gluPerspective( FOVY,
                        viewport->width / ((float) viewport->height),
                        // Force float division
                        NEAR_Z,
                        FAR_Z );
    }
    
    gluLookAt( viewport->camera->position[0],
               viewport->camera->position[1],
               viewport->camera->position[2],
               viewport->camera->forward[0],
               viewport->camera->forward[1],
               viewport->camera->forward[2],
               viewport->camera->up[0],
               viewport->camera->up[1],
               viewport->camera->up[2] );
               
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


