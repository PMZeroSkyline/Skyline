#include <iostream>
#include <string>

#include "vec3.h"
#include "color.h"
#include "ray.h"

using std::cout;
using std::endl;
using std::cerr;
using std::flush;
using std::string;
using std::to_string;

color ray_color(const ray& r)
{
	vec3 dir = normalize(r.dir);
	double t = 0.5 * (dir.y() + 1.0);
	return (1 - t) * color(1, 1, 1) + t * color(0.5, 0.7, 1);
}

int main()
{
	// Image
	const double aspect_ratio = 16. / 9.;
	const int image_width = 256;
	const int image_height = 256;

	// Camera
	double viewport_height = 2.;
	double viewport_width = aspect_ratio * viewport_height;
	double focal_length = 1.;

	point3 origin;
	point3 horizontal = vec3(viewport_width, 0, 0);
	point3 vertical = vec3(0, viewport_height, 0);
	point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

	// Render
	cerr << "\nstart render\n" << std::flush;
	cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for (int y = image_height - 1; y >= 0; y--)
	{
		cerr << "\r" << to_string((float)(image_height - y) / (float)image_height * 100) + "%" << std::flush;
		for (int x = 0; x < image_width; x++)
		{
			double u = double(x) / (image_width - 1);
			double v = double(y) / (image_height - 1);

			ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
			color pixel_color = ray_color(r);
			write_color(cout, pixel_color);
		}
	}
	cerr << "\ndone\n" << std::flush;
	system("pause");
	return 0;
}