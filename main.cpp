
#include "core.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"

color ray_color(const ray& r, const hittable &world, int depth)
{
	if (depth <= 0)
	{
		return color(0);
	}
	
	hit_record rec;
	if (world.hit(r, 0, infinity, rec))
	{
		point3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
	}
	vec3 dir = normalize(r.dir);
	double t = 0.5 * (dir.y() + 1);
	return color(1 - t) + t * color(0.5, 0.7, 1.0);
}


int main()
{
	// Image
	const double aspect_ratio = 16. / 9.;
	const int image_width = 256;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 1;
	const int max_depth = 1;

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

	// Camera
	camera cam;

	// Render
	
	cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int y = image_height - 1; y >= 0; y--)
	{
		
		cerr << "\r" << to_string((float)(image_height - y) / (float)image_height * 100) + "%" << std::flush;

		for (int x = 0; x < image_width; x++)
		{
			double u = (x + random_double()) / (image_width - 1);
			double v = (y + random_double()) / (image_height - 1);
			color pixel_color;
			for (int s = 0; s < samples_per_pixel; s++)
			{
				double u = (x + random_double()) / (image_width - 1);
				double v = (y + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(cout, pixel_color, samples_per_pixel);
		}
	}
	
	return 0;
}