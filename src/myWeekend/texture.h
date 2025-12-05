#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtw_stb_image.h"

class texture {
    public:
        virtual ~texture() = default;

        virtual color value(double u, double v, const point3& p) const = 0;
};

class solidColor : public texture {
    public:
        solidColor(const color& albedo) : albedo(albedo) {}

        solidColor(double r, double g, double b) : solidColor(color(r,g,b)) {}

        color value(double u, double v, const point3& p) const override {
            return albedo;
        }

    private:
        color albedo;
};

class checkerTexture : public texture {
    public:
        checkerTexture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
            : invScale(1.0 / scale), even(even), odd(odd) {}

        checkerTexture(double scale, const color& c1, const color& c2)
            : checkerTexture(scale, make_shared<solidColor>(c1), make_shared<solidColor>(c2)) {}

        color value(double u, double v, const point3& p) const override {
            auto xInt = int(std::floor(invScale * p.x()));
            auto yInt = int(std::floor(invScale * p.y()));
            auto zInt = int(std::floor(invScale * p.z()));

            bool isEven = (xInt + yInt + zInt) % 2 == 0;
            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        double invScale;
        shared_ptr<texture> even;
        shared_ptr<texture> odd;
};

class imageTexture : public texture {
    public:
        imageTexture(const char* fileName) : img(fileName) {}

        color value(double u, double v, const point3& p) const override {
            // If no texture data, return cyan to notice easier while debugging
            if (img.height() <= 0) return color(0, 1, 1); 

            u = interval(0, 1).clamp(u);
            v = 1.0 - interval(0, 1).clamp(v);

            auto i = int(u * img.width());
            auto j = int(v * img.height());
            auto pixel = img.pixel_data(i, j);
            auto colorScale = 1.0 / 255.0;
            return color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
        }

    private:
        rtw_image img;
};

#endif
