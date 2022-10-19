#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "ray.h"

#include "hittable.h"

struct hittable_list_t
{
    struct hittable_t **objects;
    unsigned int size;
    unsigned int capacity;
};

void hittable_list_create( struct hittable_list_t *v );
void hittable_list_destroy( struct hittable_list_t *v );
void hittable_list_add( struct hittable_list_t *v, struct hittable_t *obj );
void hittable_list_clear( struct hittable_list_t *v );
int hittable_list_hit( const struct hittable_list_t *v,
                       const struct ray_t *r,
                       double t_min,
                       double t_max,
                       struct hit_record_t *rec );

#endif
