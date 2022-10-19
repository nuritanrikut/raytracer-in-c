#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "hit_record.h"

struct hittable_t
{
    void *opaque;
    int ( *hit )( void *opaque, const struct ray_t *r, double t_min, double t_max, struct hit_record_t *rec );
};

#endif
