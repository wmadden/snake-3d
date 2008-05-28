#include "Window.h"
#include <stdlib.h>
#include <GL/glut.h>

/******************************************************************************
 * TYPE DEFINITIONS
 *****************************************************************************/
typedef struct {
    int glutID;
    char* title; 
    void (*displayHandler)(void);
    void (*reshapeHandler)(int width, int height);
    void (*visibilityChangeHandler)(int state);
    void (*keyHandler)(unsigned char key, int x, int y);
    void (*specialKeyHandler)(int key, int x, int y);
    void (*mouseButtonHandler)(int button, int state, int x, int y);
    void (*mouseActiveMotionHandler)(int x, int y);
    void (*mousePassiveMotionHandler)(int x, int y);
    void (*mouseEntryHandler)(int state);
} Window;

/******************************************************************************
 * INTERNAL VARIABLES AND CONSTANTS
 *****************************************************************************/
#define INITIAL_WINDOW_CAPACITY 1
Window** windows = NULL;
int window_count = 0;

/******************************************************************************
 * INTERNAL FUNCTION PROTOTYPES
 *****************************************************************************/
void Window_init();
void init_registry();
int register_window( Window* );
void deregister_window( int window_id );
Window* get_window( int window_id );
int get_window_glutID( int window_id );
int get_state_variable( int window_id, GLenum en );
Window* find_window( int glut_id );

// GLUT event handlers
void display_event(void);
void reshape_event(int width, int height);
void visibility_change_event(int state);
void key_event(unsigned char key, int x, int y);
void special_key_event(int key, int x, int y);
void mouse_button_event(int button, int state, int x, int y);
void mouse_active_motion_event(int x, int y);
void mouse_passive_motion_event(int x, int y);
void mouse_entry_event(int state);
void register_callbacks();

/******************************************************************************
 * PUBLIC FUNCTIONS
 *****************************************************************************/
int Window_new( char* title )
{
	// Create the new window structure
    Window *window = (Window*)calloc( 1, sizeof(Window) );

	// Get GLUT to make a new window
    window->glutID = glutCreateWindow(title);
    
	// Initialize the module
	Window_init();

    // Set the title
    window->title = title;
    
    register_callbacks();
    
    // Register the window
    return register_window(window);
}

/* Note: in order to maintain a simple implementation of this module, deleting
 * windows is fairly inefficient. However, since we won't have more than one
 * window, this function is completely unnecessary, and only provided for
 * completeness. */
void Window_delete( int window_id )
{
    // Get the window structure so we can free it
    Window* window = get_window(window_id);
    
    // Destroy the GLUT window
    glutDestroyWindow(window->glutID);
    
    // Free the window structure
    free(window);
    // Delete the entry from the registry
    deregister_window(window_id);
}

void Window_setTitle( int window_id, char* title )
{
    Window* window = get_window(window_id);
    int cur = glutGetWindow();
    
    glutSetWindow( window->glutID );
    glutSetWindowTitle( title );
    glutSetWindow(cur);
    
    window->title = title;
}

int Window_getWidth( int window_id )
{
    return get_state_variable( window_id,  GLUT_WINDOW_WIDTH );
}

int Window_getHeight( int window_id )
{
    return get_state_variable( window_id, GLUT_WINDOW_HEIGHT );
}

char* Window_getTitle( int window_id )
{
    Window* window = get_window(window_id);
    return window->title;
}

void Window_setIconTitle( int window_id, char* title )
{
    int cur = glutGetWindow();
    
    glutSetWindow( get_window_glutID(window_id) );
    glutSetIconTitle(title);
    glutSetWindow(cur);
}

/**
 * Makes the window fullscreen.
 */
void Window_makeFullScreen( int window_id )
{
    int cur = glutGetWindow();
    
    glutSetWindow( get_window_glutID(window_id) );
    glutFullScreen();
    glutSetWindow(cur);
}

/**
 * Resizes the window.
 */
void Window_resize( int window_id, int width, int height )
{
    int cur = glutGetWindow();
    
    glutSetWindow( get_window_glutID(window_id) );
    glutReshapeWindow( width, height );
    glutSetWindow(cur);
}

/**
 * Moves the window.
 */
void Window_move( int window_id, int x, int y )
{
    int cur = glutGetWindow();
    
    glutSetWindow( get_window_glutID(window_id) );
    glutPositionWindow( x, y );
    glutSetWindow(cur);
}

void Window_makeCurrent( int window_id )
{
    glutSetWindow( get_window_glutID(window_id) );
}

void Window_makeDirty( int window_id )
{
    int cur = glutGetWindow();
    
    glutSetWindow( get_window_glutID(window_id) );
    glutPostRedisplay();
    glutSetWindow(cur);
}

void Window_swapBuffers( int window_id )
{
    int cur = glutGetWindow();
    
    glutSetWindow( get_window_glutID(window_id) );
    glutSwapBuffers();
    glutSetWindow(cur);
}

void Window_show( int window_id )
{
    int cur = glutGetWindow();
    
    glutSetWindow( get_window_glutID(window_id) );
    glutShowWindow();
    glutSetWindow(cur);
}

void Window_hide( int window_id )
{
    int cur = glutGetWindow();
    
    glutSetWindow( get_window_glutID(window_id) );
    glutHideWindow();
    glutSetWindow(cur);
}

void Window_iconify( int window_id )
{
    int cur = glutGetWindow();
    
    glutSetWindow( get_window_glutID(window_id) );
    glutIconifyWindow();
    glutSetWindow(cur);
}

void Window_setCursor( int window_id, int cursor )
{
    int cur = glutGetWindow();
    
    glutSetWindow( get_window_glutID(window_id) );
    glutSetCursor( cursor );
    glutSetWindow(cur);
}

void Window_registerDisplayEventHandler( int window_id, void (*displayHandler)(void) )
{
    // Find the current window object
    Window *window = get_window(window_id);
    // Register the handler
    window->displayHandler = displayHandler;
}

void Window_registerReshapeEventHandler( int window_id, void (*reshapeHandler)(int width, int height) )
{
    // Find the current window object
    Window *window = get_window(window_id);
    // Register the handler
    window->reshapeHandler = reshapeHandler;
}

void Window_registerVisibilityChangeEventHandler( int window_id, void (*visibilityChangeHandler)(int state) )
{
    // Find the current window object
    Window *window = get_window(window_id);
    // Register the handler
    window->visibilityChangeHandler = visibilityChangeHandler;
}

void Window_registerKeyEventHandler( int window_id, void (*keyHandler)(unsigned char key, int x, int y) )
{
    // Find the current window object
    Window *window = get_window(window_id);
    // Register the handler
    window->keyHandler = keyHandler;
}

void Window_registerSpecialKeyEventHandler( int window_id, void (*specialKeyHandler)(int key, int x, int y) )
{
    // Find the current window object
    Window *window = get_window(window_id);
    // Register the handler
    window->specialKeyHandler = specialKeyHandler;
}

void Window_registerMouseButtonEventHandler( int window_id, void (*mouseButtonHandler)(int button, int state, int x, int y) )
{
    // Find the current window object
    Window *window = get_window(window_id);
    // Register the handler
    window->mouseButtonHandler = mouseButtonHandler;
}

void Window_registerMouseActiveMotionEventHandler( int window_id, void (*mouseActiveMotionHandler)(int x, int y) )
{
    // Find the current window object
    Window *window = get_window(window_id);
    // Register the handler
    window->mouseActiveMotionHandler = mouseActiveMotionHandler;
}

void Window_registerMousePassiveMotionEventHandler( int window_id, void (*mousePassiveMotionHandler)(int x, int y) )
{
    // Find the current window object
    Window *window = get_window(window_id);
    // Register the handler
    window->mousePassiveMotionHandler = mousePassiveMotionHandler;
}

void Window_registerMouseEntryEventHandler( int window_id, void (*mouseEntryHandler)(int state) )
{
    // Find the current window object
    Window *window = get_window(window_id);
    // Register the handler
    window->mouseEntryHandler = mouseEntryHandler;
}


/******************************************************************************
 * PRIVATE FUNCTIONS
 *****************************************************************************/
/**
 * Initializes the module.
 * 
 * NOTE: this function must be called before all other Window functions.
 */
void Window_init()
{
	static int initialized = 0;

	if( initialized != 0 )
	{
		initialized = 1;
		return;
	}

    // Initialize the window registry
    init_registry();
    
    // Register glut callbacks
    register_callbacks();
}

// TODO: check that window pointers are not NULL, when retrieving them from the
// registry
/**
 * Registers the event routing functions with GLUT.
 */
void register_callbacks()
{
    glutDisplayFunc( display_event );
    glutReshapeFunc( reshape_event );
    glutVisibilityFunc( visibility_change_event );
    glutKeyboardFunc( key_event );
    glutMouseFunc( mouse_button_event );
    glutMotionFunc( mouse_active_motion_event );
    glutPassiveMotionFunc( mouse_passive_motion_event );
    glutEntryFunc( mouse_entry_event );
    glutSpecialFunc( special_key_event );
}

/**
 * Initializes the window registry.
 *
 * Preconditions:
 * Postconditions:
 *     o `windows` points to an array of Window pointers, of length
 *       INITIAL_WINDOW_CAPACITY, all initialized to point to NULL.
 *     o `window_count` is zero.
 */
void init_registry()
{
    // Included unnecessarily in case we are _re_initializing
    if( windows != NULL )
    {
        free(windows);
        window_count = 0;
    }
    
    windows = (Window**)calloc( sizeof(Window*), INITIAL_WINDOW_CAPACITY );
    window_count = 0;
}
/**
 * Registers a window in the module, so that callback events are routed to the
 * right place.
 */
int register_window( Window* window )
{
    windows[window_count] = window;
    
    return window_count++;
}

/**
 * Takes the ID for a currently registered window and removes it from the
 * registry.
 */
void deregister_window( int window_id )
{
    windows[window_id] = NULL;
    // DO NOT DECREMENT window_count
}

/**
 * Returns a pointer to the Window identified by `window_id`.
 * 
 * If the window has been deregistered, NULL is returned.
 */
Window* get_window( int window_id )
{
    return windows[window_id];
}

/**
 * Returns the identifier used by GLUT to refer to the window.
 */
int get_window_glutID( int window_id )
{
    Window* window = get_window(window_id);
    return window->glutID;
}

/**
 * Wrapper to glutGet().
 */
int get_state_variable( int window_id, GLenum en )
{
    // Get the current window
    int cur = glutGetWindow(), result;
    // Set the current window to this window
    glutSetWindow( get_window_glutID(window_id) );
    // Get the state variable
    result = glutGet( en );
    // Set the current window back to what it was
    glutSetWindow(cur);
    
    return result;
}

/**
 * Finds and returns a pointer to the Window struct which corresponds to the
 * given GLUT window ID.
 * 
 * Returns NULL if no Window struct can be found which matches the given GLUT
 * ID.
 * 
 * @return A pointer to a Window, or NULL.
 */
Window* find_window( int glut_id )
{
    int i;
    for( i = 0; i < window_count; i++ )
    {
        if( windows[i] != NULL && windows[i]->glutID == glut_id )
        {
                return windows[i];
        }
    }
    
    return NULL;
}

/******************************************************************************
 * EVENT TRIGGERS
 *****************************************************************************/
void display_event()
{
    // Get the current glut window ID
    int glut_id = glutGetWindow();
    // Find the current window object
    Window *window = find_window(glut_id);
    // If set, call the event handler
    if( window->displayHandler != NULL )
        (*window->displayHandler)();
}

void reshape_event( int width, int height )
{
    // Get the current glut window ID
    int glut_id = glutGetWindow();
    // Find the current window object
    Window *window = find_window(glut_id);
    // If set, call the event handler
    if( window->reshapeHandler != NULL )
        (*window->reshapeHandler)( width, height );
}

void visibility_change_event( int state )
{
    // Get the current glut window ID
    int glut_id = glutGetWindow();
    // Find the current window object
    Window *window = find_window(glut_id);
    // If set, call the event handler
    if( window->visibilityChangeHandler != NULL )
        (*window->visibilityChangeHandler)( state );
}

void key_event( unsigned char key, int x, int y )
{
    // Get the current glut window ID
    int glut_id = glutGetWindow();
    // Find the current window object
    Window *window = find_window(glut_id);
    // If set, call the event handler
    if( window->keyHandler != NULL )
        (*window->keyHandler)( key, x, y );
}

void special_key_event( int key, int x, int y )
{
    // Get the current glut window ID
    int glut_id = glutGetWindow();
    // Find the current window object
    Window *window = find_window(glut_id);
    // If set, call the event handler
    if( window->specialKeyHandler != NULL )
        (*window->specialKeyHandler)( key, x, y );
}

void mouse_button_event( int button, int state, int x, int y )
{
    // Get the current glut window ID
    int glut_id = glutGetWindow();
    // Find the current window object
    Window *window = find_window(glut_id);
    // If set, call the event handler
    if( window->mouseButtonHandler != NULL )
        (*window->mouseButtonHandler)( button, state, x, y );
}

void mouse_active_motion_event( int x, int y )
{
    // Get the current glut window ID
    int glut_id = glutGetWindow();
    // Find the current window object
    Window *window = find_window(glut_id);
    // If set, call the event handler
    if( window->mouseActiveMotionHandler != NULL )
        (*window->mouseActiveMotionHandler)( x, y );
}

void mouse_passive_motion_event( int x, int y )
{
    // Get the current glut window ID
    int glut_id = glutGetWindow();
    // Find the current window object
    Window *window = find_window(glut_id);
    // If set, call the event handler
    if( window->mousePassiveMotionHandler != NULL )
        (*window->mousePassiveMotionHandler)( x, y );
}

void mouse_entry_event( int state )
{
    // Get the current glut window ID
    int glut_id = glutGetWindow();
    // Find the current window object
    Window *window = find_window(glut_id);
    // If set, call the event handler
    if( window->mouseEntryHandler != NULL )
        (*window->mouseEntryHandler)( state );
}
