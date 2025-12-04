#ifndef MATERIAL_H
#define MATERIAL_H

#include <cmath>
class material {
    public:
        virtual ~material() = default;

        // Return a boolena if the incoming ray will scatter or not
        virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const {
            return false;
        }
};

class lambertian : public material {
    public:
        lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            auto scatter_dir = rec.normal + getRandomUnitVector();

            if (scatter_dir.near_zero())
                scatter_dir = rec.normal;
            scattered = ray(rec.p, scatter_dir, ray_in.time());
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
};

class metal : public material {
    public:
        metal (const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            reflected = convertToUnitVector(reflected) + (fuzz * getRandomUnitVector());
            scattered = ray(rec.p, reflected, r_in.time());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double fuzz;
};

class dielectric : public material {
    public:
        dielectric(double refraction_index) : refraction_index(refraction_index) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

            vec3 unit_dir = convertToUnitVector(r_in.direction());
            double cos_theta = std::fmin(dot(-unit_dir, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 direction;

            if (cannot_refract || reflectance(cos_theta, ri) > randomDouble())
                direction = reflect(unit_dir, rec.normal);
            else
                direction = refract(unit_dir, rec.normal, ri);

            scattered = ray(rec.p, direction, r_in.time());
            return true;
        }

    private:
        double refraction_index;

        static double reflectance(double cosine, double refractionIndex) {
            auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
            r0 = r0*r0;
            return r0 + (1-r0)*std::pow((1-cosine), 5);
        }
};

#endif
