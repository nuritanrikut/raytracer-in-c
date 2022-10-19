#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"
#include "rng.h"

struct camera_t
{
    struct vec3_t origin;
    struct vec3_t lower_left_corner;
    struct vec3_t horizontal;
    struct vec3_t vertical;
    struct vec3_t u;
    struct vec3_t v;
    struct vec3_t w;
    double lens_radius;
};

void camera_create( struct camera_t *c,
                    const struct vec3_t *lookfrom,
                    const struct vec3_t *lookat,
                    const struct vec3_t *vup,
                    double vfov, // vertical field-of-view in degrees
                    double aspect_ratio,
                    double aperture,
                    double focus_dist );
void camera_destroy( struct camera_t *c );

void camera_get_ray( struct camera_t *c, struct ray_t *out, struct random_number_generator_t *rng, double s, double t );

#endif
