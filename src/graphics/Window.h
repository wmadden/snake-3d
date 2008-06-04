#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <string>
#include <map>
#include <GL/glut.h>

using std::string;
using std::map;

namespace graphics
{

class Window;

class Window
{
public:
	////////////////////////////////////////////////////////////////////////////
	//  CONSTRUCTORS / DESTRUCTOR
	////////////////////////////////////////////////////////////////////////////
	Window( string title );
	virtual ~Window();
	
	////////////////////////////////////////////////////////////////////////////
	//  STATIC MEMBER FUNCTIONS
	////////////////////////////////////////////////////////////////////////////
	/**
	 * Returns the Window with the given ID.
	 * 
	 * @param int window_id
	 * 
	 * @return Window* A pointer to the window.
	 */
	static Window* getWindow( int window_id );
	
	////////////////////////////////////////////////////////////////////////////
	//  HELPER FUNCTIONS
	////////////////////////////////////////////////////////////////////////////
	int getStateVariable( GLenum en );
	
	////////////////////////////////////////////////////////////////////////////
	//  PROPERTY ACCESSORS
	////////////////////////////////////////////////////////////////////////////
	/**
	 * Returns the GLUT-assigned ID of the window.
	 */
	int getId();
	/**
	 * Set the window title.
	 */
	void setTitle( string title );
	/**
	 * Returns the window title.
	 */
	string getTitle();
	/**
	 * Sets the icon title.
	 */
	void setIconTitle( string title );
	/**
	 * Gets the icon title.
	 */
	void getIconTitle();
	/**
	 * Returns the window's width.
	 */
	int getWidth();
	/**
	 * Returns the window's height.
	 */
	int getHeight();
	/**
	 * Sets the window's width.
	 */
	void setWidth( int width );
	/**
	 * Sets the window's height.
	 */
	void setHeight( int height );
	/**
	 * Makes the window fullscreen.
	 */
	void makeFullScreen();
	/**
	 * Resizes the window.
	 */
	void resize( int width, int height );
	/**
	 * Moves the window.
	 */
	void move( int x, int y );
	/**
	 * Makes the window the current window.
	 */
	void makeCurrent();
	/**
	 * Flags the window as dirty, requiring a redraw.
	 */
	void makeDirty();
	/**
	 * Swaps the window's buffers.
	 */
	void swapBuffers();
	/**
	 * Shows the window.
	 */
	void show();
	/**
	 * Hides the window.
	 */
	void hide();
	/**
	 * Iconifies the window.
	 */
	void iconify();
	/**
	 * Sets the window cursor.
	 */
	void setCursor( int cursor );

	////////////////////////////////////////////////////////////////////////////
	//  EVENT TRIGGERS
	////////////////////////////////////////////////////////////////////////////
	void renderEvent();
	void resizeEvent( int width, int height );
	void keyboardEvent( unsigned char key, int x, int y );
	void keyboardSpecialEvent( int key, int x, int y );
	void mouseEvent( int button, int state, int x, int y );
	void mouseActiveMotionEvent( int x, int y );
	void mousePassiveMotionEvent( int x, int y );
	void mouseEntryEvent( int state );
protected:
	int windowID;
	string title;
	
	////////////////////////////////////////////////////////////////////////////
	//  EVENT HANDLERS
	////////////////////////////////////////////////////////////////////////////
	virtual void renderHandler();
	virtual void resizeHandler( int width, int height );
	virtual void keyboardHandler( unsigned char key, int x, int y );
	virtual void keyboardSpecialHandler( int key, int x, int y );
	virtual void mouseHandler( int button, int state, int x, int y );
	virtual void mouseActiveMotionHandler( int x, int y );
	virtual void mousePassiveMotionHandler( int x, int y );
	virtual void mouseEntryHandler( int state );

private:
	static map<int, Window*> windows;
	
	static void registerWindow( Window* window );
};

} // End of namespace 'graphics'

#endif /*WINDOW_H_*/
