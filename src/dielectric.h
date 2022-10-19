#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"

struct dielectric_t
{
    struct material_t material;

    double ir; // Index of Refraction
};

void dielectric_create( struct dielectric_t *v, double ir );
void dielectric_destroy( struct dielectric_t *v );

#endif
