#include "Edible.h"

namespace domain
{

Edible::Edible()
{
}

Edible::~Edible()
{
}

float Edible::getValue()
{
    return value;
}

void Edible::setValue( float value )
{
    this->value = value;
}


} // End namespace `domain'
