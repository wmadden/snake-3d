#include "Body.h"
#include <stdlib.h>

namespace domain
{

Body::Body()
{
    next = prev = NULL;
}

Body::~Body()
{
}

Body* Body::getNext()
{
    return next;
}

Body* Body::getPrev()
{
    return prev;
}


Body* Body::pushAhead( Body* body )
{
    if( next )
    {
        next->prev = body;
        body->next = next;
    }
    
    next = body;
    next->prev = this;
    
    return body;
}

Body* Body::pushBehind( Body* body )
{
    if( prev )
    {
        prev->next = body;
        body->prev = prev;
    }
    
    prev = body;
    prev->next = this;
    
    return body;
}

void Body::pop()
{
    if( next && prev )
    {
        next->prev = prev;
        prev->next = next;
    }
    
    next = prev = NULL;
}


} // End namespace `domain'
