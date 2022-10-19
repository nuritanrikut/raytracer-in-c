#include <math.h>

#include "vec3.h"
#include "camera.h"
#include "dielectric.h"
#include "hittable.h"
#include "hittable_list.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
#include "rng.h"
#include "sphere.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>

void ray_color( struct vec3_t *out,
                int i,
                int j,
                const struct ray_t *r,
                const struct hittable_list_t *world,
                int depth,
                struct random_number_generator_t *rng )
{
    struct hit_record_t rec;
    struct vec3_t unit_direction;
    struct vec3_t white;
    struct vec3_t blue;
    double t;

    if( depth <= 0 )
    {
        vec3_assign_v( out, 1.0, 1.0, 1.0 );
        return;
    }

    if( hittable_list_hit( world, r, 0.001, INFINITY, &rec ) )
    {
        struct ray_t scattered;
        struct vec3_t attenuation;

        vec3_assign_v( &attenuation, 0.0, 0.0, 0.0 );

        if( ( *rec.material.scatter )( rec.material.data, r, &rec, rng, &attenuation, &scattered ) )
        {
            struct vec3_t recursed_color;
            vec3_assign_v( &recursed_color, 0.0, 0.0, 0.0 );
            ray_color( &recursed_color, i, j, &scattered, world, depth - 1, rng );
            vec3_mult( out, &attenuation, &recursed_color );

            // fprintf( stderr,
            //          "> Scatter %d %d dir=(%.4lf, %.4lf, %.4lf) attenuation=(%.4lf, %.4lf, %.4lf) "
            //          "recursed_color=(%.4lf, %.4lf, "
            //          "%.4lf) out=(%.4lf, %.4lf, %.4lf)\n",
            //          i,
            //          j,
            //          r->direction.x,
            //          r->direction.y,
            //          r->direction.z,
            //          attenuation.x,
            //          attenuation.y,
            //          attenuation.z,
            //          recursed_color.x,
            //          recursed_color.y,
            //          recursed_color.z,
            //          out->x,
            //          out->y,
            //          out->z );
            return;
        }

        ( *rec.material.diffuse )( rec.material.data, out );
        // fprintf( stderr, "> Diffuse %d %d = (%.4lf, %.4lf, %.4lf)\n", i, j, out->x, out->y, out->z );
        return;
    }

    vec3_unit_vector( &unit_direction, &r->direction );
    vec3_assign_v( &white, 1.0, 1.0, 1.0 );
    vec3_assign_v( &blue, 0.5, 0.7, 1.0 );
    t = 0.5 * ( unit_direction.y + 1.0 );
    vec3_scale( &white, 1.0 - t );
    vec3_scale( &blue, t );
    vec3_add( out, &white, &blue );

    // fprintf( stderr, "> Sky %d %d =(%.4lf, %.4lf, %.4lf)\n", i, j, out->x, out->y, out->z );
}

void simple_scene( struct hittable_list_t *world )
{
    struct lambertian_t *ground_material;
    struct vec3_t ground_material_albedo;
    struct sphere_t *ground_sphere;
    struct dielectric_t *material1;
    struct lambertian_t *material2;
    struct vec3_t material2_albedo;
    struct metal_t *material3;
    struct vec3_t material3_albedo;
    struct sphere_t *object1;
    struct sphere_t *object2;
    struct sphere_t *object3;

    vec3_assign_v( &ground_material_albedo, 0.5, 0.5, 0.5 );
    ground_material = (struct lambertian_t *)malloc( sizeof( struct lambertian_t ) );
    lambertian_create( ground_material, &ground_material_albedo );
    ground_sphere = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( ground_sphere, 0.0, -1000.0, 0.0, 1000, (struct material_t *)ground_material );

    material1 = (struct dielectric_t *)malloc( sizeof( struct dielectric_t ) );
    dielectric_create( material1, 1.5 );
    object1 = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( object1, 0, 1, 0, 1, (struct material_t *)material1 );

    vec3_assign_v( &material2_albedo, 0.4, 0.2, 0.1 );
    material2 = (struct lambertian_t *)malloc( sizeof( struct lambertian_t ) );
    lambertian_create( material2, &material2_albedo );
    object2 = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( object2, -4, 1, 0, 1, (struct material_t *)material2 );

    vec3_assign_v( &material3_albedo, 0.7, 0.6, 0.5 );
    material3 = (struct metal_t *)malloc( sizeof( struct metal_t ) );
    metal_create( material3, &material3_albedo, 0.0 );
    object3 = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( object3, 4, 1, 0, 1, (struct material_t *)material3 );

    hittable_list_create( world );
    hittable_list_add( world, (struct hittable_t *)ground_sphere );
    hittable_list_add( world, (struct hittable_t *)object1 );
    hittable_list_add( world, (struct hittable_t *)object2 );
    hittable_list_add( world, (struct hittable_t *)object3 );
}

void random_scene( struct hittable_list_t *world, struct random_number_generator_t *rng )
{
    ;
}

int main( int argc, char **argv )
{
    // Image
    double aspect_ratio = 16.0 / 10.0;
    int image_width = 1920;
    int image_height = (int)( image_width / aspect_ratio );
    int samples_per_pixel_x = 16;
    int samples_per_pixel_y = 16;
    int max_depth = 50;

    // Camera
    struct vec3_t lookfrom;
    struct vec3_t lookat;
    struct vec3_t vup;
    double vfov = 20.0; // vertical field-of-view in degrees
    double dist_to_focus = 10.0;
    double aperture = 0.1;
    struct camera_t cam;

    // RNG
    struct random_number_generator_t rng;

    // World
    struct hittable_list_t world;

    //------

    // Camera
    vec3_assign_v( &lookfrom, 13.0, 2.0, 3.0 );
    vec3_assign_v( &lookat, 0.0, 0.0, 0.0 );
    vec3_assign_v( &vup, 0.0, 1.0, 0.0 );

    camera_create( &cam, &lookfrom, &lookat, &vup, vfov, aspect_ratio, aperture, dist_to_focus );

    fprintf( stderr,
             "Rendering %dx%d image with %dx%d samples per pixel\n",
             image_width,
             image_height,
             samples_per_pixel_x,
             samples_per_pixel_y );

    // RNG
    rng_create( &rng );
    rng_random_double( &rng );

    // World
    if( argc > 1 && strcmp( argv[1], "simple" ) != 0 )
    {
        fprintf( stderr, "Loading random scene\n" );
        random_scene( &world, &rng );
    }
    else
    {
        fprintf( stderr, "Loading simple scene\n" );
        simple_scene( &world );
    }

    // Render

    printf( "P3\n%d %d\n255\n", image_width, image_height );
    for( int j = image_height - 1; j >= 0; j-- )
    {
        fprintf( stderr, "Scanlines remaining: %d\n", j );
        for( int i = 0; i < image_width; i++ )
        {
            struct vec3_t pixel_color;
            int s = 0;
            vec3_assign_v( &pixel_color, 0.0, 0.0, 0.0 );
            // #pragma omp parallel for shared( pixel_color ) private( s )
            for( s = 0; s < samples_per_pixel_x * samples_per_pixel_y; ++s )
            {
                struct ray_t r;
                struct vec3_t color;
                int sample_y = s / samples_per_pixel_y;
                int sample_x = s % samples_per_pixel_y;
                double y = (double)sample_y / samples_per_pixel_y - 0.5;
                double x = (double)sample_x / samples_per_pixel_x - 0.5;
                double u = ( i + x ) / ( image_width - 1 );
                double v = ( j + y ) / ( image_height - 1 );
                vec3_assign_v( &color, 0, 0, 0 );
                camera_get_ray( &cam, &r, &rng, u, v );
                ray_color( &color, i, j, &r, &world, max_depth, &rng );
                vec3_add_assign( &pixel_color, &color );
            }

            vec3_write_color( stdout, &pixel_color, samples_per_pixel_x * samples_per_pixel_y );
        }
    }

    fprintf( stderr, "\nDone.\n" );

    return EXIT_SUCCESS;
}
