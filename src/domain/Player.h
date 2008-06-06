#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.h"
#include "Body.h"
#include "Landscape.h"

namespace domain
{

class Player : public Entity
{
public:
    Player( Landscape* landscape, int starting_row, int starting_column,
            Direction initial_direction );
	virtual ~Player();
	
	void move( float distance );
	float getMaxLength();
	float getCurrentLength();
	void setMaxLength( float length );
	
	Body *getBody();
	Point getHeadPosition();
	Point getTailPosition();
	
	Point getForwardVector();
	Point getUpVector();
	
	void setDirection( Direction direction );
	
protected:
    Body *head, *tail;
    float headOffset, tailOffset;
    Point nextPoint, prevPoint;
    
    float maxLength, currentLength;
    
    Direction direction;
    
    Point forwardVector, upVector;
    
    Landscape* landscape;
};

} // End namespace `domain'

#endif /*PLAYER_H_*/
