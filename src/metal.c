#include "metal.h"

#include "hit_record.h"
#include "ray.h"
#include "rng.h"

void metal_diffuse( void *opaque, struct vec3_t *out )
{
    struct metal_t *v = (struct metal_t *)opaque;
    vec3_assign( out, &v->albedo );
}

int metal_scatter( void *opaque,
                   const struct ray_t *r_in,
                   const struct hit_record_t *rec,
                   struct random_number_generator_t *rng,
                   struct vec3_t *attenuation,
                   struct ray_t *scattered )
{
    struct vec3_t reflected;
    struct vec3_t unit_vector;
    struct vec3_t direction;
    struct metal_t *v = (struct metal_t *)opaque;

    vec3_unit_vector( &unit_vector, &r_in->direction );
    vec3_reflect( &reflected, &unit_vector, &rec->normal );
    rng_random_in_unit_sphere( rng, &direction );
    vec3_scale( &direction, v->fuzz );
    vec3_add_assign( &direction, &reflected );
    ray_assign( scattered, &rec->p, &direction );
    vec3_assign( attenuation, &v->albedo );
    return ( vec3_dot( &scattered->direction, &rec->normal ) > 0.0 );
}

void metal_create( struct metal_t *v, const struct vec3_t *albedo, double fuzz )
{
    v->material.data = v;
    v->material.diffuse = metal_diffuse;
    v->material.scatter = metal_scatter;
    vec3_assign( &v->albedo, albedo );
    v->fuzz = fuzz < 1.0 ? fuzz : 1.0;
}
