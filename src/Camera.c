#include "Camera.h"
#include <stdlib.h>

Camera* Camera_new()
{
    return (Camera*)calloc( sizeof(Camera), 1 );
}

void Camera_delete( Camera* camera )
{
    free(camera);
}
