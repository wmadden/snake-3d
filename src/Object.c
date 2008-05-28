#include "Object.h"
#include <stdlib.h>

Object* Object_new( /* params */ )
{
    Object* object = (Object*)calloc( sizeof(Object), 1 );
    return object;
}

void Object_delete( Object* object )
{
    free(object);
}
