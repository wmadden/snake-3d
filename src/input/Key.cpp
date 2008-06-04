#include "Key.h"

namespace input
{

Key::Key( int id, bool special )
{
    this->id = id;
    this->special = special;
}

Key::~Key()
{
}

}
