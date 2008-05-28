#ifndef RENDER_H_
#define RENDER_H_

/**
 * Initializes the module.
 */
void render_init();

/**
 * Renders the game.
 * 
 * Note: assumes the OpenGL context is already set (which GLUT should take care
 * of).
 */
void render();

void render_update( int delta );

/**
 * The notification function for when the window is resized.
 */
void window_resized( int width, int height );

#endif /*RENDER_H_*/
