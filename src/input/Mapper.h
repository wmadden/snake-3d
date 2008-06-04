#ifndef MAPPER_H_
#define MAPPER_H_

namespace input
{

class Mapper
{
public:
    static Mapper* getMapper();
	virtual ~Mapper();
	
	/*void map( Key key, Action action );*/

protected:
    
    Mapper();
    
};

}

#endif /*MAPPER_H_*/
