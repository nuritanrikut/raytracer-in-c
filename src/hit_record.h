#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "vec3.h"
#include "ray.h"
#include "material.h"

struct hit_record_t
{
    struct vec3_t p;
    struct vec3_t normal;
    struct material_t material;
    double t;
    int front_face;
};

void hit_record_set_face_normal( struct hit_record_t *v, const struct ray_t *r, const struct vec3_t *outward_normal );

#endif
