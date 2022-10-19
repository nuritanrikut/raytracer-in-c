#ifndef MATERIAL_H
#define MATERIAL_H

struct vec3_t;
struct ray_t;
struct hit_record_t;
struct random_number_generator_t;

struct material_t
{
    void *data;
    void ( *diffuse )( void *opaque, struct vec3_t *color );
    int ( *scatter )( void *opaque,
                      const struct ray_t *r_in,
                      const struct hit_record_t *rec,
                      struct random_number_generator_t *rng,
                      struct vec3_t *attenuation,
                      struct ray_t *scattered );
};

void material_assign( struct material_t *v, const struct material_t *a );

#endif
