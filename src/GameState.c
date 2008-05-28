#include "GameState.h"
#include <stdlib.h>

GameState* GameState_new(  )
{
    GameState *gamestate = (GameState*)calloc( sizeof(GameState), 1 );
    
    // Create the players
    gamestate->player1 = Player_new();
    gamestate->player2 = Player_new();
    
    return gamestate;
}

void GameState_delete( GameState* gamestate )
{
    // Delete the players
    if( gamestate->player1 != NULL )
    {
        Player_delete( gamestate->player1 );
    }
    if( gamestate->player2 != NULL )
    {
        Player_delete( gamestate->player2 );
    }
    
    GameState_clearProjectiles(gamestate);
    GameState_clearEdibles(gamestate);
    
    // Delete the landscape
    Landscape_delete( gamestate->landscape );
}

void GameState_clearProjectiles( GameState* gamestate )
{
    Object_delete(gamestate->player1_projectile);
    Object_delete(gamestate->player2_projectile);
    gamestate->player1_projectile = NULL;
    gamestate->player2_projectile = NULL;
}

void GameState_clearEdibles( GameState* gamestate )
{
    Object_delete(gamestate->edible);
    gamestate->edible = NULL;
}

