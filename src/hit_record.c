#include "hit_record.h"

#include "material.h"
#include "ray.h"
#include "vec3.h"

void hit_record_set_face_normal( struct hit_record_t *v, const struct ray_t *r, const struct vec3_t *outward_normal )
{
    v->front_face = vec3_dot( &r->direction, outward_normal ) < 0.0;
    if( v->front_face )
    {
        vec3_assign( &v->normal, outward_normal );
    }
    else
    {
        vec3_neg( &v->normal, outward_normal );
    }
}
