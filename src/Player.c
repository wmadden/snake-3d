#include "Player.h"
#include <stdlib.h>
#include "maths.h"

// Default player colours
#define PLAYER1_R 1.0f
#define PLAYER1_G 1.0f
#define PLAYER1_B 0.0f

#define PLAYER2_R 0.0f
#define PLAYER2_G 0.0f
#define PLAYER2_B 1.0f

Player* Player_new( /* params */ )
{
    static int count = 0;
    
    Player *player = (Player*)calloc( sizeof(Player), 1 );
    
    switch( count )
    {
    case 0:
        player->color[0] = PLAYER1_R;
        player->color[1] = PLAYER1_G;
        player->color[2] = PLAYER1_B;
        break;
    case 1:
        player->color[0] = PLAYER2_R;
        player->color[1] = PLAYER2_G;
        player->color[2] = PLAYER2_B;
        count = -1;
        break;
    }
    
    count++;
    
    return player;
}

void Player_delete( Player* player )
{
    // Free the player's body
    while( player->head != NULL )
    {
        free(player->head);
        player->head = player->head->next;
    }
    
    free(player);
}

Body* Body_new( int row, int column, float x, float y, float z )
{
    Body* body = (Body*)calloc( sizeof(Body), 1 );
    
    body->position = (float*)malloc( sizeof(float) * 3 );
    body->position[0] = x;
    body->position[1] = y;
    body->position[2] = z;
    
    body->gridPosition[0] = row;
    body->gridPosition[1] = column;
    
    return body;
}

void Body_delete( Body* body )
{
    if( body->next != NULL )
    {
        Body_delete(body->next);
    }
    if( body->previous != NULL )
    {
        body->previous->next = NULL;
    }
    
    free(body->position);
    
    free(body);
}

void Body_push( Body* new_segment, Body** body )
{
    new_segment->next = *body;
    (*body)->previous = new_segment;
    *body = new_segment;
}

void Body_setPosition( Body* body, int row, int column,
                       float x, float y, float z )
{
    body->gridPosition[0] = row;
    body->gridPosition[1] = column;
    body->position[0] = x;
    body->position[1] = y;
    body->position[2] = z;
}
