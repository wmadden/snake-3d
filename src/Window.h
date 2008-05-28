#ifndef WINDOW_H_
#define WINDOW_H_
/**
 * Window.h
 * This module is responsible for handling windows.
 * 
 * It serves as a wrapper for GLUT windowing functions, making the use of GLUT
 * windows object oriented.
 */

/**
 * Creates a new window and returns its ID.
 * 
 * @return The ID number of the new window.
 */
int Window_new( char* title );

/**
 * Deletes a window.
 */
void Window_delete( int window_id );

/**
 * Set the window title.
 */
void Window_setTitle( int window_id, char* title );
/**
 * Returns the window title.
 */
char* Window_getTitle( int window_id );
/**
 * Sets the icon title.
 */
void Window_setIconTitle( int window_id, char* title );
/**
 * Gets the icon title.
 */
void Window_getIconTitle( int window_id );
/**
 * Returns the window's width.
 */
int Window_getWidth( int window_id );
/**
 * Returns the window's height.
 */
int Window_getHeight( int window_id );
/**
 * Sets the window's width.
 */
void Window_setWidth( int window_id, int width );
/**
 * Sets the window's height.
 */
void Window_setHeight( int window_id, int height );
/**
 * Makes the window fullscreen.
 */
void Window_makeFullScreen( int window_id );
/**
 * Resizes the window.
 */
void Window_resize( int window_id, int width, int height );
/**
 * Moves the window.
 */
void Window_move( int window_id, int x, int y );
/**
 * Makes the window the current window.
 */
void Window_makeCurrent( int window_id );
/**
 * Flags the window as dirty, requiring a redraw.
 */
void Window_makeDirty( int window_id );
/**
 * Swaps the window's buffers.
 */
void Window_swapBuffers( int window_id );
/**
 * Shows the window.
 */
void Window_show( int window_id );
/**
 * Hides the window.
 */
void Window_hide( int window_id );
/**
 * Iconifies the window.
 */
void Window_iconify( int window_id );
/**
 * Sets the window cursor.
 */
void Window_setCursor( int window_id, int cursor );

/* Functions for registering event handlers */
void Window_registerDisplayEventHandler( int window_id, void (*displayHandler)(void) );
void Window_registerReshapeEventHandler( int window_id, void (*reshapeHandler)(int width, int height) );
void Window_registerVisibilityChangeEventHandler( int window_id, void (*visibilityChangeHandler)(int state) );
void Window_registerKeyEventHandler( int window_id, void (*keyHandler)(unsigned char key, int x, int y) );
void Window_registerSpecialKeyEventHandler( int window_id, void (*specialKeyHandler)(int key, int x, int y) );
void Window_registerMouseButtonEventHandler( int window_id, void (*mouseButtonHandler)(int button, int state, int x, int y) );
void Window_registerMouseActiveMotionEventHandler( int window_id, void (*mouseActiveMotionHandler)(int x, int y) );
void Window_registerMousePassiveMotionEventHandler( int window_id, void (*mousePassiveMotionHandler)(int x, int y) );
void Window_registerMouseEntryEventHandler( int window_id, void (*mouseEntryHandler)(int state) );

#endif /*WINDOW_H_*/
