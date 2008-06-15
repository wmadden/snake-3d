#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <list>

using std::list;

namespace patterns
{

template< class event_t >
class Subject;

template< class event_t >
class Observer
{
public:
	virtual void notify( Subject<event_t>*, event_t event ) = 0;
};

template< class event_t >
class Subject
{
public:
    void attach( Observer<event_t>* );
    void detach( Observer<event_t>* );
    
protected:
    void notify();

private:
    list< Observer<event_t>* > observers;
};

}

#endif /*OBSERVER_H_*/
