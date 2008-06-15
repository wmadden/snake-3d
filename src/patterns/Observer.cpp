#include "Observer.h"

namespace patterns
{

void Subject::attach( Observer* observer )
{
    observers.push_back(observer);
}

void Subject::detach( Observer* observer )
{
    for( list<Observer*>::iterator it = observers.begin();
         it != observers.end();
         it++ )
    {
        if( *it == observer )
        {
            observers.erase(it);
        }
    }
}

void Subject::notify()
{
    for( list<Observer*>::iterator it = observers.begin();
         it != observers.end();
         it++ )
    {
        (*it)->notify();
    }
}


}
