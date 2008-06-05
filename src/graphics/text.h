#ifndef TEXT_H_
#define TEXT_H_

#include "Viewport.h"

/**
 * Draws `text` at pixel coordinates (x,y), with a height of `height`, on
 * viewport `vp`.
 */
void draw_2D_text( char* text, int x, int y, int height, Viewport* vp );

/**
 * Draws `text`.
 */
void draw_text( char* text );

void draw_centered_text( char* text, int x, int y, int height, Viewport* vp );

#endif /*TEXT_H_*/
