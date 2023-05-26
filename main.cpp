#include <iostream>
#include <limits>
#include "geometry.h"
#include "scene.h"
#include "image.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const Color BACKGROUND_COLOR = {135, 206, 235, 255};
const float INF = std::numeric_limits<float>::infinity();

//const Color BACKGROUND_COLOR = {0, 0, 0, 0};

Color TraceRay(Vec3f O, Vec3f D, float t_min, float t_max, Scene &scene, int num_bounces);

Vec3f CanvasToViewport(int x, int y, int canvas_width, int canvas_height, float viewport_width, float viewport_height, float viewport_distance);

Vec2f IntersectRaySphere(Vec3f O, Vec3f D, Sphere &sphere);

float ComputeLighting(Vec3f P, Vec3f N, Vec3f V, float s, Scene &scene);

std::pair<int, float> ClosestIntersection(Vec3f O, Vec3f D, float t_min, float t_max, Scene &scene);

Vec3f ReflectRay(Vec3f R, Vec3f N);


int main() {
    std::cout << "Sloth: It's ray.. tracing.. time... :D" << std::endl;
    Image canvas = Image(800, 600, Channel::RGBA);

    std::cout << "Reading test scene file" << std::endl;
//    std::ifstream f("test_scene.json");
//    json data = json::parse(f);
    Scene scene(1, 1, 1);
    scene.spheres.emplace_back(Vec3f(0, -1, 3), 1, Vec3i(255, 0, 0), 500, 0.2);
    scene.spheres.emplace_back(Vec3f(2, 0 , 4), 1, Vec3i(0, 0, 255), 500, 0.3);
    scene.spheres.emplace_back(Vec3f(-2, 0 , 4), 1, Vec3i(0, 255, 0), 10, 0.4);
    scene.spheres.emplace_back(Vec3f(0, -5001, 0), 5000, Vec3i(255, 255, 0), 1000, 0.5);

    scene.lights.emplace_back(LightType::AMBIENT, Vec3f(), 0.2);
    scene.lights.emplace_back(LightType::POINT, Vec3f(2, 1, 0), 0.6);
    scene.lights.emplace_back(LightType::DIRECTIONAL, Vec3f(1, 4, 4), 0.2);

    Vec3f O = Vec3f(0, 0, 0);

    clock_t t_start = clock();

    for (int x = -1*canvas.getWidth()/2; x <= canvas.getWidth()/2; x++) {
        for (int y = -1*canvas.getHeight()/2; y <= canvas.getHeight()/2; y++) {
//            std::cout << "x: " << x << " y: " << y << std::endl;
            Vec3f D = CanvasToViewport(x, y, canvas.getWidth(), canvas.getHeight(), scene.viewport_width, scene.viewport_height, scene.viewport_height);
//            std::cout << D << std::endl;
            Color color = TraceRay(O, D, 1, INF, scene, 100);
            canvas.setPixel(x, y, color);
        }
    }

    printf("Time taken to complete ray tracing: %.2fs\n", (double) (clock() - t_start)/CLOCKS_PER_SEC);

    std::cout << "Writing to file: output.png" << std::endl;
    canvas.writeToDiskAsPNG("output.png");
    return 0;
}

Color TraceRay(Vec3f O, Vec3f D, float t_min, float t_max, Scene &scene, int num_bounces) {
    auto intersection_pair= ClosestIntersection(O, D, t_min, t_max, scene);
    if (intersection_pair.first == -1) {
        return BACKGROUND_COLOR;
    }
    Sphere& closest_sphere = scene.spheres[intersection_pair.first];
    float closest_t = intersection_pair.second;
    Vec3f P = O + (D * closest_t);
    Vec3f N = (P - closest_sphere.position).normalize();
    float intensity = ComputeLighting(P, N, D*-1, closest_sphere.specular, scene);
    Color local_color = {std::min(int(intensity * closest_sphere.color.x), 255),
            std::min(int(intensity * closest_sphere.color.y), 255),
            std::min(int(intensity * closest_sphere.color.z), 255),
            255};

    if (closest_sphere.reflective <= 0 || num_bounces <= 0) {
        return local_color;
    }

    Vec3f R = ReflectRay(D*-1, N);
    Color reflected_color = TraceRay(P, R, 0.001, INF, scene, num_bounces - 1);
    printf("Local color: (%d,%d,%d)\tReflected color: (%d,%d,%d)\tFinal color: (%d,%d,%d)\n",
           local_color.r,
           local_color.g,
           local_color.b,
           reflected_color.r,
           reflected_color.g,
           reflected_color.b,
           int((local_color.r * (1. - closest_sphere.reflective)) + ((float)reflected_color.r * closest_sphere.reflective)),
           int((local_color.g * (1. - closest_sphere.reflective)) + ((float)reflected_color.g * closest_sphere.reflective)),
           int((local_color.b * (1. - closest_sphere.reflective)) + ((float)reflected_color.b * closest_sphere.reflective)));

    return {
        std::min(int((local_color.r * (1. - closest_sphere.reflective)) + ((float)reflected_color.r * closest_sphere.reflective)), 255),
        std::min(int((local_color.g * (1. - closest_sphere.reflective)) + ((float)reflected_color.g * closest_sphere.reflective)), 255),
        std::min(int((local_color.b * (1. - closest_sphere.reflective)) + ((float)reflected_color.b * closest_sphere.reflective)), 255),
        255
    };
}

float ComputeLighting(Vec3f P, Vec3f N, Vec3f V, float s, Scene &scene) {
    float intensity = 0;
    for (auto light : scene.lights) {
        if (light.type == LightType::AMBIENT) {
            intensity += light.intensity;
        } else {
            Vec3f L;
            float t_max = 1;
            if (light.type == LightType::POINT) {
                L = light.position - P;
                t_max = 1;
            } else if (light.type == LightType::DIRECTIONAL) {
                L = light.position; // Re-using position to indicate direction... refactor?
                t_max = INF;
            }

            // Shadows
            auto intersection_pair = ClosestIntersection(P, L, 0.001, t_max, scene);
            if (intersection_pair.first != -1) {
                continue;
            }

            // Diffuse Reflection
            float n_dot_l = N*L;
            if (n_dot_l > 0) {
                intensity += light.intensity * n_dot_l/(N.norm() * L.norm());
            }

            // Specular Reflection
            if (s != -1) {
                Vec3f R = ReflectRay(L, N);
                float r_dot_v = R*V;
                if (r_dot_v > 0) {
                    intensity += light.intensity * std::pow(r_dot_v/(R.norm() * V.norm()), s);
                }
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
        return {INF, INF};
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

std::pair<int, float> ClosestIntersection(Vec3f O, Vec3f D, float t_min, float t_max, Scene &scene) {
    float closest_t = INF;
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
        return std::make_pair(-1, 0);
    }

    return std::make_pair(closest_sphere_idx, closest_t);
}

Vec3f ReflectRay(Vec3f R, Vec3f N) {
    Vec3f reflected_ray = N * 2 * (N*R) - R;
    /*
    printf("Original Ray: (%.2f,%.2f,%.2f)\tNormal: (%.2f,%.2f,%.2f)\tReflected Ray: (%.2f,%.2f,%.2f)\n",
            R.x, R.y, R.z,
            N.x, N.y, N.z,
            reflected_ray.x, reflected_ray.y, reflected_ray.z);
    */
    return reflected_ray;
}
