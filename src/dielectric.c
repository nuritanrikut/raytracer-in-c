#include "dielectric.h"

#include "hit_record.h"
#include "material.h"
#include "ray.h"
#include "rng.h"

#include <math.h>

double reflectance( double cosine, double ref_idx )
{
    // Use Schlick's approximation for reflectance.
    double r0 = ( 1.0 - ref_idx ) / ( 1.0 + ref_idx );
    r0 = r0 * r0;
    return r0 + ( 1.0 - r0 ) * pow( ( 1.0 - cosine ), 5.0 );
}

void dielectric_diffuse( void *opaque, struct vec3_t *out )
{
    vec3_assign_v( out, 1.0, 1.0, 1.0 );
}

int dielectric_scatter( void *opaque,
                        const struct ray_t *r_in,
                        const struct hit_record_t *rec,
                        struct random_number_generator_t *rng,
                        struct vec3_t *attenuation,
                        struct ray_t *scattered )
{
    struct vec3_t direction;
    struct vec3_t unit_direction;
    double refraction_ratio;
    double d;
    double cos_theta;
    double sin_theta;
    double t;
    double refl;
    int cannot_refract = 0;
    int should_reflect = 0;
    struct dielectric_t *v = (struct dielectric_t *)opaque;

    vec3_assign_v( attenuation, 1.0, 1.0, 1.0 );

    refraction_ratio = rec->front_face ? ( 1.0 / v->ir ) : v->ir;

    vec3_unit_vector( &unit_direction, &r_in->direction );
    vec3_neg_assign( &unit_direction );
    d = vec3_dot( &unit_direction, &rec->normal );
    cos_theta = fmin( d, 1.0 );
    sin_theta = sqrt( 1.0 - cos_theta * cos_theta );

    cannot_refract = refraction_ratio * sin_theta > 1.0;
    t = rng_random_double( rng );
    refl = reflectance( cos_theta, refraction_ratio );
    should_reflect = refl > t;

    // fprintf( stderr,
    //          "> reflectance unit_direction( %+.4lf, %+.4lf, %+.4lf ) rec.normal( %+.4lf, %+.4lf, %+.4lf ) d %+.4lf "
    //          "cos_theta "
    //          "%+.4lf refraction_ratio %+.4lf refl %+.4lf t %+.4lf",
    //          unit_direction.x,
    //          unit_direction.y,
    //          unit_direction.z,
    //          rec->normal.x,
    //          rec->normal.y,
    //          rec->normal.z,
    //          d,
    //          cos_theta,
    //          refraction_ratio,
    //          refl,
    //          t );
    vec3_neg_assign( &unit_direction );

    if( cannot_refract || should_reflect )
    {
        vec3_reflect( &direction, &unit_direction, &rec->normal );
    }
    else
    {
        vec3_refract( &direction, &unit_direction, &rec->normal, refraction_ratio );
    }

    ray_assign( scattered, &rec->p, &direction );
    return 1;
}

void dielectric_create( struct dielectric_t *v, double ir )
{
    v->material.data = v;
    v->material.diffuse = dielectric_diffuse;
    v->material.scatter = dielectric_scatter;
    v->ir = ir;
}
