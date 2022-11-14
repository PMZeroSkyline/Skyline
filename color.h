#ifndef COLOR_H
#define COLOR_H
#include "vec3.h"

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) 
{
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    double scale = 1. / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif