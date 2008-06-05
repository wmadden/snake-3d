#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.h"

namespace domain
{

class Player : public domain::Entity
{
public:
	Player();
	virtual ~Player();
};

}

#endif /*PLAYER_H_*/
