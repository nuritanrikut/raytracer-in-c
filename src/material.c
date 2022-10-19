#include "material.h"

void material_assign( struct material_t *v, const struct material_t *a )
{
    v->data = a->data;
    v->diffuse = a->diffuse;
    v->scatter = a->scatter;
}
