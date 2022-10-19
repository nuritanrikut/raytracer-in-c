#include "sphere.h"

#include "hit_record.h"

#include <math.h>

int sphere_hit( void *opaque, const struct ray_t *r, double t_min, double t_max, struct hit_record_t *rec )
{
    struct sphere_t *v = (struct sphere_t *)opaque;
    struct vec3_t oc;
    struct vec3_t outward_normal;
    double a;
    double half_b;
    double c;
    double discriminant;
    double sqrtd;
    double t;

    vec3_sub( &oc, &r->origin, &v->center );
    a = vec3_length_squared( &r->direction );
    half_b = vec3_dot( &oc, &r->direction );
    c = vec3_length_squared( &oc ) - v->radius * v->radius;

    discriminant = half_b * half_b - a * c;
    if( discriminant < 0 )
    {
        return 0;
    }
    sqrtd = sqrt( discriminant );

    // Find the nearest root that lies in the acceptable range.
    t = ( -half_b - sqrtd ) / a;
    if( t < t_min || t_max < t )
    {
        t = ( -half_b + sqrtd ) / a;
        if( t < t_min || t_max < t )
        {
            return 0;
        }
    }

    rec->t = t;
    ray_get_at( r, &rec->p, rec->t );
    vec3_sub( &outward_normal, &rec->p, &v->center );
    vec3_div_assign( &outward_normal, v->radius );

    hit_record_set_face_normal( rec, r, &outward_normal );
    material_assign( &rec->material, &v->material );

    return 1;
}

void sphere_create( struct sphere_t *v,
                    double origin_x,
                    double origin_y,
                    double origin_z,
                    double radius,
                    const struct material_t *material )
{
    v->hittable.opaque = v;
    v->hittable.hit = sphere_hit;
    vec3_assign_v( &v->center, origin_x, origin_y, origin_z );
    v->radius = radius;
    material_assign( &v->material, material );
}
