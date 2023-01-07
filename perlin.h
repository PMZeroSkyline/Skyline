#ifndef PERLIN_H
#define PERLIN_H

#include "core.h"
#include "vec3.h"

class perlin
{
public:
    static const int point_count = 256;
    double *ranfloat;
    int *perm_x;
    int *perm_y;
    int *perm_z;
    vec3 *ranvec;

    perlin()
    {
        ranfloat = new double[point_count];
        for (size_t i = 0; i < point_count; i++)
        {
            ranfloat[i] = random_double();
        }
        ranvec = new vec3[point_count];
        for (size_t i = 0; i < point_count; i++)
        {
            ranvec[i] = normalize(vec3::random(-1, 1));
        }
        
        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }
    ~perlin()
    {
        delete[] ranfloat;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
        delete[] ranvec;
    }
    
    double noise_hashed(const point3 &p) const
    {
        auto i = static_cast<int>(4*p.x()) & 255;
        auto j = static_cast<int>(4*p.y()) & 255;
        auto k = static_cast<int>(4*p.z()) & 255;

        return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }
    double noise_trilinearly(const point3 &p) const
    {
        double u = p.x() - floor(p.x());
        double v = p.y() - floor(p.y());
        double w = p.z() - floor(p.z());
        
        auto i = static_cast<int>(floor(p.x()));
        auto j = static_cast<int>(floor(p.y()));
        auto k = static_cast<int>(floor(p.z()));

        double c[2][2][2];

        for (int di = 0; di < 2; di++)
        {
            for (int dj = 0; dj < 2; dj++)
            {
                for (int dk = 0; dk < 2; dk++)
                {
                    c[di][dj][dk] = ranfloat[
                        perm_x[(i+di) & 255] ^
                        perm_y[(j+dj) & 255] ^
                        perm_z[(k+dk) & 255]
                        ];
                }
            }
        }
        return trilinear_interp(c, u, v, w);
    }
    double noise_trilinearly_smoothed(const point3 &p) const
    {
        double u = p.x() - floor(p.x());
        double v = p.y() - floor(p.y());
        double w = p.z() - floor(p.z());

        #pragma region Hermitian Smoothing
        u = u*u*(3-2*u);
        v = v*v*(3-2*v);
        w = w*w*(3-2*w);
        #pragma endregion
        
        int i = static_cast<int>(floor(p.x()));
        int j = static_cast<int>(floor(p.y()));
        int k = static_cast<int>(floor(p.z()));

        double c[2][2][2];

        for (int di = 0; di < 2; di++)
        {
            for (int dj = 0; dj < 2; dj++)
            {
                for (int dk = 0; dk < 2; dk++)
                {
                    c[di][dj][dk] = ranfloat[
                        perm_x[(i+di) & 255] ^
                        perm_y[(j+dj) & 255] ^
                        perm_z[(k+dk) & 255]
                        ];
                }
            }
        }
        return trilinear_interp(c, u, v, w);
    }
    double noise_trilinearly_smoothed_ranvec(const point3 &p) const
    {
        double u = p.x() - floor(p.x());
        double v = p.y() - floor(p.y());
        double w = p.z() - floor(p.z());

        int i = static_cast<int>(floor(p.x()));
        int j = static_cast<int>(floor(p.y()));
        int k = static_cast<int>(floor(p.z()));

        vec3 c[2][2][2];

        for (int di = 0; di < 2; di++)
        {
            for (int dj = 0; dj < 2; dj++)
            {
                for (int dk = 0; dk < 2; dk++)
                {
                    c[di][dj][dk] = ranvec[
                        perm_x[(i+di) & 255] ^
                        perm_y[(j+dj) & 255] ^
                        perm_z[(k+dk) & 255]
                        ];
                }
            }
        }
        return perlin_interp(c, u, v, w);
    }
    static double perlin_interp(vec3 c[2][2][2], double u, double v, double w)
    {
        double uu = u*u*(3-2*u);
        double vv = v*v*(3-2*v);
        double ww = w*w*(3-2*w);

        double accum = 0.0;

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    vec3 weight_v(u-i, v-j, w-k);
                    accum += (i*uu + (1-i)*(1-uu)) * 
                             (j*vv + (1-j)*(1-vv)) * 
                             (k*ww + (1-k)*(1-ww)) * 
                             dot(c[i][j][k], weight_v);
                }
            }
        }
        return accum;
    }
    static double trilinear_interp(double c[2][2][2], double u, double v, double w)
    {
        double accum = 0.0;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    accum += (i*u+(1-i)*(1-u)) *
                             (j*v+(1-j)*(1-v)) * 
                             (k*w+(1-k)*(1-w)) * 
                             c[i][j][k];
                }
            }
        }
        return accum;
    }
    static int *perlin_generate_perm()
    {
        int *p = new int[point_count];

        for (size_t i = 0; i < perlin::point_count; i++)
        {
            p[i] = i;
        }

        permute(p, point_count);

        return p;
    }
    static void permute(int *p, int n)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
        
    }
    double turb(const point3 &p, int depth = 7) const
    {
        double d = noise_trilinearly_smoothed_ranvec(p);

        double accum = 0.0;
        point3 temp_p = p;
        double weight = 1.0;
        for (int i = 0; i < depth; i++)
        {
            accum += weight * noise_trilinearly_smoothed_ranvec(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }
        return fabs(accum);
    }
};

#endif