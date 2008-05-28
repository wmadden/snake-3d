#include "mechanics.h"
#include "input.h"
#include "render.h"
#include "Window.h"
#include <stdlib.h>

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
#define PROJECT_TITLE "Project 2"
#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_X 0
#define DEFAULT_WINDOW_Y 0


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
    int window_id;
    
    // Seed the random number generator
    srand( time(NULL) );
    
	/* Initialize windowing and rendering */
    window_id = Window_new(PROJECT_TITLE);

    // Create a new game
    new_game();
    // TODO: show main menu instead of immediately starting the game
    /* Initialize the game mechanics module */
    //mechanics_init( GET_CURRENT_TIME() );
    
    Window_registerReshapeEventHandler( window_id, window_resized );
    
    /* Initialize input */
    input_init(window_id);
    
	render_init();

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
    update_world(delta);
    
    /* TODO: Set the current window to the main window */
    
    /* Update the render module */
    render_update(delta);
    
    /* Render the game */
    render();
    
    /* TODO: set the current window to the debug window */
    /* TODO: output debug info */
    last_time = now;
}
