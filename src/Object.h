#ifndef OBJECT_H_
#define OBJECT_H_

typedef struct {
    // The position of the object
    float position[3];
    // The velocity of the object (in distance units per second)
    float velocity[3];
    
    // The radius of the object
    float radius;
} Object;

// Define projectiles and food as Objects
typedef Object Projectile;
typedef Object Edible;

// Function for creating new objects
Object* Object_new( /* params */ );
void Object_delete( Object* object );

// Define functions for creating new food/projectiles
#define Projectile_new  Object_new
#define Edible_new      Object_new
// and functions for deleting them
#define Projectile_delete Object_delete
#define Edible_delete     Object_delete

#endif /*OBJECT_H_*/
