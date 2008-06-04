/*******************************************************************************
 * main.cpp
 * 
 * This file defines the entry point for the application.
 * It performs general initialization, and contains the game loop.
 ******************************************************************************/
#include "graphics/Window.h"
#include <stdlib.h>

using namespace graphics;


/*******************************************************************************
 * MACROS
 ******************************************************************************/
#ifdef WIN32

// Define Windows-specific stuff here
#include "Windows.h"

#elif unix

// Define *NIX-specific stuff here

#endif

#include <time.h>
// The following returns the number of milliseconds since system start
#define GET_CURRENT_TIME() clock() / (CLOCKS_PER_SEC / 1000.0f)

// Make sure glut.h is included after stdlib.h to keep the VS compiler from
// throwing a hissy fit
#include <GL/glut.h>


/*******************************************************************************
 * GLOBALS AND CONSTANTS
 ******************************************************************************/
#define WINDOW_TITLE "Snake 3D"
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_X 0
#define DEFAULT_WINDOW_Y 0

Window* main_window = NULL;
Window* debug_window = NULL;

/*******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/
void initialize_GLUT( int*, char** );
void game_loop();
void initialize_game();


/*******************************************************************************
 * ENTRY POINT
 ******************************************************************************/
int main( int argc, char** argv )
{
    /* Initialize GLUT */
    initialize_GLUT( &argc, argv );
    
    /* Initialize game modules */
    initialize_game();
    
    /* Start event processing */
    glutMainLoop();
    
    return 0;
}


/*******************************************************************************
 * FUNCTIONS
 ******************************************************************************/
/**
 * Initializes GLUT.
 */
void initialize_GLUT( int* argc, char** argv )
{
    // Set up the GLUT defaults
    glutInitWindowSize( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT );
    glutInitWindowPosition( DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y );
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    
    // Let GLUT parse command line arguments to override defaults set above
    glutInit( argc, argv );
}

/**
 * Initializes game modules.
 * 
 * Mostly what this function does is link up the various game modules.
 */
void initialize_game()
{    
    // Seed the random number generator
    srand( time(NULL) );
    
    /* Initialize windowing and rendering */
    main_window = new Window(WINDOW_TITLE);

    /* Create a new game */
    // TODO: show main menu instead of immediately starting the game
    
    /* Initialize the game mechanics module */
    //mechanics_init( GET_CURRENT_TIME() );
    
    /* Initialize input */

    /* Register the game loop */
    glutIdleFunc(game_loop);
}

/**
 * The game loop, this function is called periodically to update the game.
 */
void game_loop()
{
    static int last_time = -1;
    int delta, now;
    
    /* Update the game */
    now = GET_CURRENT_TIME();
    
    if( last_time == -1 )
    {
        last_time = now;
        return;
    }
    
    // Get the time since the last update (minimum of one millisecond)
    delta = 1 + now - last_time; // TODO: remove the 1
    
    /* Render the game */
    
    last_time = now;
}
