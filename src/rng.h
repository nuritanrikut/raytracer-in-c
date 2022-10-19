#ifndef RNG_H
#define RNG_H

struct vec3_t;
struct random_number_generator_t
{
    unsigned long state;
    unsigned long div;
    unsigned long mod;
};

void rng_create( struct random_number_generator_t *rng );
void rng_clone( struct random_number_generator_t *out, struct random_number_generator_t *in );

double rng_random_double( struct random_number_generator_t *rng );
double rng_random_range( struct random_number_generator_t *rng, double min, double max );

void rng_random_vec3( struct random_number_generator_t *rng, struct vec3_t *v );
void rng_random_vec3_range( struct random_number_generator_t *rng, struct vec3_t *v, double min, double max );

void rng_random_in_unit_sphere( struct random_number_generator_t *rng, struct vec3_t *v );
void rng_random_unit_vector( struct random_number_generator_t *rng, struct vec3_t *v );
void rng_random_in_unit_disk( struct random_number_generator_t *rng, struct vec3_t *v );

#endif
