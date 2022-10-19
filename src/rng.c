#include "rng.h"

#include "vec3.h"

void rng_create( struct random_number_generator_t *rng )
{
    rng->state = 675248;
    rng->div = 1000;
    rng->mod = 1000000;
}

unsigned long next( struct random_number_generator_t *rng )
{
    rng->state = rng->state * rng->state / rng->div % rng->mod;
    return rng->state;
}

double rng_random_double( struct random_number_generator_t *rng )
{
    double t1 = next( rng );
    double t2 = next( rng );
    return ( t1 * rng->mod + t2 ) / rng->mod / rng->mod;
}

double rng_random_range( struct random_number_generator_t *rng, double min, double max )
{
    return min + ( max - min ) * rng_random_double( rng );
}

void rng_random_vec3_range( struct random_number_generator_t *rng, struct vec3_t *v, double min, double max )
{
    v->x = rng_random_range( rng, min, max );
    v->y = rng_random_range( rng, min, max );
    v->z = rng_random_range( rng, min, max );
}

void rng_random_in_unit_sphere( struct random_number_generator_t *rng, struct vec3_t *v )
{
    while( 1 )
    {
        rng_random_vec3_range( rng, v, -1.0, 1.0 );
        if( vec3_length_squared( v ) < 1.0 )
        {
            return;
        }
    }
}

void rng_random_unit_vector( struct random_number_generator_t *rng, struct vec3_t *v )
{
    struct vec3_t temp;
    rng_random_in_unit_sphere( rng, &temp );
    vec3_unit_vector( v, &temp );
}

void rng_random_in_unit_disk( struct random_number_generator_t *rng, struct vec3_t *v )
{
    while( 1 )
    {
        vec3_assign_v( v, rng_random_range( rng, -1.0, 1.0 ), rng_random_range( rng, -1.0, 1.0 ), 0.0 );
        if( vec3_length_squared( v ) < 1.0 )
        {
            return;
        }
    }
}
