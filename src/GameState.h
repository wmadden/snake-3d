#ifndef GAMESTATE_H_
#define GAMESTATE_H_
/**
 * GameState.h
 * This module defines the Gamestate object, responsible for holding all data
 * used to determine the state of the game.
 */

#include "Player.h"
#include "Landscape.h"
#include "Object.h"

typedef enum {
    MODE_MENU, MODE_COUNTDOWN, MODE_RUNNING, MODE_PAUSED, MODE_FINISHED
} GameMode;

/**
 * This structure defines the game state - that is, the state of all game world
 * variables.
 */
typedef struct {
    GameMode mode; // The mode of the game (playing, paused, menu etc.)
    float countdown; // The number of seconds before the game starts
    
    // The game's players
    Player* player1;
    Player* player2;
    // The speed players travel, in distance units per second
    float playerSpeed;
    
    // Each player may have one projectile in existance at a time
    Projectile* player1_projectile;
    Projectile* player2_projectile;
    
    // There is only ever one edible in the game at a time
    Edible* edible;
    
    Landscape* landscape; // The landscape
} GameState;

GameState* GameState_new( /* params */ );
void GameState_delete( GameState* gamestate );
void GameState_clearProjectiles( GameState* gamestate );
void GameState_clearEdibles( GameState* gamestate );
void GameState_clearPlayers( GameState* gamestate );

#endif /*GAMESTATE_H_*/
