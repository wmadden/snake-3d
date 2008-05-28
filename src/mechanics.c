#include "mechanics.h"
#include "GameState.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "maths.h"

/******************************************************************************
 * GLOBALS AND CONSTANTS
 ******************************************************************************/
// The number of points across the landscape grid
#define DEFAULT_GRID_SIZE 129

// Define the bounds of the game world
#define WORLD_WEST_BOUND -4.0f
#define WORLD_SOUTH_BOUND -4.0f

#define WORLD_MINHEIGHT 0.0f
#define WORLD_MAXHEIGHT 1.5f

#define WORLD_WIDTH 8.0f
#define WORLD_DEPTH 8.0f

// Distance units per second per second
#define GRAVITY -4.9

// The players' speed, in grid divisions per second
#define PLAYER_SPEED 20
// Initial maximum length of a player, in grid divisions
#define PLAYER_INITIAL_LENGTH 5
// Player's radius, in grid divisions
#define PLAYER_RADIUS 0.5

// Food is generated above the landscape, and falls to the ground.
// This gives the height above the landscape that it starts
#define INITIAL_FOOD_HEIGHT 5.0f
#define POINTS_FOR_FOOD 100
// The radius of edibles, in grid divisions
#define FOOD_RADIUS 3.0f
// The amount of tail food gives you, in grid divisions
#define FOOD_LENGTH_INCREMENT 10.0f

// Projectile speed, in grid divisions per second
#define PROJ_SPEED 3.0f
// deformation amount is the depth in divisions
// radius is also in divisions
#define DEFORMATION_RADIUS 80.0f
#define DEFORMATION_AMOUNT 5.0f

GameState* gamestate = NULL;


/******************************************************************************
 * INTERNAL FUNCTION PROTOTYPES
 ******************************************************************************/
/* Domain rules:
 *   1. Collisions
 *      1. Between player head and player tail
 *      2. Between player head and wall
 *      3. Between player head and food
 *      4. Between player head and projectile
 *      5. Between player head and player head
 *      6. Between projectiles and landscape
 *      7. Between projectiles and walls
 *      8. Between food and the landscape
 *   2. Movement
 *      1. Players can only move on grid lines
 *      2. Speed is constant
 *      3. Projectiles fall
 *      4. Food falls
 *   3. Initialization
 *      1. Random landscape is generated
 *      2. Players begin on opposite sides
 *      3. Any food or projectiles in the game are destroyed
 *      4. The first edible is generated
 */
int player_player_collision( int );
int player_wall_collision( Player*, Direction );
int player_food_collision( Player* );
int player_projectile_collision( Player*, Projectile* );
void projectile_landscape_collision( Projectile*, Landscape* );
void projectile_wall_collision( Projectile*, Direction );
void edible_landscape_collision( Edible*, Landscape* );

void update_players( int delta );
void update_projectiles( int delta );
void update_projectile( Projectile* projectile, int delta );
void update_food( int delta );
void apply_gravity( Object* object, int delta );
void apply_object_velocity( Object* object, int delta );

void set_next_point( Player* player );

void move_player( Player* player, int delta );
int test_player_collisions( Player* player, float movement );
void clear_gamestate();
void generate_edible();

void update_player_positions( Player* player );
void calculate_offset_position( Direction dir, float position[3],
                                float offset, float from[3], float to[3] );
void set_player_forward_vector( Player* player, Landscape* landscape );
void set_player_up_vector( Player* player );

void initialize_player( Player* player, int row, int column, Direction dir );
float distance_to_next_point( Player* player );
void closest_grid_coordinate( float X, float Z, int* row, int* column );
float get_height_at_point( Landscape* landscape, float X, float Z );


void deform_landscape( float X, float Z, Landscape* landscape );

void print_score();


/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/
// TODO: delete this function
void mechanics_init( int current_time )
{
}

void update_world( int delta )
{
    if( gamestate->mode != MODE_RUNNING )
        return;
    
    // Move players
    update_players(delta);
    // Update projectiles
    update_projectiles(delta);
    // Update food
    update_food(delta);
}

void new_game()
{
    /* If this is the first game, create the gamestate.
     * Otherwise, clear it. */
    if( gamestate == NULL )
    {
        gamestate = GameState_new();
        gamestate->landscape = Landscape_new( DEFAULT_GRID_SIZE,
                                              WORLD_MINHEIGHT,
                                              WORLD_MAXHEIGHT,
                                              WORLD_SOUTH_BOUND,
                                              WORLD_WEST_BOUND,
                                              WORLD_WIDTH,
                                              WORLD_DEPTH );
        gamestate->mode = MODE_RUNNING;
    }
    else
    {
        clear_gamestate();
    }
    
    /* Set up initial game conditions */
    // Set the player speed.
    gamestate->playerSpeed = PLAYER_SPEED * gamestate->landscape->gridDivisionWidth;
    // Generate the landscape
    Landscape_generate( gamestate->landscape );
    
    // Put the players on opposite sides
    // Start player 1 at the southmost edge, in the middle, moving north
    initialize_player( gamestate->player1,
                       gamestate->landscape->gridWidth / 2,
                       0,
                       DIRECTION_NORTH );
    // Start player 2 at the northmost edge, in the middle, moving south
    initialize_player( gamestate->player2,
                       gamestate->landscape->gridWidth / 2,
                       gamestate->landscape->gridWidth - 1,
                       DIRECTION_SOUTH );
    
    // Generate the first edible
    generate_edible();
}

int fire_player_weapon( int player_id )
{
    Player* player;
    Projectile* projectile;
    
    if( gamestate->mode != MODE_RUNNING )
    {
        return;
    }
    
    if( player_id == 1 )
    {
        player = gamestate->player1;
        // Don't let them fire if they have a projectile in existance
        if( gamestate->player1_projectile != NULL )
            return 0;
    }
    else if( player_id == 2 )
    {
        player = gamestate->player2;
        if( gamestate->player2_projectile != NULL )
            return 0;
    }
    else
        return 0;
    
    // Do not let the player fire their weapon if they're beneath the landscape
    if( player->underGround )
        return 0;
    
    // Create a new projectile just outside the bounds of the player's head,
    // with an appropriate velocity
    projectile = Object_new();
    projectile->velocity[0] = player->forward[0] * gamestate->playerSpeed;
    projectile->velocity[1] = player->forward[1] * gamestate->playerSpeed;
    projectile->velocity[2] = player->forward[2] * gamestate->playerSpeed;
    
    // Make it 45 degrees
    float origin[3] = {0, 0, 0};
    projectile->velocity[1] += distance_between_points(origin, projectile->velocity) / 2.0;
    
    // Normalise the velocity
    normaliseVector(projectile->velocity);
    
    // Make it move fast
    projectile->velocity[0] *= PROJ_SPEED;
    projectile->velocity[1] *= PROJ_SPEED;
    projectile->velocity[2] *= PROJ_SPEED;
    
    // Set the projectile's position
    projectile->position[0] = player->headPosition[0];
    projectile->position[1] = player->headPosition[1] +  0.5 *
    gamestate->landscape->gridDivisionWidth; // TODO: refine this;
    projectile->position[2] = player->headPosition[2];
    
    // Set the projectile's radius
    projectile->radius = gamestate->landscape->gridDivisionWidth;
    
    // Set it in the gamestate
    if( player_id == 1 )
        gamestate->player1_projectile = projectile;
    else
        gamestate->player2_projectile = projectile;
    
    return 1;
}

void
change_player_direction( int player_id, Turn dir )
{
    Player* player = NULL;

    if( player_id == 1 )
    {
        player = gamestate->player1;
    }
    else if( player_id == 2 )
    {
        player = gamestate->player2;
    }
    
    /* Note: a cursory glance at the logic below will show that the directions
     * are all back to front. This is deliberate.
     * 
     * OpenGL has the interesting habit of reversing the Z axis. This means that
     * increasing values on the Z axis are the opposite to what you'd expect.
     */
    
    switch( player->currentDir )
    {
    case DIRECTION_NORTH:
        if( dir == TURN_RIGHT )
        {
            player->nextDir = DIRECTION_WEST;
        }
        else
        {
            player->nextDir = DIRECTION_EAST;
        }
        break;
    case DIRECTION_SOUTH:
        if( dir == TURN_RIGHT )
        {
            player->nextDir = DIRECTION_EAST;
        }
        else
        {
            player->nextDir = DIRECTION_WEST;
        }
        break;
    case DIRECTION_EAST:
        if( dir == TURN_RIGHT )
        {
            player->nextDir = DIRECTION_NORTH;
        }
        else
        {
            player->nextDir = DIRECTION_SOUTH;
        }
        break;
    case DIRECTION_WEST:
        if( dir == TURN_RIGHT )
        {
            player->nextDir = DIRECTION_SOUTH;
        }
        else
        {
            player->nextDir = DIRECTION_NORTH;
        }
        break;
    }
}

void pause_game(){
    gamestate->mode = MODE_PAUSED;
}

void resume_game(){
    gamestate->mode = MODE_RUNNING;
}

int is_paused(){
    return gamestate->mode == MODE_PAUSED;
}

GameState* get_gamestate()
{
    return gamestate;
}


/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/
void initialize_player( Player* player, int row, int column, Direction dir )
{
    // Set the player's direction
    player->currentDir = player->nextDir = dir;
    
    // Set the player's maximum length to five divisions
    player->maxLength = gamestate->landscape->gridDivisionWidth * PLAYER_INITIAL_LENGTH;
    
    // Set their radius
    player->radius = gamestate->landscape->gridDivisionWidth * PLAYER_RADIUS;
    
    // Set their initial point
    player->head = player->tail =
        Body_new( row, column,
                  gamestate->landscape->pointMap[row][column][0],
                  gamestate->landscape->pointMap[row][column][1],
                  gamestate->landscape->pointMap[row][column][2] );
    
    // Set their next point
    set_next_point(player);
    
    // Set their head and tail offsets
    player->headOffset = 0.0f;
    // Set the tail offset to maximum, so that head and tail are both at the
    // starting point. When the player moves, collision detection should take
    // place *after* moving the head away.
    player->tailOffset = gamestate->landscape->gridDivisionDepth;
    
    // Update the player. That is, recalculate their head and tail positions,
    // as well as their forward and up vectors.
    update_player_positions(player);
}

/**
 * This function sets the next point the player will reach.
 * Also sets their forward and up vectors.
 * 
 * Takes as parameter the player whose properties are being set.
 */
void set_next_point( Player* player )
{
    int next_point[2];
    Landscape* landscape = gamestate->landscape;
    
    // Change their direction
    player->currentDir = player->nextDir;
    // Set their head offset to zero
    player->headOffset = 0.0f;
    
    // Find the next grid point
    switch( player->currentDir )
    {
    case DIRECTION_NORTH:
        next_point[0] = player->head->gridPosition[0];
        next_point[1] = player->head->gridPosition[1] + 1;
        break;
    case DIRECTION_SOUTH:
        next_point[0] = player->head->gridPosition[0];
        next_point[1] = player->head->gridPosition[1] - 1;
        break;
    case DIRECTION_EAST:
        next_point[0] = player->head->gridPosition[0] + 1;
        next_point[1] = player->head->gridPosition[1];
        break;
    case DIRECTION_WEST:
        next_point[0] = player->head->gridPosition[0] - 1;
        next_point[1] = player->head->gridPosition[1];
        break;
    default:
        exit(1); // Should never happen
    }
    
    // Check that they haven't collided with the walls
    /* This is possibly not the appropriate place for such a check, but we can't
     * calculate their forward vector if they're on an edge heading out. */
    if( next_point[0] < 0 )
    {
        player_wall_collision(player, DIRECTION_WEST);
        return;
    }
    else if( next_point[0] >= landscape->gridWidth )
    {
        player_wall_collision(player, DIRECTION_EAST);
        return;
    }
    if( next_point[1] < 0 )
    {
        player_wall_collision(player, DIRECTION_SOUTH);
        return;
    }
    else if( next_point[1] >= landscape->gridWidth )
    {
        player_wall_collision(player, DIRECTION_NORTH);
        return;
    }
    
    // Create the new body segment
    Body_push( Body_new( next_point[0],next_point[1],
         gamestate->landscape->pointMap[next_point[0]][next_point[1]][0],
         gamestate->landscape->pointMap[next_point[0]][next_point[1]][1],
         gamestate->landscape->pointMap[next_point[0]][next_point[1]][2] ),
         &(player->head) );
}

void update_players( int delta )
{
    move_player(gamestate->player1, delta);
    update_player_positions(gamestate->player1);
    move_player(gamestate->player2, delta);
    update_player_positions(gamestate->player2);
}

/**
 * Called to indicate that a player has collided with a wall.
 * 
 * `player` gives the player who is colliding, and wall gives the wall they're
 * colliding with.
 */
int player_wall_collision( Player* player, Direction wall )
{
    // Reverse their direction, and put them back on a viable grid coordinate
    if( player->currentDir == DIRECTION_NORTH )
    {
        player->currentDir = player->nextDir = DIRECTION_SOUTH;
        player->head->gridPosition[1]--;
    }
    else if( player->currentDir == DIRECTION_EAST )
    {
        player->currentDir = player->nextDir = DIRECTION_WEST;
        player->head->gridPosition[0]--;
    }
    else if( player->currentDir == DIRECTION_SOUTH )
    {
        player->currentDir = player->nextDir = DIRECTION_NORTH;   
        player->head->gridPosition[1]++;
    }
    else if( player->currentDir == DIRECTION_WEST )
    {
        player->currentDir = player->nextDir = DIRECTION_EAST;
        player->head->gridPosition[0]++;
    }
    
    // Put them on the opposite side of the map
    player->underGround = !player->underGround;
    
    // TODO: this is not enough to make them go underground without errors
    printf("Player %d wins! (Player %d crashed into a wall)\n",
           player == gamestate->player1 ? 2 : 1,
           player == gamestate->player1 ? 1 : 2 );
    
    print_score();
    
    exit(0);
}

void move_player( Player* player, int delta )
{    
    /* Algorithm for moving the player:
     *    1. Find the amount to move them (speed x delta)
     *    2. If the amount to move is greater than the distance to the next
     *       point
     *       1. Move the head to the next point
     *       2. Set the `amount moved` to the headOffset
     *       3. Find the player's next point
     *    2. Else,
     *       1. Increment their offset
     *       2. Set the `amount moved` to the amount to move
     *    3. Increment their length by the `amount moved`
     *    3. If the player's length is now greater than their maximum length
     *       1. Find the `excess length` (max length - current length)
     *       2. If the `excess length` is greater than the tail length
     *          1. Pop the last body segment off the player's body
     *          2. Reduce the excess length by the tail offset
     *          3. Set the tail offset to zero
     *          4. Go back to 3.2.1
     *       3. Else
     *          1. Reduce the tail length by the `excess length`
     *    4. Reduce the `amount to move` by the `amount moved`
     *    5. Go back to 1
     */
    // Player speed is in distance units per second, delta is in milliseconds
    float amount_to_move = gamestate->playerSpeed * (delta / 1000.0f);
    // Speed is fixed for both players
    float amount_moved = 0;
    float distance;
    
    // While we still have to move them
    while( amount_to_move > 0 )
    {
        // We haven't moved them at all yet
        amount_moved = 0;
        
        // Work out how far to the next point
        distance = distance_to_next_point(player);
        
        // If the amount we need to move them is more than the distance to the
        // next point
        if( amount_to_move > distance )
        {
            // Check for collisions
            if( !test_player_collisions(player, distance) )
            {
                return;
            }
            // Set the amount moved
            amount_moved = distance;
            // Set the next point the player will hit
            set_next_point(player);
            // Set their forward vector
            set_player_forward_vector( player, gamestate->landscape );
            // Set their up vector
            set_player_up_vector( player );
        }
        else
        {
            // Check for collisions
            if( !test_player_collisions(player, distance) )
            {
                return;
            }
            // Move the player's head towards the next point by `amount_to_move`
            player->headOffset += amount_to_move;
            // Increment `amount_moved` by the amount we moved
            amount_moved = amount_to_move;
        }
        
        // Test for collisions
        //test_player_collisions( player, amount_moved );
        
        // Increase the player's length by the amount we moved the head
        player->length += amount_moved;
        
        // If they are now longer than they're allowed to be, we need to move
        // the tail
        float tail_excess = player->length - player->maxLength;
        while( tail_excess > 0 )
        {
            
            // If the excess tail is less than or equal to the tail offset
            if( tail_excess <= player->tailOffset )
            {
                // Reduce the offset
                player->tailOffset -= tail_excess;
                // Reduce the player's length
                player->length -= tail_excess;
                break; // We're done
            }
            else
            {
                // Reduce the excess by the offset
                tail_excess -= player->tailOffset;
                // Reduce the player's length
                player->length -= player->tailOffset;
                // Set the offset to maximum
                player->tailOffset = gamestate->landscape->gridDivisionDepth;//TODO
                // Pop the last point off the tail
                player->tail = player->tail->previous;
                Body_delete(player->tail->next);
                player->tail->next = NULL;
            }
        }
        
        // Decrease the amount we need to move them
        amount_to_move -= amount_moved;
    }
}

int test_player_collisions( Player* player, float movement )
{
    // Construct the line from the player's current position to where they will
    // be after the movement
    Point before, after;
    Player* other = (player == gamestate->player1 ? gamestate->player2 :
                                                    gamestate->player1);
    
    after[0] = before[0] = player->headPosition[0];
    after[1] = before[1] = player->headPosition[1];
    after[2] = before[2] = player->headPosition[2];
    // The player's new head position
    calculate_offset_position( player->currentDir, after,
                               movement,
                               player->headPosition,
                               player->head->position );
    
    // See if they're going to hit the food
    // (Need the distance between a point and a line)
    float distance = distance_between_points( after, gamestate->edible->position );
    if( distance < player->radius + gamestate->edible->radius )
    {
        return player_food_collision(player);
    }
    
    // See if they're going to hit the other player's head
    distance = distance_between_points( after, other->headPosition );
    if( distance < 
        player->radius * 2.0 )
    {
        // Head on collision
        return player_player_collision(1);
    }
    
    // See if they're going to hit the other player's body
    Body *body;
    float radius = player->radius + other->radius;
    float distToBody;
    
    body = other->head->next;
    
    while( body->next != other->tail && body->next != NULL )
    {
        
        /*distToBody = dist_line_to_line( body->position,
                                        body->next->position,
                                        before,
                                        after );
        if( distToBody < radius )
            return player_player_collision(0);*/
        
        if( player->head->next->position[0] == body->position[0] &&
            player->head->next->position[1] == body->position[1] && 
            player->head->next->position[2] == body->position[2] )
        {
            return player_player_collision(0);
        }
        
        body = body->next;
    }
    
    body = player->head->next;
        
    while( body->next != other->tail && body->next != NULL )
    {
        
        /*distToBody = dist_line_to_line( body->position,
                                        body->next->position,
                                        before,
                                        after );
        if( distToBody < radius )
            return player_player_collision(0);*/
        
        if( after[0] == body->position[0] && 
            after[1] == body->position[1] && 
            after[2] == body->position[2] )
        {
            return player_player_collision(0);
        }
        
        body = body->next;
    }
    
    return 1;
}

/**
 * Calculates the distance to the next point from a players current position.
 */
float distance_to_next_point( Player* player )
{
    float distance, distance_travelled;
    
    if( player->currentDir == DIRECTION_NORTH || 
        player->currentDir == DIRECTION_SOUTH )
    {
        distance = gamestate->landscape->gridDivisionDepth;
    }
    else
    {
        distance = gamestate->landscape->gridDivisionWidth;
    }
    
    distance_travelled = player->headOffset;
    
    return distance - distance_travelled;
}

/**
 * Called to indicate that a player has collided with another player, or
 * themself.
 * 
 * This function indicates that `player1` has collided with `player2`.
 * The parameter `head_on` indicates a head on collision, where both players
 * should be penalised.
 */
int player_player_collision( int head_on )
{
    if( head_on )
    {
        printf("Players collided head on!\n(You guys suck)\n");
    }
    else
    {
        printf("Player hit the body of another player\n");
    }
    
    print_score();
    exit(0);
}

/**
 * Updates a player.
 * 
 * The purpose of this function is to update a player's head and tail positions.
 */
void update_player_positions( Player* player )
{
    calculate_offset_position( player->currentDir, player->headPosition,
                               player->headOffset,
                               player->head->next->position,
                               player->head->position );
    calculate_offset_position( player->currentDir, player->tailPosition,
                                   player->tailOffset,
                                   player->tail->previous->position,
                                   player->tail->position );
}

void calculate_offset_position( Direction dir, float position[3],
                                float offset, float from[3], float to[3] )
{
    float scalar;
    
    if( dir == DIRECTION_NORTH || dir == DIRECTION_SOUTH )
    {
        scalar = offset / gamestate->landscape->gridDivisionDepth;
    }
    else
    {
        scalar = offset / gamestate->landscape->gridDivisionWidth;
    }
    
    position[0] = from[0] + scalar * (to[0] - from[0]);
    position[1] = from[1] + scalar * (to[1] - from[1]);
    position[2] = from[2] + scalar * (to[2] - from[2]);
}

void set_player_forward_vector( Player* player, Landscape* landscape )
{
    int next_row, next_col, last_row, last_col;
    
    next_row = player->head->gridPosition[0];
    next_col = player->head->gridPosition[1];
    last_row = player->head->next->gridPosition[0];
    last_col = player->head->next->gridPosition[1];
    
    player->forward[0] = landscape->pointMap[next_row][next_col][0] -
                         landscape->pointMap[last_row][last_col][0];
    player->forward[1] = landscape->pointMap[next_row][next_col][1] -
                         landscape->pointMap[last_row][last_col][1];
    player->forward[2] = landscape->pointMap[next_row][next_col][2] -
                         landscape->pointMap[last_row][last_col][2];
    
    normaliseVector(player->forward);
}

void set_player_up_vector( Player* player )
{
    // If moving EAST/WEST, Z remains the same
    if( player->currentDir == DIRECTION_EAST ||
        player->currentDir == DIRECTION_WEST )
    {
        player->up[0] = -player->forward[1];
        player->up[1] = fabs(player->forward[0]) * (player->underGround ? -1.0 : 1.0);
        player->up[2] = player->forward[2];
    }
    
    // If moving NORTH/SOUTH, X remains the same
    else
    {
        player->up[0] = player->forward[0];
        player->up[1] = fabs(player->forward[2]) * (player->underGround ? -1.0 : 1.0);
        player->up[2] = -player->forward[1];
    }
}

void clear_gamestate()
{
    // Clear any projectiles in the game
    GameState_clearProjectiles(gamestate);
    // Clear any food in the game
    GameState_clearEdibles(gamestate);
}

void generate_edible()
{
    // Create a new edible at a random grid location
    int row, column;
    Edible* edible = gamestate->edible = Object_new();
    
    // Generate random coordinates
    row = bounded_random(0, gamestate->landscape->gridWidth);
    column = bounded_random(0, gamestate->landscape->gridWidth);
    
    edible->position[0] = gamestate->landscape->pointMap[row][column][0];
    edible->position[1] = gamestate->landscape->pointMap[row][column][1];
    edible->position[2] = gamestate->landscape->pointMap[row][column][2];
    edible->position[1] += INITIAL_FOOD_HEIGHT;
    edible->radius = gamestate->landscape->gridDivisionWidth * FOOD_RADIUS;
}

void update_projectiles( int delta )
{
    update_projectile(gamestate->player1_projectile, delta);
    update_projectile(gamestate->player2_projectile, delta);
}

void update_projectile( Projectile* projectile, int delta )
{
    float height;
    
    if( projectile != NULL )
    {
        // Check that it's within the boundaries of the landscape
        if( projectile->position[0] < gamestate->landscape->westBound )
        {
            projectile_wall_collision( projectile, DIRECTION_WEST );
            return;
        }
        else if( projectile->position[0] > gamestate->landscape->eastBound )
        {
            projectile_wall_collision( projectile, DIRECTION_EAST );
            return;
        }
        if( projectile->position[2] < gamestate->landscape->southBound )
        {
            projectile_wall_collision( projectile, DIRECTION_SOUTH );
            return;
        }
        else if( projectile->position[2] > gamestate->landscape->northBound )
        {
            projectile_wall_collision( projectile, DIRECTION_NORTH );
            return;
        }
        
        // The height of the landscape at the projectile's location
        height = Landscape_getHeight( gamestate->landscape,
                                      projectile->position[0],
                                      projectile->position[2] );
        
        // Apply gravity to the projectile
        apply_gravity( projectile, delta );
        
        // Apply its velocity
        apply_object_velocity( projectile, delta );
        
        // If it is below ground level, deform the landscape
        if( projectile->position[1] <= height )
        {
            projectile_landscape_collision( projectile,
                                            gamestate->landscape );
        }
        
        //if( distance_between_points(player->head->position) )
    }
}

void update_food( int delta )
{
    // If there's no food, don't update it
    if( gamestate->edible == NULL )
        return;
    
    // Apply gravity
    apply_gravity(gamestate->edible, delta);
    
    // Move the object
    apply_object_velocity( gamestate->edible, delta );
    
    // If it's below ground level, it has collided with the landscape
    if( gamestate->edible->position[1] <= 
            Landscape_getHeight(gamestate->landscape,
                                gamestate->edible->position[0],
                                gamestate->edible->position[2]) )
    {
        edible_landscape_collision( gamestate->edible, gamestate->landscape );
    }
}

void apply_gravity( Object* object, int delta )
{
    object->velocity[1] += GRAVITY * (delta / 1000.0f);
}

void apply_object_velocity( Object* object, int delta )
{
    object->position[0] += object->velocity[0] * delta / 1000.0f;
    object->position[1] += object->velocity[1] * delta / 1000.0f;
    object->position[2] += object->velocity[2] * delta / 1000.0f;
}

/**
 * Deforms the landscape at the given X/Z coordinates.
 * Note: takes real-space coordinates, not grid coordinates.
 */
void deform_landscape( float X, float Z, Landscape* landscape )
{
    // i should be the Row not the column, but it wasn't working right
    // useing the column is a hacky fix, 
    // it works provided that landscape->westBound == landscape->southBound
    int epicentre_j = Landscape_getColumn(landscape, Z);
    int epicentre_i = Landscape_getColumn(landscape, X);
    Point* epicentre;
    int i = 0, j = 0;
    int i_min = epicentre_i - DEFORMATION_RADIUS * landscape->gridDivisionWidth;
    int i_max = epicentre_i + DEFORMATION_RADIUS * landscape->gridDivisionWidth;
    int j_min = epicentre_j - DEFORMATION_RADIUS * landscape->gridDivisionWidth;
    int j_max = epicentre_j + DEFORMATION_RADIUS * landscape->gridDivisionWidth;
    Point* point;
    float dist;
    
    epicentre = Landscape_getPoint(landscape, epicentre_i, epicentre_j);

    for(i=i_min; i<i_max; i++){
        if( i < 0 || i > landscape->gridWidth )
            continue;
        for(j=j_min; j<j_max; j++){
            if( j < 0 || j > landscape->gridWidth )
                continue;
            point = Landscape_getPoint(landscape, i, j); 
            dist = distance_between_points(*point, *epicentre);
            // to make the square defined by the iterators into a circle
            if (dist <= DEFORMATION_RADIUS * landscape->gridDivisionWidth){
                (*point)[1] -= DEFORMATION_AMOUNT * landscape->gridDivisionWidth * cosf((DEFORMATION_RADIUS * landscape->gridDivisionWidth - dist)/DEFORMATION_RADIUS * landscape->gridDivisionWidth);
                // prevent it from deforming into a black hole
                if((*point)[1] < landscape->minHeight)
                    (*point)[1] = landscape->minHeight;
                // set the normal to the vector from the current point to the epicentre
                // not using vertex normals here because I can go one better: 
                // this is a special case for spheres
                gamestate->landscape->normalMap[i][j][0] = (*epicentre)[0] - (*point)[0];
                gamestate->landscape->normalMap[i][j][1] = (*epicentre)[1] - (*point)[1];
                gamestate->landscape->normalMap[i][j][2] = (*epicentre)[2] - (*point)[2];
                // change the colour to "burnt out"
                gamestate->landscape->colorMap[i][j][0] = 0.1f;
                gamestate->landscape->colorMap[i][j][1] = 0.15f;
                gamestate->landscape->colorMap[i][j][2] = 0.15f;
            }
        }
    }
}

void edible_landscape_collision( Edible* edible, Landscape* landscape )
{
    edible->position[1] =
        Landscape_getHeight( landscape,
                             edible->position[0],
                             edible->position[2] );
    
    edible->velocity[0] = 0;
    edible->velocity[1] = 0;
    edible->velocity[2] = 0;
}

void projectile_landscape_collision( Projectile* projectile,
                                     Landscape* landscape )
{
    // Deform the landscape
    deform_landscape( projectile->position[0],
                      projectile->position[2],
                      landscape );
    
    // Destroy the projectile
    if( projectile == gamestate->player1_projectile )
    {
        Projectile_delete( gamestate->player1_projectile );
        gamestate->player1_projectile = NULL;
    }
    else
    {
        Projectile_delete( gamestate->player2_projectile );
        gamestate->player2_projectile = NULL;
    }
}

int player_food_collision( Player* player )
{
    // increment the player's score
    player->score += POINTS_FOR_FOOD;
    // Destroy the edible
    Object_delete(gamestate->edible);
    // Create a new edible
    generate_edible();
    // Increase their length
    player->maxLength += FOOD_LENGTH_INCREMENT * gamestate->landscape->gridDivisionWidth;
    
    return 0;
}

/**
 * Make projectiles bounce back from the walls.
 */
void projectile_wall_collision( Projectile* projectile, Direction wall )
{
    // Reverse its direction
    // Set its position on the edge of the wall, or otherwise ensure that it's
    // within the bounds of the landscape
    
    // For now, destroy the projectile
    if( projectile == gamestate->player1_projectile )
    {
        gamestate->player1_projectile = NULL;
    }
    else
    {
        gamestate->player2_projectile = NULL;
    }
    
    Object_delete(projectile);
    
}

void print_score()
{
    printf( "Player 1 score: %d\n", gamestate->player1->score );
    printf( "Player 2 score: %d\n", gamestate->player2->score );
}
