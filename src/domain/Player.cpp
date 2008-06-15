#include "Player.h"

namespace domain
{

Player::Player( Landscape* landscape, int starting_row, int starting_column,
                Direction initial_direction )
{
    this->landscape = landscape;
    
    direction = initial_direction;
    
    // TODO: set up player
}

Player::~Player()
{
}

void Player::move( float distance )
{
}

float Player::getMaxLength()
{
    return maxLength;
}

float Player::getCurrentLength()
{
    return currentLength;
}

void Player::setMaxLength( float length )
{
    this->maxLength = length;
}


Body* Player::getBody()
{
    return head;
}

Point Player::getHeadPosition()
{
    // TODO
}

Point Player::getTailPosition()
{
    // TODO
}

void Player::setDirection( Direction direction )
{
    this->direction = direction;
}


} // End namespace `domain'
