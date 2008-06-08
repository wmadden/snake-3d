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
	Window( string title );
	virtual ~Window();
	
	/**
	 * Returns the Window with the given ID.
	 * 
	 * @param int window_id
	 * 
	 * @return Window* A pointer to the window.
	 */
	static Window* getWindow( int window_id );
	
	int getStateVariable( GLenum en );
	
	/**
	 * Returns the GLUT-assigned ID of the window.
	 */
	int getId();
	/**
	 * Makes the window the current window.
	 */
	void makeCurrent();

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
