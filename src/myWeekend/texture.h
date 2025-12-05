#ifndef TEXTURE_H
#define TEXTURE_H

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

    private:
        double invScale;
        shared_ptr<texture> even;
        shared_ptr<texture> odd;
};

#endif
