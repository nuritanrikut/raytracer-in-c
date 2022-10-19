#ifndef SPHERE_H
#define SPHERE_H

#include "material.h"
#include "hittable.h"
#include "vec3.h"

struct sphere_t
{
    struct hittable_t hittable;

    struct vec3_t center;
    double radius;
    struct material_t material;
};

void sphere_create( struct sphere_t *v,
                    double origin_x,
                    double origin_y,
                    double origin_z,
                    double radius,
                    const struct material_t *material );
void sphere_destroy( struct sphere_t *v );

#endif
