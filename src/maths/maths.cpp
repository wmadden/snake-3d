#include "maths.h"
#include <stdlib.h>

namespace maths
{

float random( float min, float max )
{
    return min + (max - min) * (rand() / RAND_MAX);
}

int random( int min, int max )
{
    return min + (max - min) * (rand() / RAND_MAX);
}

} // End of namespace `maths'
