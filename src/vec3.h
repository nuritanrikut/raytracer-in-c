#ifndef VEC3_H
#define VEC3_H

#include <stdio.h>

struct vec3_t
{
    double x;
    double y;
    double z;
};

void vec3_print( FILE *fp, const struct vec3_t *v );
void vec3_write_color( FILE *fp, const struct vec3_t *color, int samples_per_pixel );

double vec3_length_squared( const struct vec3_t *v );
double vec3_length( const struct vec3_t *v );

void vec3_assign( struct vec3_t *v, const struct vec3_t *a );
void vec3_assign_v( struct vec3_t *v, double x, double y, double z );

void vec3_add( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *b );
void vec3_add_assign( struct vec3_t *v, const struct vec3_t *a );
void vec3_sub( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *b );
void vec3_sub_assign( struct vec3_t *v, const struct vec3_t *a );
void vec3_neg( struct vec3_t *v, const struct vec3_t *a );
void vec3_neg_assign( struct vec3_t *v );
void vec3_mult( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *b );
void vec3_scale( struct vec3_t *v, double t );
void vec3_times( struct vec3_t *v, const struct vec3_t *a, double t );
void vec3_div( struct vec3_t *v, const struct vec3_t *a, double t );
void vec3_div_assign( struct vec3_t *v, double t );

void vec3_unit_vector( struct vec3_t *v, const struct vec3_t *a );
int vec3_near_zero( const struct vec3_t *v );

double vec3_dot( const struct vec3_t *a, const struct vec3_t *b );
void vec3_cross( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *b );

void vec3_reflect( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *n );
void vec3_refract( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *n, double etai_over_etat );

#endif
