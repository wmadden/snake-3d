#ifndef INPUT_H_
#define INPUT_H_
/**
 * This module is responsible for handling input.
 * 
 * The module is event driven, with the various handler functions called by GLUT
 * when an input event takes place. These handler functions then invoke
 * functions in the mechanics module, which affect the gamestate.
 * 
 * More generally, this module is used to map keystrokes to player actions.
 */

/**
 * Initializes the input module.
 * 
 * Requires the identifier for the window from which input is captured.
 */
void input_init( int window_id );

#endif /*INPUT_H_*/
