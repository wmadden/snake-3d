#include "render.h"
#include "Camera.h"
#include "Viewport.h"
#include "GameState.h"
#include "Player.h"
#include "mechanics.h"
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <stdio.h>
#include "maths.h"
#include <math.h>
#include "text.h"

/*******************************************************************************
 * TYPE DEFINITIONS
 ******************************************************************************/

/*******************************************************************************
 * GLOBALS AND CONSTANTS
 ******************************************************************************/
Viewport *player1_viewport, *player2_viewport, *minimap, *hud;

// The current frames per second
float fps = 0.0f;

// Do we prefer horizontal or vertical split screen?
//#define PREFER_VERTICAL

// Draw normals
//#define DRAW_NORMALS

// How close the camera sticks to the player (0.0 - 1.0)
#define CAMERA_TRACK_STRENGH 0.01

// The distance of the camera from the player, in grid divisions
#define CAMERA_DISTANCE 5
#define CAMERA_HEIGHT 2.0f
float camera_distance = 0;
float min_camera_height = 0.0f;

#define SKY_R 0.8515625f
#define SKY_G 0.91796875f
#define SKY_B 0.953125f

#define FOG_START 0.5
#define FOG_END 0.6

// The minimum number of milliseconds to wait before redrawing the scene
#define MIN_REDRAW_DELAY 0


/*******************************************************************************
 * INTERNAL FUNCTION PROTOTYPES
 ******************************************************************************/
void setup_cameras();
void update_cameras( int delta );
void set_up_GL();
void set_up_lighting();
void render_landscape( Landscape* landscape, float actual_width );
void render_player( Player* player, float scale );
void render_projectiles( Projectile* proj1, Projectile* proj2 );
void render_edibles( Edible* edible );
int should_render();
void render_viewport( Viewport* viewport, GameState* gamestate );
void render_hud( Viewport* viewport, GameState* gamestate );
void calc_fps();
void set_3_4_view( Camera* camera, float position[3], float forward[3], float up[3], int delta );
//void draw_segment( Point start, Point end, )

/*******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/
void render_init()
{
    GameState* gamestate = get_gamestate();
    // Create cameras for both players
    player1_viewport = Viewport_new( 0, 0, 800, 300 );
    player2_viewport = Viewport_new( 0, 300, 800, 300 );
    
    minimap = Viewport_new( 350, 250, 100, 100 );
    minimap->ortho = 1;
    minimap->bottom = gamestate->landscape->southBound;
    minimap->top = gamestate->landscape->northBound;
    minimap->left = gamestate->landscape->westBound;
    minimap->right = gamestate->landscape->eastBound;
    
    hud = Viewport_new( 0, 0, 800, 600 );
    hud->ortho = 1;
    hud->bottom = -1;
    hud->top = 1;
    hud->left = -1;
    hud->right = 1;
    
    // Set up the scene
    set_up_GL();
    
    // Set the cameras up
    setup_cameras();
    update_cameras(0);
}

void render()
{
    GameState* gamestate;
    
    if( !should_render() )
        return;
    
    // Get the gamestate
    gamestate = get_gamestate();
    
    // Set the default buffer colour to black
    glClearColor( SKY_R, SKY_G, SKY_B, 1.0f );
    
    // Clear the colour and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Draw player 1's viewport
    render_viewport( player1_viewport, gamestate );
    // Draw player 2's viewport
    render_viewport( player2_viewport, gamestate );
    
    // Turn off depth buffering for the minimap and HUD
    glDisable(GL_DEPTH_TEST);
    render_viewport( minimap, gamestate );
    render_hud( hud, gamestate );
    glEnable(GL_DEPTH_TEST);
    
    // Swap the buffers
    glutSwapBuffers();

    // Calculate the framerate
    calc_fps();
}

void window_resized( int width, int height )
{
    /* Adjust the viewports to match the new window dimensions */
    
    if(
#ifdef PREFER_VERTICAL
        width > height  // If equal, choose vertical
#else
        width <= height // If equal, choose horizontal
#endif
      )
    {
        // Horizontal split screen
        player1_viewport->x = player1_viewport->y = 0; // Player 1 on bottom
        player2_viewport->x = 0; player2_viewport->y = height / 2;
        
        player1_viewport->width = player2_viewport->width = width;
        player1_viewport->height = player2_viewport->height = height / 2;
        
        minimap->height = minimap->width = height / 6;
    }
    else
    {
        // Vertical split screen
        player1_viewport->x = width / 2; player1_viewport->y = 0;
        // Player 1 on right
        player2_viewport->x = player2_viewport->y = 0;
        
        player1_viewport->width = player2_viewport->width = width / 2;
        player1_viewport->height = player2_viewport->height = height;
        
        minimap->height = minimap->width = width / 6;
    }
    
    minimap->x = width / 2 - minimap->width / 2;
    minimap->y = height / 2 - minimap->width / 2;
    
    hud->x = hud->y = 0;
    hud->width = width; hud->height = height;
}

void render_update( int delta )
{
    update_cameras(delta);
    // Update anything else that changes with time
}

/*******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/
void setup_cameras()
{
    // Get the players
    GameState* gamestate = get_gamestate();
    Player *player1 = gamestate->player1,
           *player2 = gamestate->player2;
    
    // Set camera distance
    camera_distance = gamestate->landscape->gridDivisionDepth * CAMERA_DISTANCE;
    // Set the minimum camera height above the terrain
    min_camera_height = gamestate->player1->radius * CAMERA_HEIGHT;
    
    /* Set up the players' viewports */
    set_3_4_view( player1_viewport->camera,
                  player1->headPosition,
                  player1->forward,
                  player1->up,
                  1000000 );
    set_3_4_view( player2_viewport->camera,
                  player2->headPosition,
                  player2->forward,
                  player2->up,
                  1000000 );
    
    /* Set up the minimap camera */
    minimap->camera->position[0] = gamestate->landscape->westBound + 
                                   gamestate->landscape->worldWidth / 2.0f;
    minimap->camera->position[1] = gamestate->landscape->maxHeight * 10;
    minimap->camera->position[2] = gamestate->landscape->southBound +
                                   gamestate->landscape->worldDepth / 2.0f;
    minimap->camera->forward[0] = 0.0f;
    minimap->camera->forward[1] = 0.0f;
    minimap->camera->forward[2] = 0.0f;
    minimap->camera->up[0] = 0.0f;
    minimap->camera->up[1] = 0.0f;
    minimap->camera->up[2] = 1.0f;
    
    /* Set up the HUD camera */
    hud->camera->position[0] = 0.0f;
    hud->camera->position[1] = 0.0f;
    hud->camera->position[2] = 10.0f;
    hud->camera->forward[0] = 0.0f;
    hud->camera->forward[1] = 0.0f;
    hud->camera->forward[2] = 0.0f;
    hud->camera->up[0] = 0.0f;
    hud->camera->up[1] = 1.0f;
    hud->camera->up[2] = 0.0f;
}

/**
 * This function updates the players' camera positions.
 */
void update_cameras( int delta )
{
    // Get the players
    GameState* gamestate = get_gamestate();
    Player *player1 = gamestate->player1,
           *player2 = gamestate->player2;
    
    set_3_4_view( player1_viewport->camera,
                  player1->headPosition,
                  player1->forward,
                  player1->up,
                  delta );
    set_3_4_view( player2_viewport->camera,
                  player2->headPosition,
                  player2->forward,
                  player2->up,
                  delta );
}

void set_3_4_view( Camera* camera, float position[3], float forward[3], float up[3], int delta )
{
    float ideal_position[3], ideal_up[3], // The ideal values
          position_distance, up_distance, // The distance from ideals
          position_correction, up_correction, // The speed at which to correct
          position_movement, up_movement; // The amount of correction
    
    // Set the camera's forward vector
    camera->forward[0] = position[0];
    camera->forward[1] = position[1];
    camera->forward[2] = position[2];
    
    // Find the camera's ideal position
    ideal_position[0] = position[0] + (-0.05 * fabs(forward[0]) + 
                          1.5 * up[0]) * camera_distance;
    ideal_position[1] = position[1] + (-0.05 * fabs(forward[1]) + 
                          1.5 * up[1]) * camera_distance;
    ideal_position[2] = position[2] + (-0.05 * fabs(forward[2]) + 
                          1.5 * up[2]) * camera_distance;
    
    // Find the distance between the camera's current position and its ideal
    // position
    position_distance = distance_between_points( ideal_position, camera->position );
    up_distance = distance_between_points( ideal_up, camera->up );
    
    // As the distance between what it is and what it should be increases, so
    // too does the amount we move it to catch up.
    position_correction = position_distance * position_distance;
    up_correction = up_distance * up_distance;
    
    // Calculate the movement
    position_movement = position_correction * (delta / 1000.0f) + CAMERA_TRACK_STRENGH;
    up_movement = up_correction * (delta / 1000.0f) + CAMERA_TRACK_STRENGH;
    
    // Correct the camera position
    /*if( position_movement > position_distance )
    {*/
        camera->position[0] = ideal_position[0];
        camera->position[1] = ideal_position[1];
        camera->position[2] = ideal_position[2];
    /*}*/
    /*else if( position_movement != 0 )
    {
        camera->position[0] += position_movement *
                               (ideal_position[0] - camera->position[0]);
        camera->position[1] += position_movement *
                               (ideal_position[1] - camera->position[1]);
        camera->position[2] += position_movement *
                               (ideal_position[2] - camera->position[2]);
    }*/
    // Correct the camera up vector
    /*if( up_movement != 0 )
    {
        camera->up[0] += up_movement *
                               (ideal_up[0] - camera->up[0]);
        camera->up[1] += up_movement *
                               (ideal_up[1] - camera->up[1]);
        camera->up[2] += up_movement *
                               (ideal_up[2] - camera->up[2]);
    }*/
    camera->up[0] = 0;//ideal_up[0];
    camera->up[1] = 1;//ideal_up[1];
    camera->up[2] = 0;//ideal_up[2];

	// Ensure that camera is above terrain
	Landscape* landscape = get_gamestate()->landscape;
	float terrain_height =
		Landscape_getHeight( landscape,
							 camera->position[0],
							 camera->position[2] );
	if( camera->position[1] < terrain_height + min_camera_height )
	{
		camera->position[1] = terrain_height + min_camera_height;
	}
}

/**
 * Implements very simple framerate limiting.
 * 
 * Only redraw the scene if at least MIN_REDRAW_DELAY milliseconds have passed.
 */
int should_render()
{
    static int last_render = 0;
    int now;
    
    now = glutGet(GLUT_ELAPSED_TIME);
    
    if( now - last_render >= MIN_REDRAW_DELAY )
    {
        last_render = now;
        return 1;
    }
    
    return 0;
}

/**
 * Sets up the scene.
 * 
 * Enables useful OpenGL features (like depth testing and lighting), and sets up
 * the scene lighting.
 */
void set_up_GL()
{
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Set the default buffer colour to black
    glClearColor( SKY_R, SKY_G, SKY_B, 0.0f );
    
    // Draw the backs of polygons in wireframe mode
    glPolygonMode(GL_BACK, GL_LINE);
    
    // Automatically adjust normals so they're unit vectors
    glEnable(GL_NORMALIZE);
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    
    // Enable colour materials
    glEnable(GL_COLOR_MATERIAL);
    
}

/**
 * Sets up lighting.
 * 
 * Remember that lights are affected by the current model/view matrix.
 */
void set_up_lighting()
{
    GameState* gamestate = get_gamestate();
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glScalef( gamestate->landscape->worldWidth,
              gamestate->landscape->worldWidth,
              gamestate->landscape->worldWidth );
    
    // Create a light
    GLfloat light_color[]    = { 1.0f, 1.0f, 1.0f, 1.0f },
            light_position[3],
            ambient          = 0.2f,
            diffuse          = 0.9,
            specular         = 0.4f;
    
    light_position[0] = light_position[2] = 0;
    light_position[1] = gamestate->landscape->maxHeight;
    
    GLfloat ambient_light[] = { ambient * light_color[0],
                                ambient * light_color[1],
                                ambient * light_color[2],
                                1.0f };
    GLfloat diffuse_light[] = { diffuse * light_color[0],
                                diffuse * light_color[1],
                                diffuse * light_color[2],
                                1.0f };
    GLfloat specular_light[] = { specular * light_color[0],
                                 specular * light_color[1],
                                 specular * light_color[2],
                                 1.0f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable(GL_LIGHT0);
    
    glPopMatrix();
}

/**
 * Renders the given landscape.
 */
void render_landscape( Landscape* landscape, float actual_width )
{
    // The column and row numbers give the location in the height map.
    // X and Z values are inferred from our position in the array.
    // Columns map to X values, rows map to Z values.
    int column, row;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    // Scale
    //glScalef( actual_width, actual_width / 4.0f, actual_width );
    float ambient = 0.0,
          diffuse = 0.0,
          specular = 1.0, specular_focus = 60;
    GLfloat ambient_colour[] = { ambient, ambient, ambient, 1.0f };
    GLfloat diffuse_colour[] = { diffuse, diffuse, diffuse, 1.0f };
    GLfloat specular_colour[] = { specular, specular, specular, 0.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_colour);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_colour);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_colour);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, specular_focus);
    
    // Set up lighting in the context of the landscape
    set_up_lighting();

    for( column = 0; column < landscape->gridWidth - 1; column++ )
    {
        glBegin(GL_QUAD_STRIP);
        for( row = 0; row < landscape->gridWidth; row++ )
        {
            // Left point
            glColor3fv( landscape->colorMap[row][column] );
            glNormal3fv( landscape->normalMap[row][column] );
            glVertex3fv( landscape->pointMap[row][column] );
            
            // Right point
            glColor3fv( landscape->colorMap[row][column + 1] );
            glNormal3fv( landscape->normalMap[row][column + 1] );
            glVertex3fv( landscape->pointMap[row][column + 1] );
        }
        glEnd();
        
#ifdef DRAW_NORMALS
        glBegin(GL_LINES);
        for( row = 0; row < landscape->gridWidth; row++ )
        {
            // Left point
            glColor3f( 1.0f, 1.0f, 1.0f );
            glVertex3fv( landscape->pointMap[row][column] );
            glVertex3fv( landscape->normalMap[row][column] );
            
            // Right point
            glVertex3fv( landscape->pointMap[row][column + 1] );
            glVertex3fv( landscape->normalMap[row][column + 1] );
        }
        glEnd();
#endif
    }
    
    glPopMatrix();
}


/**
 * Renders a player.
 */
void render_player( Player* player, float scale )
{
    Body* segment = player->head->next;
    
    // Push the current modelview matrix
    glMatrixMode(GL_MODELVIEW);
    
    // Draw in the colour of the player
    glColor3fv( player->color );
    
    // Draw the head (in white for debugging)
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPushMatrix();
        // Translate
        glTranslatef( player->headPosition[0],
                      player->headPosition[1],
                      player->headPosition[2] );
        
        // Draw forward and up vectors
        glBegin(GL_LINES);
        glVertex3fv( player->headPosition );
        glVertex3fv( player->forward );
        glVertex3fv( player->headPosition );
        glVertex3fv( player->up );
        glEnd();
        
        glutSolidCube( 1.0f * scale );
    glPopMatrix();
    
    // Draw the body
    while( segment != NULL && segment->next != NULL )
    {
        glPushMatrix();
            // Translate
            glTranslatef( segment->position[0],
                          segment->position[1],
                          segment->position[2] );
            
            glColor3fv( player->color );
            
            glutSolidCube(1.0f * scale);
        glPopMatrix();
        
        segment = segment->next;
    }
    
    // Draw the tail (in white for debugging)
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPushMatrix();
        // Translate
        glTranslatef( player->tailPosition[0],
                      player->tailPosition[1],
                      player->tailPosition[2] );
        
        glutSolidCube( 1.0f * scale );
    glPopMatrix();
}

void render_projectiles( Projectile* proj1, Projectile* proj2 )
{
    glMatrixMode(GL_MODELVIEW);
    
    // Draw projectiles in white
    glColor3f( 1.0f, 1.0f, 1.0f );
    
    if( proj1 != NULL )
    {
        glPushMatrix();
            // Move to the object's location
            glTranslatef( proj1->position[0],
                          proj1->position[1],
                          proj1->position[2] );
            
            glutSolidSphere(proj1->radius, 4, 4);
        
        glPopMatrix();
    }
    if( proj2 != NULL )
    {
        glPushMatrix();
            // Move to the object's location
            glTranslatef( proj2->position[0],
                          proj2->position[1],
                          proj2->position[2] );
            
            glutSolidSphere(proj2->radius, 4, 4);
        
        glPopMatrix();
    }
}

void render_edible( Edible* edible )
{
    if( edible == NULL )
        return;
    
    // Draw edibles in red
    glColor3f( 1.0f, 0.0f, 0.0f );
    glPushMatrix();
        // Move to the object's location
        glTranslatef( edible->position[0],
                      edible->position[1],
                      edible->position[2] );
        
        // glutSolidSphere( edible->radius, 4, 4 );
        glPolygonMode(GL_BACK, GL_FILL);
        glutSolidTeapot(edible->radius);
        glPolygonMode(GL_BACK, GL_LINE);
    
    glPopMatrix();
}

void render_viewport( Viewport* viewport, GameState* gamestate )
{
    // Reload the identity matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Apply the viewport to the current OpenGL context (and its camera's
    // perspective)
    Viewport_apply(viewport);
    
    // Render the landscape
    render_landscape( gamestate->landscape, gamestate->landscape->worldWidth );
    // Render the players
    render_player( gamestate->player1, gamestate->landscape->gridDivisionWidth );
    render_player( gamestate->player2, gamestate->landscape->gridDivisionWidth );
    // Render the objects
    render_projectiles( gamestate->player1_projectile,
                        gamestate->player2_projectile );
    render_edible( gamestate->edible );
}

/**
 * This function calculates the current frames per second.
 * It should be called every time a frame is drawn.
 */
void calc_fps()
{
    static float frames = 0;
    static float time = 0;
    static int last_time = -1;
    
    int cur_time = glutGet(GLUT_ELAPSED_TIME);
    
    if( last_time == -1 )
    {
        last_time = cur_time;
        return;
    }
    
    int diff = cur_time - last_time;
    
    time += diff;
    frames++;
    
    last_time = cur_time;
    
    float result = frames / time * 1000.0f;
    
    if( time >= 1000.0f )
    {
        time = 0;
        frames = 0;
        // Update the FPS
        fps = result;
    }
}

/**
 * Renders the HUD on the given viewport.
 */
void render_hud( Viewport* hud, GameState* gamestate )
{
    char fps_string[15];
    // Reload the identity matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    Viewport_apply(hud);
    
    
    // Draw the FPS
    sprintf( fps_string, "FPS: %3.1f", fps );
    
    glDisable(GL_LIGHTING);
    
    glLineWidth(2.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    draw_2D_text( fps_string, 0, 590, 10, hud );
    glLineWidth(0.8f);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    draw_2D_text( fps_string, 0, 590, 10, hud );
    
    glEnable(GL_LIGHTING);
}

