#include "text.h"
#include <stdlib.h>
#include <GL/glut.h>

#define FONT_MAX_HEIGHT 152.38

void draw_2D_text( char* text, int x, int y, int height, Viewport* vp )
{
    float vscale,
          hscale,
          vppu,
          hppu;
    
    if( text == NULL )
        return;
    
    vppu = vp->height / (vp->top - vp->bottom);
    hppu = vp->width / (vp->right - vp->left);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    vscale = height / (vppu * (float)FONT_MAX_HEIGHT);
    glTranslatef( vp->left + x / hppu, vp->bottom + y / vppu, 0.0f );
    glScalef( vscale, vscale, 1.0f );
    
    draw_text(text);
    
    glPopMatrix();
}

void draw_text( char* text )
{
    
    while( *text != '\0' )
    {
        glutStrokeCharacter( GLUT_STROKE_ROMAN, *text );
        text++;
    }
}

void draw_centered_text( char* text, int x, int y, int height, Viewport* vp )
{
    char* c = text;
    float width = 0.0f;
    
    while( *c != '\0' )
    {
        width += glutStrokeWidth( GLUT_STROKE_ROMAN, *c );
    }
    
    draw_2D_text( text, x - width / 2, y, height, vp );
}
