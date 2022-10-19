#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"
#include "vec3.h"

struct lambertian_t
{
    struct material_t material;

    struct vec3_t albedo;
};

void lambertian_create( struct lambertian_t *v, const struct vec3_t *albedo );
void lambertian_destroy( struct lambertian_t *v );

#endif
