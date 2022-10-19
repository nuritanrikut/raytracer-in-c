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
#include <error.h>

void ray_color( struct vec3_t *out,
                int row,
                int col,
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
            ray_color( &recursed_color, row, col, &scattered, world, depth - 1, rng );
            vec3_mult( out, &attenuation, &recursed_color );

            // fprintf( stderr,
            //          "> Scatter %d %d dir=(%.4lf, %.4lf, %.4lf) attenuation=(%.4lf, %.4lf, %.4lf) "
            //          "recursed_color=(%.4lf, %.4lf, "
            //          "%.4lf) out=(%.4lf, %.4lf, %.4lf)\n",
            //          col,
            //          row,
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
        // fprintf( stderr, "> Diffuse %d %d = (%.4lf, %.4lf, %.4lf)\n", row, col, out->x, out->y, out->z );
        return;
    }

    vec3_unit_vector( &unit_direction, &r->direction );
    vec3_assign_v( &white, 1.0, 1.0, 1.0 );
    vec3_assign_v( &blue, 0.5, 0.7, 1.0 );
    t = 0.5 * ( unit_direction.y + 1.0 );
    vec3_scale( &white, 1.0 - t );
    vec3_scale( &blue, t );
    vec3_add( out, &white, &blue );

    // fprintf( stderr, "> Sky %d %d =(%.4lf, %.4lf, %.4lf)\n", row, col, out->x, out->y, out->z );
}

void simple_scene( struct hittable_list_t *world )
{
    struct lambertian_t *lambertian_material;
    struct vec3_t color;
    struct sphere_t *ground_sphere;
    struct dielectric_t *dielectric_material;
    struct lambertian_t *material2;
    struct vec3_t material2_albedo;
    struct metal_t *metal_material;
    struct vec3_t material3_albedo;
    struct sphere_t *object1;
    struct sphere_t *object2;
    struct sphere_t *object3;

    vec3_assign_v( &color, 0.5, 0.5, 0.5 );
    lambertian_material = (struct lambertian_t *)malloc( sizeof( struct lambertian_t ) );
    lambertian_create( lambertian_material, &color );
    ground_sphere = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( ground_sphere, 0.0, -1000.0, 0.0, 1000, (struct material_t *)lambertian_material );

    dielectric_material = (struct dielectric_t *)malloc( sizeof( struct dielectric_t ) );
    dielectric_create( dielectric_material, 1.5 );
    object1 = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( object1, 0, 1, 0, 1, (struct material_t *)dielectric_material );

    vec3_assign_v( &material2_albedo, 0.4, 0.2, 0.1 );
    material2 = (struct lambertian_t *)malloc( sizeof( struct lambertian_t ) );
    lambertian_create( material2, &material2_albedo );
    object2 = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( object2, -4, 1, 0, 1, (struct material_t *)material2 );

    vec3_assign_v( &material3_albedo, 0.7, 0.6, 0.5 );
    metal_material = (struct metal_t *)malloc( sizeof( struct metal_t ) );
    metal_create( metal_material, &material3_albedo, 0.0 );
    object3 = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( object3, 4, 1, 0, 1, (struct material_t *)metal_material );

    hittable_list_create( world );
    hittable_list_add( world, (struct hittable_t *)ground_sphere );
    hittable_list_add( world, (struct hittable_t *)object1 );
    hittable_list_add( world, (struct hittable_t *)object2 );
    hittable_list_add( world, (struct hittable_t *)object3 );
}

void random_scene( struct hittable_list_t *world, struct random_number_generator_t *rng )
{
    struct sphere_t *sphere_object;
    struct lambertian_t *lambertian_material;
    struct dielectric_t *dielectric_material;
    struct metal_t *metal_material;
    struct material_t *sphere_material;
    struct vec3_t color;
    struct vec3_t world_center;
    struct vec3_t sphere_center;
    struct vec3_t distance_to_world_center;
    double radius = 0.2;
    double choose_mat;
    int a;
    int b;

    hittable_list_create( world );

    vec3_assign_v( &world_center, 4, 0.2, 0 );

    vec3_assign_v( &color, 0.5, 0.5, 0.5 );
    lambertian_material = (struct lambertian_t *)malloc( sizeof( struct lambertian_t ) );
    lambertian_create( lambertian_material, &color );
    sphere_object = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( sphere_object, 0.0, -1000.0, 0.0, 1000, (struct material_t *)lambertian_material );
    hittable_list_add( world, (struct hittable_t *)sphere_object );

    for( a = -11; a < 11; a++ )
    {
        for( b = -11; b < 11; b++ )
        {
            vec3_assign_v(
                &sphere_center, a + 0.9 * rng_random_double( rng ), 0.2, b + 0.9 * rng_random_double( rng ) );
            vec3_sub( &distance_to_world_center, &sphere_center, &world_center );

            if( vec3_length( &distance_to_world_center ) > 0.9 )
            {
                choose_mat = rng_random_double( rng );
                if( choose_mat < 0.8 )
                {
                    // diffuse
                    rng_random_vec3( rng, &color );
                    lambertian_material = (struct lambertian_t *)malloc( sizeof( struct lambertian_t ) );
                    lambertian_create( lambertian_material, &color );
                    sphere_material = (struct material_t *)lambertian_material;
                }
                else if( choose_mat < 0.95 )
                {
                    // metal
                    double fuzz = rng_random_range( rng, 0, 0.5 );
                    rng_random_vec3_range( rng, &color, 0.5, 1 );
                    metal_material = (struct metal_t *)malloc( sizeof( struct metal_t ) );
                    metal_create( metal_material, &color, fuzz );
                    sphere_material = (struct material_t *)metal_material;
                }
                else
                {
                    // glass
                    dielectric_material = (struct dielectric_t *)malloc( sizeof( struct dielectric_t ) );
                    dielectric_create( dielectric_material, 1.5 );
                    sphere_material = (struct material_t *)dielectric_material;
                }

                sphere_object = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
                sphere_create(
                    sphere_object, sphere_center.x, sphere_center.y, sphere_center.z, radius, sphere_material );
                hittable_list_add( world, (struct hittable_t *)sphere_object );
            }
        }
    }

    dielectric_material = (struct dielectric_t *)malloc( sizeof( struct dielectric_t ) );
    dielectric_create( dielectric_material, 1.5 );
    sphere_object = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( sphere_object, 0, 1, 0, 1, (struct material_t *)dielectric_material );
    hittable_list_add( world, (struct hittable_t *)sphere_object );

    vec3_assign_v( &color, 0.4, 0.2, 0.1 );
    lambertian_material = (struct lambertian_t *)malloc( sizeof( struct lambertian_t ) );
    lambertian_create( lambertian_material, &color );
    sphere_object = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( sphere_object, -4, 1, 0, 1, (struct material_t *)lambertian_material );
    hittable_list_add( world, (struct hittable_t *)sphere_object );

    vec3_assign_v( &color, 0.7, 0.6, 0.5 );
    metal_material = (struct metal_t *)malloc( sizeof( struct metal_t ) );
    metal_create( metal_material, &color, 0.0 );
    sphere_object = (struct sphere_t *)malloc( sizeof( struct sphere_t ) );
    sphere_create( sphere_object, 4, 1, 0, 1, (struct material_t *)metal_material );
    hittable_list_add( world, (struct hittable_t *)sphere_object );
}

struct Job
{
    struct vec3_t *color;
    struct random_number_generator_t rng;
    int row;
    int col;
};

void render( struct Job *job,
             struct hittable_list_t *world,
             struct camera_t *cam,
             int image_width,
             int image_height,
             int samples_per_pixel_x,
             int samples_per_pixel_y,
             int max_depth )
{
    int samples_per_pixel = samples_per_pixel_x * samples_per_pixel_y;

    vec3_assign_v( job->color, 0, 0, 0 );

    for( int sample = 0; sample < samples_per_pixel; sample++ )
    {
        struct vec3_t sample_color;
        struct ray_t r;
        int sample_y = sample / samples_per_pixel_y;
        int sample_x = sample % samples_per_pixel_y;
        double y = (double)sample_y / samples_per_pixel_y - 0.5;
        double x = (double)sample_x / samples_per_pixel_x - 0.5;
        double u = ( job->col + x ) / ( image_width - 1 );
        double v = ( job->row + y ) / ( image_height - 1 );

        vec3_assign_v( &sample_color, 0.0, 0.0, 0.0 );
        camera_get_ray( cam, &r, &job->rng, u, v );
        ray_color( &sample_color, job->row, job->col, &r, world, max_depth, &job->rng );
        vec3_add_assign( job->color, &sample_color );
    }
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
    int samples_per_pixel = samples_per_pixel_x * samples_per_pixel_y;

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

    // Render
    struct vec3_t **pixels; // [row][col]
    struct Job *jobs;
    int job_count = image_height * image_width;
    int job_index = 0;
    int remaining_jobs;
    int remaining_lines;

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
    jobs = (struct Job *)malloc( job_count * sizeof( struct Job ) );
    if( !jobs )
    {
        error( 1, 0, "Cannot alloc jobs\n" );
    }
    memset( jobs, 0, job_count * sizeof( struct Job ) );
    pixels = (struct vec3_t **)malloc( image_height * sizeof( struct vec3_t * ) );
    if( !pixels )
    {
        error( 1, 0, "Cannot alloc pixels\n" );
    }
    job_index = 0;
    for( int row = 0; row < image_height; row++ )
    {
        pixels[row] = (struct vec3_t *)malloc( image_width * sizeof( struct vec3_t ) );
        if( !pixels[row] )
        {
            error( 1, 0, "Cannot alloc row\n" );
        }
        memset( pixels[row], 0, image_width * sizeof( struct vec3_t ) );
        for( int col = 0; col < image_width; col++ )
        {
            jobs[job_index].color = &pixels[row][col];
            jobs[job_index].row = row;
            jobs[job_index].col = col;
            rng_clone( &jobs[job_index].rng, &rng );
            job_index++;
        }
    }

    remaining_jobs = job_count;
    remaining_lines = image_height;
#pragma omp parallel for shared( remaining_jobs, remaining_lines )
    for( job_index = 0; job_index < job_count; job_index++ )
    {
        if( remaining_jobs % image_width == 0 )
        {
            remaining_lines--;
            fprintf( stderr, "Lines remaining: %d  \r", remaining_lines );
        }
        render( &jobs[job_index],
                &world,
                &cam,
                image_width,
                image_height,
                samples_per_pixel_x,
                samples_per_pixel_y,
                max_depth );
        remaining_jobs--;
    }
    fprintf( stderr, "Lines remaining: 0  \n" );
    fprintf( stderr, "Jobs finished\n" );
    fprintf( stderr, "Writing image\n" );

    printf( "P3\n%d %d\n255\n", image_width, image_height );
    for( int row = image_height - 1; row >= 0; row-- )
    {
        for( int col = 0; col < image_width; col++ )
        {
            vec3_write_color( stdout, &pixels[row][col], samples_per_pixel );
        }
    }

    fprintf( stderr, "\nDone.\n" );

    return EXIT_SUCCESS;
}
