#include <iostream>
#include "geometry.h"
#include "image.h"
#include "scene.h"


Color TraceRay(Vec3f O, Vec3f D, int t_min, int t_max, Scene &scene);

Vec3f CanvasToViewport(int x, int y, int canvas_width, int canvas_height, float viewport_width, float viewport_height, float viewport_distance);

Vec2f IntersectRaySphere(Vec3f O, Vec3f D, Sphere &sphere);

int main() {
    std::cout << "Sloth: It's ray.. tracing.. time... :D" << std::endl;
    Image canvas = Image(800, 600, Channel::RGBA);

    std::cout << "Reading test scene file" << std::endl;
//    std::ifstream f("test_scene.json");
//    json data = json::parse(f);
    Scene scene(1, 1, 1);
    scene.spheres.emplace_back(0, -1, 3, 1, 255, 0, 0);
    scene.spheres.emplace_back(2, 0, 4, 1, 0, 0, 255);
    scene.spheres.emplace_back(-2, 0, 4, 1, 0, 255, 0);

    Vec3f O = Vec3f(0, 0, 0);

    for (int x = -1*canvas.getWidth()/2; x <= canvas.getWidth()/2; x++) {
        for (int y = -1*canvas.getHeight()/2; y <= canvas.getHeight()/2; y++) {
//            std::cout << "x: " << x << " y: " << y << std::endl;
            Vec3f D = CanvasToViewport(x, y, canvas.getWidth(), canvas.getHeight(), scene.viewport_width, scene.viewport_height, scene.viewport_height);
//            std::cout << D << std::endl;
            Color color = TraceRay(O, D, 1, INT32_MAX, scene);
            canvas.setPixel(x, y, color);
        }
    }

    std::cout << "Writing to file: output.png" << std::endl;
    canvas.writeToDiskAsPNG("output.png");
    return 0;
}

Color TraceRay(Vec3f O, Vec3f D, int t_min, int t_max, Scene &scene) {
    int closest_t = INT32_MAX;
    int closest_sphere_idx = -1;
    for (int i = 0; i < scene.spheres.size(); i++) {
        Vec2f t = IntersectRaySphere(O, D, scene.spheres.at(i));
        if (t.x >= t_min && t.x <= t_max && t.x < closest_t) {
            closest_t = t.x;
            closest_sphere_idx = i;
        }
        if (t.y >= t_min && t.y <= t_max && t.y < closest_t) {
            closest_t = t.y;
            closest_sphere_idx = i;
        }
    }

    if (closest_sphere_idx == -1) {
        return {0, 0, 0, 0};
    }

    Sphere closest_sphere = scene.spheres.at(closest_sphere_idx);
//    std::cout << "closest sphere: " << closest_sphere_idx << std::endl;
    return {closest_sphere.r, closest_sphere.g, closest_sphere.b, 255};
}

Vec2f IntersectRaySphere(Vec3f O, Vec3f D, Sphere &sphere) {
    Vec3f CO = O - Vec3f(sphere.x, sphere.y, sphere.z);
    float a = D * D;
    float b = 2 * (CO * D);
    float c = (CO*CO) - (sphere.radius * sphere.radius);

    float discriminant = b*b - 4*a*c;
    //std::cout << "a: " << a << " b: " << b << " c: " << c << " discriminant: " << discriminant << std::endl;
    if (discriminant < 0) {
        return {INT32_MAX, INT32_MAX};
    }

    float t1 = (-1.*b + sqrt(discriminant)) / (2.*a);
    float t2 = (-1.*b - sqrt(discriminant)) / (2.*a);
    return {t1, t2};
}

Vec3f CanvasToViewport(int x, int y, int canvas_width, int canvas_height, float viewport_width, float viewport_height, float viewport_distance) {
    return {(float)x * (viewport_width/(float)canvas_width),
            (float)y * (viewport_height/(float)canvas_height),
            viewport_distance};
}
