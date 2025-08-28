#ifndef CAMERA_H
#define CAMERA_H

#include <vector>      // std::vector
#include <fstream>     // std::ofstream
#include <atomic>      // std::atomic
#include <iostream>    // std::cout, std::clog, std::flush
#include <algorithm>   // std::min
                       //
#include "hittable.h"
#include "material.h"
#include "myWeekend/rtweekend.h"
#include "myWeekend/vec3.h"

class camera {
    public:
        double aspect_ratio      = 1.0;  // Ratio of image width over height
        int    image_w           = 100;  // Rendered image width in pixel count
        int    samples_per_pixel = 10;   // Count of random samples per pixel
        int    max_depth         = 10;   // Max num of ray bounces into scene
 
        double vfov     = 90;             // Vertical view angle (field of view)
        point3 lookfrom = point3(0,0,0);  // Point camera is looking from
        point3 lookat   = point3(0,0,-1); // Point camera is looking at 
        vec3 vup        = vec3(0,1,0);    // Camera-relative "up" direction

        double defocus_angle = 0;
        double focus_dist = 10;

        void render(const hittable& world) {
            initialize();

            std::vector<color> framebuffer(image_w * image_h);
            std::atomic<int> done_rows{0};

            auto idx = [&](int i, int j){ return j*image_w + i; };

            #pragma omp parallel for schedule(dynamic,1)
            for (int j = 0; j < image_h; ++j) {
                for (int i = 0; i < image_w; ++i) {
                    color pixel_color(0,0,0);
                    for (int s = 0; s < samples_per_pixel; ++s) {
                        ray r = getRay(i, j);          // use thread_local RNG inside
                        pixel_color += getRaysColor(r, max_depth, world);
                    }
                    framebuffer[idx(i,j)] = pixel_samples_scale * pixel_color;
                }
                int d = ++done_rows;
                if ((d & 15) == 0) {
                    std::clog << "\rScanlines remaining: " << (image_h - d) << ' ' << std::flush;
                }
            }

            std::ofstream out("image.ppm", std::ios::binary);
            out << "P3\n" << image_w << ' ' << image_h << "\n255\n";
            for (int j = 0; j < image_h; ++j)
                for (int i = 0; i < image_w; ++i)
                    writeColor(out, framebuffer[idx(i,j)]);
            std::clog << "\rDone.                 \n";
        }

    private:
        int    image_h;             // Rendered image height
        double pixel_samples_scale; // Color scale factor for a sum of pixel samples
        point3 center;              // Camera center
        point3 pixel00_loc;         // Location of pixel at 0,0
        vec3   pixel_delta_u;       // Offset to pixel to the right
        vec3   pixel_delta_v;       // Offset to pixel below
        vec3   u, v, w;             // Camera frame basis vectors
        vec3   defocus_disk_u;
        vec3   defocus_disk_v;

        void initialize() {
            image_h = int(image_w / aspect_ratio);
            image_h = (image_h < 1) ? 1 : image_h;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            // Determine viewport dimensions.
            auto theta = degreesToRadians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2.0 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_w)/image_h);

            // Calc u,v,w unit basis vecs for the camera coord frame
            w = convertToUnitVector(lookfrom - lookat); 
            u = convertToUnitVector(cross(vup, w)); 
            v = cross(w, u);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            auto viewport_u = viewport_width * u;;
            auto viewport_v = viewport_height * -v;

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_w;
            pixel_delta_v = viewport_v / image_h;

            // Calculate the location of the upper left pixel.
            auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            auto defocus_radius = focus_dist * std::tan(degreesToRadians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        // Construct a ray originating from the origin and directed at a randomly sampled point around the pixel at i, j
        ray getRay(int i, int j) const {
            auto offset = getSampleSquare();
            auto pixel_sample_vec = pixel00_loc
                            + ((i + offset.x()) * pixel_delta_u)
                            + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = (defocus_angle <= 0) ? center : getDefocusDiskSample();
            auto ray_dir = pixel_sample_vec - ray_origin;

            return ray(ray_origin, ray_dir);
        }

        // Returns the vector to a random point in the [-0.5,-0.5] - [+0.5, +0.5] unit square
        vec3 getSampleSquare() const {
            return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
        }

        point3 getDefocusDiskSample() const {
            auto p = getRandomInUnitDisk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color getRaysColor(const ray& r, int depth, const hittable& world) const {
            // if past depth, no more light, return black
            if (depth <= 0)
                return color(0,0,0);

            hit_record rec;

            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * getRaysColor(scattered, depth-1, world);
                return color(0,0,0);
            }

            vec3 unit_dir = convertToUnitVector(r.direction());
            auto a = 0.5*(unit_dir.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        }
};

#endif
