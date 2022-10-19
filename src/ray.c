#include "ray.h"

#include "vec3.h"

void ray_assign( struct ray_t *r, const struct vec3_t *origin, const struct vec3_t *direction )
{
    vec3_assign( &r->origin, origin );
    vec3_assign( &r->direction, direction );
}

void ray_get_at( const struct ray_t *r, struct vec3_t *out, double t )
{
    vec3_times( out, &r->direction, t );
    vec3_add_assign( out, &r->origin );
}
