#include "color.h"
#include "vec3.h"

#include <iostream>

int main() {
    auto aspect_ratio = 16.0 / 9.0;
    int image_w = 400;
    int image_h = int(image_w / aspect_ratio);

    image_h = (image_h < 1) ? 1 : image_h; // ensure image height is at least 1

    // Viewport widths less than one are fine since they're "real valued". (Idk what that really means)
    auto viewport_h = 2.0; // arbitrary num
    auto viewport_w = viewport_h * (double(image_w)/image_h);

    // Render
    std::cout << "P3\n" << image_w << ' ' << image_h << "\n255\n";

    for (int j = 0; j < image_h; j++) {
        std::clog << "\rScanlines remaining: " << (image_h - j) << ' ' << std::flush;
        for (int i = 0; i < image_h; i++) {
            auto pixel = color(double(i)/(image_w-1), double(j)/(image_h-1), 0);
            write_color(std::cout, pixel);
        }
    }
    
    std::clog << "\rDone.                           \n";
}
