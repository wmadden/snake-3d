#ifndef MECHANICS_H_
#define MECHANICS_H_
/**
 * This module defines the rules by which the game state can change.
 * 
 * This is the only means by which user input can modify the gamestate. By
 * isolating access to the gamestate like this, we can ensure that the
 * mechanics - the fundamental game rules - which govern the game are enforced.
 */

#include "GameState.h"

/******************************************************************************
 * TYPEDEFS
 ******************************************************************************/
/**
 * This is used to change a player's direction of movement
 */
typedef enum {
    TURN_LEFT, TURN_RIGHT
} Turn;

/******************************************************************************
 * MODULE FUNCTIONS
 ******************************************************************************/
/**
 * This function initializes the mechanics module.
 */
void mechanics_init();

/******************************************************************************
 * PLAYER MECHANICS
 *****************************************************************************/
/**
 * Changes the player's direction.
 */
void change_player_direction( int player_id, Turn dir );
/**
 * Fires the player's weapon, if permitted.
 */
int fire_player_weapon( int player_id );

/******************************************************************************
 * WORLD MECHANICS
 *****************************************************************************/
/**
 * Updates the world, given the number of milliseconds that have passed.
 */
void update_world( int delta );

/******************************************************************************
 * GAME MECHANICS
 *****************************************************************************/
/**
 * Starts a new game, clearing the current one if already running.
 */
void new_game();
/**
 * Pauses a running game.
 */
void pause_game();
/**
 * Checks if the game is paused.
 */
int is_paused();
/**
 * Resume game.
 */
void resume_game();
/**
 * End the game.
 */
void end_game();

/*******************************************************************************
 * GAMESTATE FUNCTIONS
 ******************************************************************************/
GameState* get_gamestate();

#endif /*MECHANICS_H_*/
