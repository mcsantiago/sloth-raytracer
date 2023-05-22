#include <iostream>
#include "geometry.h"
#include "scene.h"
#include "image.h"


Color TraceRay(Vec3f O, Vec3f D, int t_min, int t_max, Scene &scene);

Vec3f CanvasToViewport(int x, int y, int canvas_width, int canvas_height, float viewport_width, float viewport_height, float viewport_distance);

Vec2f IntersectRaySphere(Vec3f O, Vec3f D, Sphere &sphere);

float ComputeLighting(Vec3f P, Vec3f N, Scene &scene);

int main() {
    std::cout << "Sloth: It's ray.. tracing.. time... :D" << std::endl;
    Image canvas = Image(800, 600, Channel::RGBA);

    std::cout << "Reading test scene file" << std::endl;
//    std::ifstream f("test_scene.json");
//    json data = json::parse(f);
    Scene scene(1, 1, 1);
    scene.spheres.emplace_back(Vec3f(0, -1, 3), 1, Vec3i(255, 0, 0));
    scene.spheres.emplace_back(Vec3f(2, 0 , 4), 1, Vec3i(0, 0, 255));
    scene.spheres.emplace_back(Vec3f(-2, 0 , 4), 1, Vec3i(0, 255, 0));
    scene.spheres.emplace_back(Vec3f(0, -5001, 0), 5000, Vec3i(255, 255, 0));

    scene.lights.emplace_back(LightType::AMBIENT, Vec3f(), 0.2);
    scene.lights.emplace_back(LightType::POINT, Vec3f(2, 1, 0), 0.6);
    scene.lights.emplace_back(LightType::DIRECTIONAL, Vec3f(1, 4, 4), 0.2);

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
    float closest_t = INT32_MAX;
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
    Vec3f P = O + (D * closest_t);
    Vec3f N = P - closest_sphere.position;
    float intensity = ComputeLighting(P, N, scene);
    return {int(intensity * closest_sphere.color.x),
            int(intensity * closest_sphere.color.y),
            int(intensity * closest_sphere.color.z),
            255};
}

float ComputeLighting(Vec3f P, Vec3f N, Scene &scene) {
    float intensity = 0;
    for (auto light : scene.lights) {
        if (light.type == LightType::AMBIENT) {
            intensity += light.intensity;
        } else {
            Vec3f L;
            if (light.type == LightType::POINT) {
                L = light.position - P;
            } else if (light.type == LightType::DIRECTIONAL) {
                L = light.position; // Re-using position to indicate direction... refactor?
            }

            float n_dot_l = N*L;
            if (n_dot_l > 0) {
                intensity += light.intensity * n_dot_l/(N.norm() * L.norm());
            }
        }
    }

    return intensity;
}

Vec2f IntersectRaySphere(Vec3f O, Vec3f D, Sphere &sphere) {
    Vec3f CO = O - sphere.position;
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
