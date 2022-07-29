#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <chrono>
#include <math.h>

double hit_sphere(const point3& center, double radius, const ray &r)
{
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
    {
        return -1;
    }
    else
    {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

color ray_color(const ray& r)
{
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main()
{
    auto start = std::chrono::steady_clock::now();

    const auto aspect_ratio = 16.0 / 9.0;
    const int width = 400;
    const int height = static_cast<int>(width / aspect_ratio);

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);


    std::cout << "P3\n" << width << ' ' << height << "\n255\n";

    for (int vi = height - 1; vi >= 0; --vi)
    {
        std::cerr << "\rScanlines remaining:" << vi << ' ' << std::flush;
        for (int ui = 0; ui < width; ++ui)
        {
            auto du = double(ui) / (width - 1);
            auto dv = double(vi) / (height- 1);
            ray r(origin, lower_left_corner + du * horizontal + dv * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone.";

    auto end = std::chrono::steady_clock::now();
    std::cerr << "Done. time in seconds: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() * 0.001 << " ms\n";
    return 0;
}