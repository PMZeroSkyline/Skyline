#ifndef TEXTURE_H
#define TEXTURE_H

#include "core.h"
#include "vec3.h"
#include "perlin.h"
#include "image.h"

class texture
{
public:
    virtual color value(double u, double v, const point3 &p) const = 0;
};

class solid_color : public texture
{
public:
    color color_value;

    solid_color(){}
    solid_color(color c) : color_value(c){}
    solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)){}

    virtual color value(double u, double v, const vec3 &p) const override
    {
        return color_value;
    }
};

class checker_texture : public texture
{
public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;

    checker_texture(){}
    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd) : even(_even), odd(_odd){}
    checker_texture(color c1, color c2) : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)){}

    virtual color value(double u, double v, const point3 &p) const override
    {
        double sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
        {
            return odd->value(u, v, p);
        }
        else
        {
            return even->value(u, v, p);
        }
    }
};
class noise_texture : public texture
{
public:
    perlin noise;
    double scale = 1;

    noise_texture(){}
    noise_texture(double sc) : scale(sc){}
    virtual color value(double u, double v, const point3 &p) const override
    {
        //return color(1) * noise.turb(scale * p);
        return color(1) * 0.5 * (1 + noise.noise_hashed(scale * p));
    }
};

class image_texture : public texture
{
public:
    const static int bytes_per_pixel = 3;
    unsigned char *data;
    int width, height;
    int bytes_per_scanline;

    image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0){}
    image_texture(const char *filename)
    {
        int components_per_pixel = bytes_per_pixel;
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
        if (!data)
        {
            cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }
        bytes_per_scanline = bytes_per_pixel * width;
    }
    ~image_texture()
    {
        delete data;
    }
    virtual color value(double u, double v, const vec3 &p) const override
    {
        if (data == nullptr)
        {
            return color(0,1,1);
        }
        u = clamp(u, 0, 1);
        v = 1 - clamp(v, 0, 1);
        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * height);
        if (i >= width)
        {
            i = width - 1;
        }
        if (j >= height)
        {
            j = height - 1;
        }
        const double color_scale = 1.0 / 255.0;
        unsigned char *pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;
        return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }
};

#endif