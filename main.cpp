
#include "core.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"

color ray_color(const ray& r, const hittable &world, int depth)
{
	if (depth <= 0)
	{
		return color(0);
	}
	
	hit_record rec;
	if (world.hit(r, 0.001, infinity, rec))
	{
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return color(0);
	}
	vec3 dir = normalize(r.dir);
	double t = 0.5 * (dir.y() + 1);
	return color(1 - t) + t * color(0.5, 0.7, 1.0);
}


int main()
{
	// Image
	const double aspect_ratio = 16. / 9.;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 128;
	const int max_depth = 16;

	// World
	hittable_list world;
	auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
	auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
	auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3);
	auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

	world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, material_ground));
	world.add(make_shared<sphere>(point3(0,0,-1), 0.5, material_center));
	world.add(make_shared<sphere>(point3(-1,0,-1), 0.5, material_left));
	world.add(make_shared<sphere>(point3(1,0,-1), 0.5, material_right));


	// Camera
	camera cam;

	// Render
	
	cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int y = image_height - 1; y >= 0; y--)
	{
		
		cerr << "\r" << to_string((float)(image_height - y) / (float)image_height * 100) + "%" << std::flush;

		for (int x = 0; x < image_width; x++)
		{
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