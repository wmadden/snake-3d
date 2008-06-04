#ifndef KEY_H_
#define KEY_H_

namespace input
{

class Key
{
public:
	Key( int id, bool special );
	virtual ~Key();
	
	
	
protected:
    int id;
    bool special;
};

}

#endif /*KEY_H_*/
