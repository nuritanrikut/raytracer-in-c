#include "vec3.h"

#include "utils.h"

#include <math.h>

void vec3_write_color( FILE *fp, const struct vec3_t *color, int samples_per_pixel )
{
    double scale = 1.0 / samples_per_pixel;

    double r = sqrt( scale * color->x );
    double g = sqrt( scale * color->y );
    double b = sqrt( scale * color->z );

    int ri = 256 * clamp( r, 0.0, 0.999 );
    int gi = 256 * clamp( g, 0.0, 0.999 );
    int bi = 256 * clamp( b, 0.0, 0.999 );

    fprintf( fp, "%d %d %d\n", ri, gi, bi );
}

double vec3_length_squared( const struct vec3_t *v )
{
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

double vec3_length( const struct vec3_t *v )
{
    return sqrt( vec3_length_squared( v ) );
}

void vec3_assign( struct vec3_t *v, const struct vec3_t *a )
{
    v->x = a->x;
    v->y = a->y;
    v->z = a->z;
}

void vec3_assign_v( struct vec3_t *v, double x, double y, double z )
{
    v->x = x;
    v->y = y;
    v->z = z;
}

void vec3_add( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *b )
{
    v->x = a->x + b->x;
    v->y = a->y + b->y;
    v->z = a->z + b->z;
}

void vec3_add_assign( struct vec3_t *v, const struct vec3_t *a )
{
    v->x += a->x;
    v->y += a->y;
    v->z += a->z;
}

void vec3_sub( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *b )
{
    v->x = a->x - b->x;
    v->y = a->y - b->y;
    v->z = a->z - b->z;
}

void vec3_sub_assign( struct vec3_t *v, const struct vec3_t *a )
{
    v->x -= a->x;
    v->y -= a->y;
    v->z -= a->z;
}

void vec3_neg( struct vec3_t *v, const struct vec3_t *a )
{
    v->x = -a->x;
    v->y = -a->y;
    v->z = -a->z;
}

void vec3_neg_assign( struct vec3_t *v )
{
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;
}

void vec3_mult( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *b )
{
    v->x = a->x * b->x;
    v->y = a->y * b->y;
    v->z = a->z * b->z;
}

void vec3_scale( struct vec3_t *v, double t )
{
    v->x *= t;
    v->y *= t;
    v->z *= t;
}

void vec3_times( struct vec3_t *v, const struct vec3_t *a, double t )
{
    v->x = a->x * t;
    v->y = a->y * t;
    v->z = a->z * t;
}

void vec3_div( struct vec3_t *v, const struct vec3_t *a, double t )
{
    v->x = a->x / t;
    v->y = a->y / t;
    v->z = a->z / t;
}

void vec3_div_assign( struct vec3_t *v, double t )
{
    v->x /= t;
    v->y /= t;
    v->z /= t;
}

void vec3_unit_vector( struct vec3_t *v, const struct vec3_t *a )
{
    double scale = vec3_length( a );
    vec3_div( v, a, scale );
}

int vec3_near_zero( const struct vec3_t *v )
{
    const double s = 1e-8;
    return ( fabs( v->x ) < s ) && ( fabs( v->y ) < s ) && ( fabs( v->z ) < s );
}

double vec3_dot( const struct vec3_t *a, const struct vec3_t *b )
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vec3_cross( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *b )
{
    v->x = a->y * b->z - a->z * b->y;
    v->y = a->z * b->x - a->x * b->z;
    v->z = a->x * b->y - a->y * b->x;
}

void vec3_reflect( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *n )
{
    struct vec3_t temp;
    vec3_times( &temp, n, 2.0 * vec3_dot( a, n ) );
    vec3_sub( v, a, &temp );
}

void vec3_refract( struct vec3_t *v, const struct vec3_t *a, const struct vec3_t *n, double etai_over_etat )
{
    struct vec3_t temp;
    double cos_theta;
    struct vec3_t r_out_perp;
    struct vec3_t r_out_parallel;

    // C++: const auto cos_theta = fmin( dot( -a, n ), 1.0 );
    vec3_neg( &temp, a );
    cos_theta = fmin( vec3_dot( &temp, n ), 1.0 );

    // C++: const vec3_t r_out_perp = etai_over_etat * ( a + cos_theta * n );
    vec3_times( &temp, n, cos_theta );
    vec3_add_assign( &temp, a );
    vec3_times( &r_out_perp, &temp, etai_over_etat );

    // C++: const vec3_t r_out_parallel = -sqrt( fabs( 1.0 - length_squared( r_out_perp ) ) ) * n;
    vec3_times( &r_out_parallel, n, -sqrt( fabs( 1.0 - vec3_length_squared( &r_out_perp ) ) ) );

    // C++: return r_out_perp + r_out_parallel;
    vec3_add( v, &r_out_perp, &r_out_parallel );
}
