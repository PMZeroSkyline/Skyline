
#include "core.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "material.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"
#include "bvh.h"

color ray_color(const ray &r, const color &background, const hittable &world, int depth)
{
	hit_record rec;

	if (depth <= 0)
	{
		return color(0);
	}

	if (!world.hit(r, 0.001, infinity, rec))
	{
		return background;
	}
	ray scattered;
	color attenuation;
	color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
	{
		return emitted;
	}
	return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

void sample_pixel(int samples_per_pixel, int x, int y, int image_width, int image_height, const camera &cam, color &pixel_color, const color &background, const hittable &world, int max_depth, int id)
{
	for (int s = 0; s < samples_per_pixel; s++)
	{
		double u = (x + random_double()) / (image_width - 1);
		double v = (y + random_double()) / (image_height - 1);
		ray r = cam.get_ray(u, v);
		pixel_color += ray_color(r, background, world, max_depth);
	}
}
hittable_list sphere_with_ground()
{
	hittable_list world;
	auto sphere_material = make_shared<lambertian>(color(0));
	auto ground_material = make_shared<lambertian>(color(1));
    world.add(make_shared<sphere>(point3(0,0,0), 0.5, sphere_material));
    world.add(make_shared<sphere>(point3(0,-100.5,0), 100, ground_material));
	return world;
}
hittable_list random_scene()
{
	hittable_list world;
	auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	auto ground_material = make_shared<lambertian>(checker);
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			double choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9)
			{
				shared_ptr<material> sphere_material;
				if (choose_mat < 0.8)
				{
					color albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					vec3 center2 = center + vec3(0, random_double(0, 0.5), 0);
					world.add(make_shared<moving_sphere>(center, center2, 0, 1, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95)
				{
					color albedo = color::random(0.5, 1);
					double fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else
				{
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}
	shared_ptr<material> material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	shared_ptr<material> material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	shared_ptr<material> material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}
hittable_list two_spheres()
{
	hittable_list objects;
	auto checker = make_shared<checker_texture>(color(0.2,0.3,0.2), color(0.9,0.9,0.9));
	objects.add(make_shared<sphere>(point3(0,-10,0), 10, make_shared<lambertian>(checker)));
	objects.add(make_shared<sphere>(point3(0,10,0), 10, make_shared<lambertian>(checker)));
	return objects;
}
hittable_list two_perlin_spheres()
{
	hittable_list objects;
	auto pertext = make_shared<noise_texture>(4);
	objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));
	return objects;
}
hittable_list earth()
{
	auto earth_texture = make_shared<image_texture>("earthmap.jpg");
	auto earth_surface = make_shared<lambertian>(earth_texture);
	auto globe = make_shared<sphere>(point3(0), 2, earth_surface);
	return hittable_list(globe);
}
hittable_list simple_light()
{
	hittable_list objects;
	auto pertext = make_shared<noise_texture>(1);
	auto difflight = make_shared<diffuse_light>(color(8));

	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));
	objects.add(make_shared<xy_rect>(3,5,1,3,-2,difflight));
	return objects;
}
hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(15, 15, 15));
	auto box1_mat = make_shared<lambertian>(color(.73));
	auto box2_mat = make_shared<lambertian>(color(.73));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
	
	shared_ptr<hittable> box1 = make_shared<box>(point3(0), point3(165, 330, 165), box1_mat);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(264, 0, 295));
	objects.add(box1);
	shared_ptr<hittable> box2 = make_shared<box>(point3(0), point3(165), box2_mat);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 64));
	objects.add(box2);

    return objects;
}
hittable_list cornell_box_smoke() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
	auto green = make_shared<lambertian>(color(.12, .45, .15));
	auto light = make_shared<diffuse_light>(color(7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));
	
	shared_ptr<hittable> box1 = make_shared<box>(point3(0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(264, 0, 295));
	box1 = make_shared<constant_medium>(box1, 0.01, color(0));
	objects.add(box1);
	shared_ptr<hittable> box2 = make_shared<box>(point3(0), point3(165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 64));
	box2 = make_shared<constant_medium>(box2, 0.01, color(1));
	objects.add(box2);

    return objects;
}
hittable_list final_scene()
{
	hittable_list boxes1;
	auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));
	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++)
	{
		for (int j = 0; j < boxes_per_side; j++)
		{
			double w = 100.0;
			double x0 = -1000 + i*w;
			double z0 = -1000 + j*w;
			double y0 = 0;
			double x1 = x0 + w;
			double y1 = random_double(1, 101);
			double z1 = z0 + w;
			boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
		}
	}
	
	hittable_list objects;
	objects.add(make_shared<bvh_node>(boxes1, 0, 1));

	auto light = make_shared<diffuse_light>(color(7));
	objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

	auto center1 = point3(400, 400, 200);
	auto center2 = center1 + point3(30, 0, 0);
	auto moveing_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
	objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moveing_sphere_material));

	objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	objects.add(make_shared<sphere>(point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));
	
	auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
	objects.add(boundary);
	objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
	boundary = make_shared<sphere>(point3(0), 5000, make_shared<dielectric>(1.5));
	objects.add(make_shared<constant_medium>(boundary, 0.0001, color(1)));

	auto emat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
	objects.add(make_shared<sphere>(point3(0), 100, emat));
	auto pertext = make_shared<noise_texture>(0.1);
	objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

	hittable_list boxes2;
	auto white = make_shared<lambertian>(color(.73));
	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
	}
	objects.add(make_shared<translate>(make_shared<rotate_y>(make_shared<bvh_node>(boxes2, 0, 1), 15), vec3(-100, 270, 295)));
	
	return objects;
}

int main(int argc, char **argv)
{
	// Image
	double aspect_ratio = stoi(argv[1]);
	int image_width = stoi(argv[2]);
	int image_height = static_cast<int>(image_width / aspect_ratio);
	int samples_per_pixel = stoi(argv[3]);
	int max_depth = stoi(argv[4]);
	int scene = stoi(argv[5]);

	// World
	hittable_list world;

	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	double vfov = 40.0;
	double aperture = 0.0;
	color background(0);

	switch(scene)
	{
		case 0:
			world = sphere_with_ground();
			background = color(0.7, 0.8, 1.0);
			lookfrom = point3(0,0,5);
			lookat = point3(0,0,0);
			vfov = 40.0;
			aperture = 0.1;
			break;
		case 1:
			world = random_scene();
			background = color(0.7, 0.8, 1.0);
			lookfrom = point3(13,2,3);
			lookat = point3(0);
			vfov = 20.0;
			aperture = 0.1;
			break;
		case 2:
			world = two_spheres();
			background = color(0.7, 0.8, 1.0);
			lookfrom = point3(13,2,3);
			lookat = point3(0,0,0);
			vfov = 20.0;
			break;
		case 3:
			world = two_perlin_spheres();
			background = color(0.7, 0.8, 1.0);
			lookfrom = point3(13,2,3);
			lookat = point3(0);
			vfov = 20.0;
			break;
		case 4:
			world = earth();
			background = color(0.7, 0.8, 1.0);
			lookfrom = point3(13,2,3);
			lookat = point3(0);
			vfov = 20.0;
			break;
		case 5:
			world = simple_light();
			background = color(0);
			lookfrom = point3(26,3,6);
			lookat = point3(0,2,0);
			vfov = 20.0;
			break;
		case 6:
			world = cornell_box();
			background = color(0);
			lookfrom = point3(278, 278, -800);
			lookat = point3(278, 278, 0);
			vfov = 40.0;
			break;
		case 7:
			world = cornell_box_smoke();
			background = color(0);
			lookfrom = point3(278, 278, -800);
			lookat = point3(278, 278, 0);
			vfov = 40.0;
			break;
		case 8:
			world = final_scene();
			background = color(0);
			lookfrom = point3(478, 278, -600);
			lookat = point3(278, 278, 0);
			vfov = 40.0;
			break;
		default:
			return -1;
	}

	vec3 vup(0,1,0);
	double dist_to_focus = 10.0;

	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0, 1);

	// Render
	clock_t t_beg, t_end;
	t_beg = clock();

	cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int y = image_height - 1; y >= 0; y--)
	{
		
		cerr << "\r" << to_string((float)(image_height - y) / (float)image_height * 100) + "%" << flush;

		for (int x = 0; x < image_width; x++)
		{
			color pixel_color;
			sample_pixel(samples_per_pixel, x, y, image_width, image_height, cam, pixel_color, background, world, max_depth, 0);
			write_color(cout, pixel_color, samples_per_pixel);
		}
	}

	t_end = clock();
	cerr << "\r" << "time " << double(t_end - t_beg) / CLOCKS_PER_SEC << " s " << flush;
	
	return 0;
}