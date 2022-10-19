#include "utils.h"

#include <math.h>

double clamp( double v, double minv, double maxv )
{
    if( v < minv )
    {
        return minv;
    }
    if( v > maxv )
    {
        return maxv;
    }
    return v;
}

double degrees_to_radians( double degrees )
{
    return degrees * M_PI / 180.0;
}
