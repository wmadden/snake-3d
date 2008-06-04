#ifndef VIEW_H_
#define VIEW_H_

#include <list>

using std::list;

namespace graphics
{

class View;

class View
{
public:
    static View* getView();
	virtual ~View();
	virtual void render();
	
protected:
    View();
    
    // View stack
    static list<View*> stack;
};

}

#endif /*VIEW_H_*/
