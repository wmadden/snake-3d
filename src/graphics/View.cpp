#include "View.h"

namespace graphics
{

list<View*> View::stack;

View::View()
{
    // Add this view to the stack
    stack.push_front(this);
}

View::~View()
{
    stack.pop_front();
}

View* View::getView()
{
    return stack.front();
}

void View::render()
{
    
}

}
