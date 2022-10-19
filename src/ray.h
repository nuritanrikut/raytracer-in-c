#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct ray_t
{
    struct vec3_t origin;
    struct vec3_t direction;
};

void ray_assign( struct ray_t *r, const struct vec3_t *origin, const struct vec3_t *direction );
void ray_get_at( const struct ray_t *r, struct vec3_t *out, double t );

#endif
