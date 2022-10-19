#ifndef METAL_H
#define METAL_H

#include "material.h"
#include "vec3.h"

typedef struct metal_t
{
    struct material_t material;

    struct vec3_t albedo;
    double fuzz;
} metal_t;

void metal_create( struct metal_t *v, const struct vec3_t *albedo, double fuzz );
void metal_destroy( struct metal_t *v );

#endif
