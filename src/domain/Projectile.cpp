#include "Projectile.h"

namespace domain
{

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

float Projectile::getBlastRadius()
{
    return blastRadius;
}

void Projectile::setBlastRadius( float radius )
{
    blastRadius = radius;
}

float Projectile::getDamage()
{
    return damage;
}

void Projectile::setDamage( float damage )
{
    this->damage = damage;
}


} // End of namespace `domain'
