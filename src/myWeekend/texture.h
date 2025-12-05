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

#endif
