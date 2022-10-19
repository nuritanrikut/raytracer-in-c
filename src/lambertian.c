#include "lambertian.h"

#include "hit_record.h"
#include "ray.h"
#include "rng.h"

void lambertian_diffuse( void *opaque, struct vec3_t *out )
{
    struct lambertian_t *v = (struct lambertian_t *)opaque;
    vec3_assign( out, &v->albedo );
}

int lambertian_scatter( void *opaque,
                        const struct ray_t *r_in,
                        const struct hit_record_t *rec,
                        struct random_number_generator_t *rng,
                        struct vec3_t *attenuation,
                        struct ray_t *scattered )
{
    struct vec3_t scatter_direction;
    struct lambertian_t *v = (struct lambertian_t *)opaque;

    rng_random_unit_vector( rng, &scatter_direction );
    vec3_add_assign( &scatter_direction, &rec->normal );

    if( vec3_near_zero( &scatter_direction ) )
    {
        vec3_assign( &scatter_direction, &rec->normal );
    }

    ray_assign( scattered, &rec->p, &scatter_direction );
    vec3_assign( attenuation, &v->albedo );
    return 1;
}

void lambertian_create( struct lambertian_t *v, const struct vec3_t *albedo )
{
    v->material.data = v;
    v->material.diffuse = lambertian_diffuse;
    v->material.scatter = lambertian_scatter;
    vec3_assign( &v->albedo, albedo );
}
