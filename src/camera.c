#include "camera.h"

#include "ray.h"
#include "rng.h"
#include "utils.h"
#include "vec3.h"

#include <math.h>

void camera_create( struct camera_t *camera,
                    const struct vec3_t *lookfrom,
                    const struct vec3_t *lookat,
                    const struct vec3_t *vup,
                    double vfov, // vertical field-of-view in degrees
                    double aspect_ratio,
                    double aperture,
                    double focus_dist )
{
    double theta = degrees_to_radians( vfov );
    double h = tan( theta / 2.0 );
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;
    struct vec3_t cross_v_w;
    struct vec3_t horizontal_half;
    struct vec3_t vertical_half;
    struct vec3_t focus_half;
    struct vec3_t look_direction;

    vec3_sub( &look_direction, lookfrom, lookat );
    vec3_unit_vector( &camera->w, &look_direction );

    vec3_cross( &cross_v_w, vup, &camera->w );
    vec3_unit_vector( &camera->u, &cross_v_w );

    vec3_cross( &camera->v, &camera->w, &camera->u );

    vec3_assign( &camera->origin, lookfrom );

    vec3_times( &camera->horizontal, &camera->u, focus_dist * viewport_width );
    vec3_div( &horizontal_half, &camera->horizontal, 2.0 );

    vec3_times( &camera->vertical, &camera->v, focus_dist * viewport_height );
    vec3_div( &vertical_half, &camera->vertical, 2.0 );

    vec3_times( &focus_half, &camera->w, focus_dist );

    vec3_assign( &camera->lower_left_corner, &camera->origin );
    vec3_sub_assign( &camera->lower_left_corner, &horizontal_half );
    vec3_sub_assign( &camera->lower_left_corner, &vertical_half );
    vec3_sub_assign( &camera->lower_left_corner, &focus_half );

    camera->lens_radius = aperture / 2;
}

void camera_get_ray( struct camera_t *camera, struct ray_t *out, struct random_number_generator_t *rng, double s, double t )
{
    struct vec3_t rd;
    struct vec3_t offset;
    struct vec3_t horizontal;
    struct vec3_t vertical;

    rng_random_in_unit_disk( rng, &rd );
    vec3_scale( &rd, camera->lens_radius );

    vec3_times( &horizontal, &camera->u, rd.x );
    vec3_times( &vertical, &camera->v, rd.y );
    vec3_add( &offset, &horizontal, &vertical );

    vec3_add( &out->origin, &camera->origin, &offset );

    vec3_times( &horizontal, &camera->horizontal, s );
    vec3_times( &vertical, &camera->vertical, t );
    vec3_assign( &out->direction, &camera->lower_left_corner );
    vec3_add_assign( &out->direction, &horizontal );
    vec3_add_assign( &out->direction, &vertical );
    vec3_sub_assign( &out->direction, &camera->origin );
    vec3_sub_assign( &out->direction, &offset );
}
