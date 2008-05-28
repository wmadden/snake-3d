#ifndef CAMERA_H_
#define CAMERA_H_
/**
 * The purpose of this module is to abstract away camera operations.
 * 
 * This module provides the `Camera` object, which represents a camera in
 * OpenGL. This is not within the domain of the game, and so is not part of the
 * gamestate.
 */

typedef struct {
    // Position
    float position[3];
    // Forward vector
    float forward[3];
    // Up vector
    float up[3];
} Camera;

Camera* Camera_new();
void Camera_delete( Camera* camera );

#endif /*CAMERA_H_*/
