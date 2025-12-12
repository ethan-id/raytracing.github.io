#include "myWeekend/bvh.h"
#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "texture.h"
#include "quad.h"

void cornellBox() {
    hittable_list world;

    auto add_box = [&](point3 p, vec3 u, vec3 v, vec3 w, shared_ptr<material> mat) {
        world.add(make_shared<quad>(p + w, u, v, mat));
        world.add(make_shared<quad>(p, v, u, mat));
        world.add(make_shared<quad>(p + u, v, w, mat));
        world.add(make_shared<quad>(p, w, v, mat));
        world.add(make_shared<quad>(p + v, w, u, mat));
        world.add(make_shared<quad>(p, u, w, mat));
    };

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuseLight>(color(15, 15, 15));

    world.add(make_shared<quad>(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.add(make_shared<quad>(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.add(make_shared<quad>(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.add(make_shared<quad>(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.add(make_shared<quad>(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.add(make_shared<quad>(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    // Box 1
    point3 center1 = point3(265 + 165/2.0, 165, 295 + 165/2.0);
    vec3 u1 = vec3(cos(degreesToRadians(15))*165, 0, -sin(degreesToRadians(15))*165);
    vec3 v1 = vec3(0, 330, 0);
    vec3 w1 = vec3(sin(degreesToRadians(15))*165, 0, cos(degreesToRadians(15))*165);
    point3 p1 = center1 - 0.5*u1 - 0.5*v1 - 0.5*w1;
    add_box(p1, u1, v1, w1, white);

    // Box 2
    point3 center2 = point3(130 + 165/2.0, 82.5, 65 + 165/2.0);
    vec3 u2 = vec3(cos(degreesToRadians(-18))*165, 0, -sin(degreesToRadians(-18))*165);
    vec3 v2 = vec3(0, 165, 0);
    vec3 w2 = vec3(sin(degreesToRadians(-18))*165, 0, cos(degreesToRadians(-18))*165);
    point3 p2 = center2 - 0.5*u2 - 0.5*v2 - 0.5*w2;
    add_box(p2, u2, v2, w2, white);

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_w           = 1920;
    cam.samples_per_pixel = 20000;
    cam.max_depth         = 100;
    cam.bg        = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(278, 278, -800);
    cam.lookat   = point3(278, 278, 0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void simpleLight() {
    hittable_list world;

    auto pertext = make_shared<noiseTexture>();
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuseLight>(color(4,4,4));
    world.add(make_shared<sphere>(point3(0,7,0), 2, difflight));
    world.add(make_shared<quad>(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_w           = 3840;
    cam.samples_per_pixel = 100000;
    cam.max_depth         = 100;
    cam.bg        = color(0,0,0);

    cam.vfov     = 20;
    cam.lookfrom = point3(26,3,6);
    cam.lookat   = point3(0,2,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void quads() {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_w           = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.bg        = color(0.70, 0.80, 1.00);

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void perlinSpheres() {
    hittable_list world;

    auto pertext = make_shared<noiseTexture>();
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_w           = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.bg        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void bouncingSpheres() {
    hittable_list world;

    auto checkerPattern = make_shared<checkerTexture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checkerPattern)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            point3 center(a + 0.9*randomDouble(), 0.2, b + 0.9*randomDouble());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, randomDouble(0, 0.5), 0);
                    world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_w           = 400;
    cam.samples_per_pixel = 20;
    cam.max_depth         = 50;
    cam.bg        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);

}

void checkeredSpheres() {
    hittable_list world;

    auto checker = make_shared<checkerTexture>(0.32, color(.2, .3, .1), color(.9, .9, .9));

    world.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_w       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.bg        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

void earth() {
    auto earth_texture = make_shared<imageTexture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_w           = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.bg        = color(0.70, 0.80, 1.00);

    cam.vfov     = 20;
    cam.lookfrom = point3(0,0,12);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(hittable_list(globe));
}

void final_scene() {
    hittable_list world;

    // 1. Floor: Checkerboard texture (Quads)
    auto checker = make_shared<checkerTexture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
    world.add(make_shared<quad>(point3(-50, 0, -50), vec3(100, 0, 0), vec3(0, 0, 100), make_shared<lambertian>(checker)));

    // 2. Light: Overhead Quad (Diffuse Light)
    auto light = make_shared<diffuseLight>(color(7, 7, 7));
    world.add(make_shared<quad>(point3(-10, 15, -10), vec3(20, 0, 0), vec3(0, 0, 20), light));

    // 3. Moving Sphere (Motion Blur)
    auto center1 = point3(-4, 2, 0);
    auto center2 = point3(-4, 2, 0) + vec3(0, 0.5, 0);
    auto moving_mat = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    world.add(make_shared<sphere>(center1, center2, 1.0, moving_mat));

    // 4. Glass Sphere (Dielectric)
    auto glass = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 2, 0), 1.5, glass));
    // Bubble inside glass
    world.add(make_shared<sphere>(point3(0, 2, 0), 1.0, make_shared<dielectric>(1.0 / 1.5)));

    // 5. Metal Sphere (Metal)
    auto metal_mat = make_shared<metal>(color(0.8, 0.8, 0.9), 0.1); 
    world.add(make_shared<sphere>(point3(4, 2, 0), 1.5, metal_mat));

    // 6. Earth Sphere (Image Texture) - Background
    auto earth_texture = make_shared<imageTexture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    world.add(make_shared<sphere>(point3(0, 8, -5), 2.0, earth_surface));

    // 7. Perlin Noise Sphere (Noise Texture)
    auto perlin_tex = make_shared<noiseTexture>();
    world.add(make_shared<sphere>(point3(-8, 3, -5), 2.0, make_shared<lambertian>(perlin_tex)));

    // 8. Mirror Quad (Background Metal)
    auto mirror = make_shared<metal>(color(0.9, 0.9, 0.9), 0.0);
    world.add(make_shared<quad>(point3(-15, 0, -15), vec3(30, 0, 0), vec3(0, 10, 0), mirror));

    // Acceleration Structure
    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_w           = 640;
    cam.samples_per_pixel = 100; 
    cam.max_depth         = 50;
    cam.bg                = color(0,0,0);

    cam.vfov     = 40;
    cam.lookfrom = point3(0, 6, 18);
    cam.lookat   = point3(0, 2, 0);
    cam.vup      = vec3(0,1,0);

    // Defocus Blur
    cam.defocus_angle = 0.5;
    cam.focus_dist    = (point3(0, 2, 0) - cam.lookfrom).length();

    cam.render(world);
}

void solar_system() {
    hittable_list world;

    // Sun (Emissive)
    auto sun_mat = make_shared<diffuseLight>(color(10, 10, 10)); 
    world.add(make_shared<sphere>(point3(0, 0, 0), 3.0, sun_mat));

    // Planets
    // Mercury (Grey/Brown)
    auto mercury_mat = make_shared<lambertian>(color(0.5, 0.4, 0.3));
    world.add(make_shared<sphere>(point3(4, 0, 1), 0.3, mercury_mat));

    // Venus (Yellow/White)
    auto venus_mat = make_shared<lambertian>(color(0.9, 0.9, 0.6));
    world.add(make_shared<sphere>(point3(6, 0, -2), 0.5, venus_mat));

    // Earth (Texture)
    auto earth_texture = make_shared<imageTexture>("earthmap.jpg");
    auto earth_mat = make_shared<lambertian>(earth_texture);
    world.add(make_shared<sphere>(point3(9, 0, 0), 0.5, earth_mat));

    // Mars (Red)
    auto mars_mat = make_shared<lambertian>(color(0.8, 0.3, 0.1));
    world.add(make_shared<sphere>(point3(12, 0, 3), 0.4, mars_mat));

    // Jupiter (Orange/Striped)
    auto jupiter_tex = make_shared<noiseTexture>();
    auto jupiter_mat = make_shared<lambertian>(color(0.8, 0.6, 0.4)); // Base color
    world.add(make_shared<sphere>(point3(18, 0, 0), 2.0, jupiter_mat));

    // Saturn (Gold)
    auto saturn_mat = make_shared<lambertian>(color(0.9, 0.8, 0.5));
    world.add(make_shared<sphere>(point3(24, 0, -5), 1.7, saturn_mat));

    // Uranus (Light Blue)
    auto uranus_mat = make_shared<lambertian>(color(0.5, 0.8, 0.9));
    world.add(make_shared<sphere>(point3(30, 0, 4), 1.2, uranus_mat));

    // Neptune (Dark Blue)
    auto neptune_mat = make_shared<lambertian>(color(0.2, 0.2, 0.7));
    world.add(make_shared<sphere>(point3(35, 0, -2), 1.2, neptune_mat));


    // Background stars (Random small diffuse lights far away)
    for (int i = 0; i < 2000; i++) {
        auto random_pos = point3::random(-200, 200);
        if (random_pos.length() < 100) continue;
        
        auto star_color = color::random(0.5, 1.0);
        world.add(make_shared<sphere>(random_pos, randomDouble(0.2, 0.5), make_shared<diffuseLight>(star_color * 5))); 
    }

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_w           = 3840;
    cam.samples_per_pixel = 100000;
    cam.max_depth         = 100;
    cam.bg                = color(0.0, 0.0, 0.0); // Pitch black background

    cam.vfov     = 40;
    cam.lookfrom = point3(0, 30, 40); // Top-down angled view
    cam.lookat   = point3(15, 0, 0);  // Look at center of system (roughly)
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main() {
    switch (7) {
        case 1: bouncingSpheres();  break;
        case 2: checkeredSpheres(); break;
        case 3: earth();            break;
        case 4: perlinSpheres();    break;
        case 5: quads();            break;
        case 6: simpleLight();      break;
        case 7: cornellBox();       break;
        case 8: final_scene();      break;
        case 9: solar_system();     break;
    }
}
