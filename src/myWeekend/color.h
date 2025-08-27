#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

using color = vec3;

void write_color(std::ostream& out, const color& pxl_color) {
    auto r = pxl_color.x();
    auto g = pxl_color.y();
    auto b = pxl_color.z();

    // Translate the [0,1] component vals to the byte range [0,255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
