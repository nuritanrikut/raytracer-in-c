#include "hittable_list.h"

#include <error.h>
#include <stdlib.h>

void hittable_list_create( struct hittable_list_t *v )
{
    v->objects = (struct hittable_t **)malloc( sizeof( struct hittable_t * ) );
    v->size = 0;
    v->capacity = 1;
}

void hittable_list_destroy( struct hittable_list_t *v )
{
    v->capacity = 0;
    v->size = 0;
    free( v->objects );
}

void hittable_list_add( struct hittable_list_t *v, struct hittable_t *obj )
{
    if( v->size == v->capacity )
    {
        v->objects = (struct hittable_t **)realloc( v->objects, ( v->capacity + 1 ) * sizeof( struct hittable_t * ) );
        if( !v->objects )
        {
            hittable_list_destroy( v );
            error( 1, 0, "Cannot realloc\n" );
        }
        v->capacity++;
    }

    v->objects[v->size] = obj;
    v->size++;
}

int hittable_list_hit( const struct hittable_list_t *v,
                       const struct ray_t *r,
                       double t_min,
                       double t_max,
                       struct hit_record_t *rec )
{
    int hit_anything = 0;
    double closest_so_far = t_max;
    int i = 0;

    for( i = 0; i < v->size; i++ )
    {
        struct hittable_t *obj = NULL;
        obj = v->objects[i];
        if( obj->hit( obj->opaque, r, t_min, closest_so_far, rec ) )
        {
            hit_anything = 1;
            closest_so_far = rec->t;
        }
    }

    return hit_anything;
}
