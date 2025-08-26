#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = h*h - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (h - std::sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    vec3 unit_dir = unit_vector(r.direction());
    auto a = 0.5*(unit_dir.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main() {
    auto aspect_ratio = 16.0 / 9.0;
    int image_w = 400;

    int image_h = int(image_w / aspect_ratio);
    image_h = (image_h < 1) ? 1 : image_h; // ensure image height is at least 1

    // Viewport widths less than one are fine since they're "real valued". (Idk what that really means)
    auto focal_length = 1.0;
    auto viewport_h = 2.0; // arbitrary num
    auto viewport_w = viewport_h * (double(image_w)/image_h);
    auto camera_center = point3(0,0,0); // AKA eye point
    
    // Vectors across the horizontal and vertical viewport edges
    auto viewport_u = vec3(viewport_w, 0, 0);
    auto viewport_v = vec3(0, -viewport_h, 0);

    // Horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_w;
    auto pixel_delta_v = viewport_v / image_h;

    // Calc location of upper left pixel
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_w << ' ' << image_h << "\n255\n";

    for (int j = 0; j < image_h; j++) {
        std::clog << "\rScanlines remaining: " << (image_h - j) << ' ' << std::flush;
        for (int i = 0; i < image_w; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);

            write_color(std::cout, pixel_color);
        }
    }
    
    std::clog << "\rDone.                           \n";
}
