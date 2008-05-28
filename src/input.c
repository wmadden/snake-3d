#include "input.h"
#include "Window.h"
#include "mechanics.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <GL/glut.h>

/*******************************************************************************
 * GLOBALS AND CONSTANTS
 ******************************************************************************/
// integer id's for the players, will correspond to those used in mechanics.c
#define LEFT_PLAYER_ID 2
#define RIGHT_PLAYER_ID 1

// extra controls
#define EXIT_KEY 27 /* ESC */   /* Q is too close to WSAD */
#define PAUSE_KEY ' '
#define DEBUG_KEY '.'   // possibly print some debug output
#define HELP_KEY '?'    // possibly print usage notes relating to these keys

// keys used by the left player for movement
#define LEFT_PLAYER_TURN_LEFT 'a'
#define LEFT_PLAYER_TURN_RIGHT 'd'
#define LEFT_PLAYER_FIRE 'w'

// right player uses the arrow keys - it is harder to use #defines for these

/*******************************************************************************
 * INTERNAL PROTOTYPES
 ******************************************************************************/
void ascii_key_handler(unsigned char key, int x, int y);
void special_key_handler(int key, int x, int y);

/*******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/
void input_init( int window_id )
{
    // Register window input event handlers
    Window_registerKeyEventHandler( window_id, ascii_key_handler );
    Window_registerSpecialKeyEventHandler( window_id, special_key_handler );
}

void ascii_key_handler(unsigned char key, int x, int y)
{
    unsigned char keytest = tolower(key);       // ignore shift/capslock
    switch(keytest){
        case LEFT_PLAYER_TURN_LEFT:
            change_player_direction(LEFT_PLAYER_ID, TURN_LEFT);
            break;
        case LEFT_PLAYER_TURN_RIGHT:
            change_player_direction(LEFT_PLAYER_ID, TURN_RIGHT);
            break;
        case LEFT_PLAYER_FIRE:
            fire_player_weapon(LEFT_PLAYER_ID);
            break;
        case PAUSE_KEY:
            if( is_running() )
                pause_game();
            else if( is_paused() )
                resume_game();
            else
                new_game();
            break;
        case DEBUG_KEY:
            fprintf(stderr, "Debug output:\n");
            break;
        case HELP_KEY:
            fprintf(stderr, "Game Commands:\n");
            fprintf(stderr, "[%c]:\tSee this help\n", HELP_KEY);
            fprintf(stderr, "[%c]:\tSee debug print\n", DEBUG_KEY);
            fprintf(stderr, "[%c]:\tPause or unpause the game\n", PAUSE_KEY);
            if(EXIT_KEY == 27)
                fprintf(stderr, "[ESC]:\tExit the game\n");
            else
                fprintf(stderr, "[%c]:\tExit the game\n", EXIT_KEY);
            fprintf(stderr, "[%c]:\tFIRE!\t(left player)\n", LEFT_PLAYER_FIRE);
            fprintf(stderr, "[%c]:\tturn left\t(left player)\n", LEFT_PLAYER_TURN_LEFT);
            fprintf(stderr, "[%c]:\tturn right\t(left player)\n", LEFT_PLAYER_TURN_RIGHT);
            fprintf(stderr, "(up arrow):\tFIRE!\t(right player)\n");
            fprintf(stderr, "(left arrow):\tturn left\t(right player)\n");
            fprintf(stderr, "(right arrow):\tturn right\t(right player)\n");
            break;
        case EXIT_KEY:
            exit(0);
            break;
        default:
            fprintf(stderr, "unknown keyboard command\n");
            break;
    }
}

void special_key_handler(int key, int x, int y)
{
    switch(key){
        case GLUT_KEY_LEFT:
            change_player_direction(RIGHT_PLAYER_ID, TURN_LEFT);
            break;
        case GLUT_KEY_RIGHT:
            change_player_direction(RIGHT_PLAYER_ID, TURN_RIGHT);
            break;
        case GLUT_KEY_UP:
            fire_player_weapon(RIGHT_PLAYER_ID);
            break;
        /* default:
            fprintf(stderr, "unknown keyboard command\n");
            break; */
    }
}
