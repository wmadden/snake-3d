#ifndef PLAYER_H_
#define PLAYER_H_
/**
 * This module defines the Player - a very simple object representing a player.
 */

/**
 * This is the direction of travel of the player, relative to the landscape.
 */
typedef enum {
    DIRECTION_NORTH, DIRECTION_SOUTH, DIRECTION_EAST, DIRECTION_WEST
} Direction;

/**
 * This struct defines a segment of the body of a player.
 * 
 * Actually, this struct defines a joint in the player's body. The body itself
 * is a doubly linked list of joints.
 * 
 * The head of the player is another body segment
 */
typedef struct _Body {
    float* position; // Position vector
    int gridPosition[2]; // The grid point this joint occupies
    
    // The length of the segment, i.e. the distance to the previous point
    struct _Body* previous;
    struct _Body* next;
} Body;

/**
 * This structure describes a player.
 * 
 * In order to keep the rules governing player movement simple, players are only
 * allowed to move along grid lines. The main reason this makes things simpler
 * is that we have a height map of landscape points, from which we can infer the
 * heights of points on grid lines, but not the points between them (without
 * some complicated maths).
 * 
 * Keeping them on gridlines becomes slightly more complicated, however, since
 * it means we can't change the player's direction until they reach the next
 * grid point, even if they've told us to change direction. So we keep their
 * current direction, and the direction they want to travel next. When they
 * reach the next grid point, if they want to go a different direction, we
 * change their direction.
 * 
 * To describe the body of the player we have a linked list of body segments.
 * Since players can only move on grid lines, body segments can be described in
 * grid coordinates, since each joint will be on a grid point.
 * 
 * However, to describe the head and tail of a player - since they may be
 * between grid points - we need to store the distance they are from the next
 * grid point.
 * 
 * The `body` of a player will be a list of grid points - of a minimum length of
 * two. The head of a player at any time will be a certain distance from the
 * first element in this list (`headOffset`), and the tail of the player will be
 * a certain distance from the last element in the list (`tailOffset`).
 */
typedef struct {
    // The position of the player's head (in real coordinates)
    float headPosition[3];
    // The player's body, a doubly linked list of body segments.
    Body *head, *tail;
    // The position of the player's tail (in real coordinates)
    float tailPosition[3];
    
    /* The head and tail offsets give the position of the head and tail between
     * the first and second, and second-last and last points in the body,
     * respectively. */
    float headOffset;
    float tailOffset;
    
    // This determines which side of the landscape the player is on
    int underGround;
    
    // The player's colour (R, G, B)
    float color[3];
    
    // The player's score
    int score;
    
    /* The direction the player is travelling, relative to the landscape grid
     * and the direction they want to go in when they reach the next grid
     * point. */
    Direction currentDir, nextDir;
    
    // The player's forward vector
    float forward[3];
    // The player's up vector
    float up[3];
    
    // The player's maximum length
    float maxLength;
    // The player's actual length
    float length;
    // The player's radius
    float radius;
} Player;

/**
 * Creates and initializes a new player, and returns a pointer to them.
 */
Player* Player_new( /* params */ );
void Player_delete( Player* );
Body* Body_new( int row, int column, float x, float y, float z );
void Body_push( Body* new_segment, Body** body );
void Body_delete( Body* );
void Body_setPosition( Body* body, int row, int column,
                       float x, float y, float z );

#endif /*PLAYER_H_*/
