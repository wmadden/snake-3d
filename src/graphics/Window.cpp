#include "Window.h"

namespace graphics 
{

bool initialized = false;
map<int, Window*> Window::windows;

void initialize();

Window::Window( string title )
{
	windowID = glutCreateWindow( title.c_str() );
	
	registerWindow(this);
	
	if( !initialized )
		initialize();
}

Window::~Window()
{
	glutDestroyWindow(windowID);
}

///////////////////////////////////////////////////////////////////////////////
//  STATIC MEMBER FUNCTIONS
///////////////////////////////////////////////////////////////////////////////
void Window::registerWindow( Window* window )
{
	Window::windows[ window->getId() ] = window;
}

Window* Window::getWindow( int window_id )
{
	map<int, Window*>::iterator it = Window::windows.find(window_id);
	
	if( it == Window::windows.end() )
		return NULL;
	
	return it->second;
}

///////////////////////////////////////////////////////////////////////////////
//  HELPER FUNCTIONS
///////////////////////////////////////////////////////////////////////////////
int Window::getStateVariable( GLenum en )
{
	// Get the current window
	int cur = glutGetWindow();
	// Set the current window to this window
	glutSetWindow( this->windowID );
	// Get the state variable
	int result = glutGet( en );
	
	glutSetWindow(cur);
	return result;
}

int Window::getId()
{
	return this->windowID;
}

void Window::makeCurrent()
{
	glutSetWindow( this->windowID );
}

void Window::renderEvent()
{
	this->renderHandler();
}

void Window::resizeEvent( int width, int height )
{
	this->resizeHandler( width, height );
}

void Window::keyboardEvent( unsigned char key, int x, int y )
{
	this->keyboardHandler( key, x, y );
}

void Window::keyboardSpecialEvent( int key, int x, int y )
{
	this->keyboardSpecialHandler( key, x, y );
}

void Window::mouseEvent( int button, int state, int x, int y )
{
	this->mouseHandler( button, state, x, y );
}

void Window::mouseActiveMotionEvent( int x, int y )
{
	this->mouseActiveMotionHandler( x, y );
}

void Window::mousePassiveMotionEvent( int x, int y )
{
	this->mousePassiveMotionHandler( x, y );
}

void Window::mouseEntryEvent( int state )
{
	this->mouseEntryHandler( state );
}

void Window::renderHandler()
{
}

void Window::resizeHandler( int width, int height )
{
}

void Window::keyboardHandler( unsigned char key, int x, int y )
{
}

void Window::keyboardSpecialHandler( int key, int x, int y )
{
}

void Window::mouseHandler( int button, int state, int x, int y )
{
}

void Window::mouseActiveMotionHandler( int x, int y )
{
}

void Window::mousePassiveMotionHandler( int x, int y )
{
}

void Window::mouseEntryHandler( int state )
{
}

/*******************************************************************************
 *  GLUT CALLBACK REROUTING
 *  -----------------------
 *  The purpose of these functions is to reroute GLUT callbacks (coded in the
 *  traditional C style) to individual Window objects' event handlers. This is
 *  necessary because C callbacks can't take member function pointers. It's
 *  a little hacky, but it should work fine.
 ******************************************************************************/

void register_window_callbacks();
void render_router();
void resize_router( int width, int height );
void keyboard_router( unsigned char key, int x, int y );
void keyboardSpecial_router( int key, int x, int y );
void mouse_router( int button, int state, int x, int y );
void mouseActiveMotion_router( int x, int y );
void mousePassiveMotion_router( int x, int y );
void mouseEntry_router( int state );

/**
 * This function initializes the Window callback system, registering glut
 * callbacks to be rerouted to the appropriate Window object.
 */
void initialize()
{
	// TODO: reconsider this catch. Is it really necessary?
	if( initialized == true )
		return;
	
	// Register callbacks.
	register_window_callbacks();
	
	initialized = true;
}

void register_window_callbacks()
{
	glutDisplayFunc( render_router );
	glutReshapeFunc( resize_router );
	glutKeyboardFunc( keyboard_router );
	glutMouseFunc( mouse_router );
	glutMotionFunc( mouseActiveMotion_router );
	glutPassiveMotionFunc( mousePassiveMotion_router );
	glutEntryFunc( mouseEntry_router );
	glutSpecialFunc( keyboardSpecial_router );
}

void render_router()
{
	Window* window = Window::getWindow( glutGetWindow() );
	if( window != NULL )
		window->renderEvent();
}

void resize_router( int width, int height )
{
	Window* window = Window::getWindow( glutGetWindow() );
	if( window != NULL )
		window->resizeEvent( width, height );
}

void keyboard_router( unsigned char key, int x, int y )
{
	Window* window = Window::getWindow( glutGetWindow() );
	if( window != NULL )
		window->keyboardEvent( key, x, y );
}

void keyboardSpecial_router( int key, int x, int y )
{
	Window* window = Window::getWindow( glutGetWindow() );
	if( window != NULL )
		window->keyboardSpecialEvent( key, x, y );
}

void mouse_router( int button, int state, int x, int y )
{
	Window* window = Window::getWindow( glutGetWindow() );
	if( window != NULL )
		window->mouseEvent( button, state, x, y );
}

void mouseActiveMotion_router( int x, int y )
{
	Window* window = Window::getWindow( glutGetWindow() );
	if( window != NULL )
		window->mouseActiveMotionEvent( x, y );
}

void mousePassiveMotion_router( int x, int y )
{
	Window* window = Window::getWindow( glutGetWindow() );
	if( window != NULL )
		window->mousePassiveMotionEvent( x, y );
}

void mouseEntry_router( int state )
{
	Window* window = Window::getWindow( glutGetWindow() );
	if( window != NULL )
		window->mouseEntryEvent( state );
}

} // End of namespace 'graphics'
