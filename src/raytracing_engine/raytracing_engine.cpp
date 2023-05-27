/*
 * Created by Matthew on 5/27/2023
 */

#include "raytracing_engine.h"

const float INF = std::numeric_limits<float>::infinity();

RayTracingEngine::RayTracingEngine(Scene &scene, Camera &camera, Color background_color)
: scene_(scene), background_color_(background_color), camera_(camera) {}

void RayTracingEngine::setScene(Scene &scene) {
    this->scene_ = scene;
}

void RayTracingEngine::setBackgroundColor(Color color) {
    this->background_color_ = color;
}

Color RayTracingEngine::TraceRay(Vec3f O, Vec3f D, float t_min, float t_max, int num_bounces) {
    auto intersection_pair= ClosestIntersection(O, D, t_min, t_max);
    if (intersection_pair.first == -1) {
        return background_color_;
    }
    Sphere& closest_sphere = scene_.spheres[intersection_pair.first];
    float closest_t = intersection_pair.second;
    Vec3f P = O + (D * closest_t);
    Vec3f N = (P - closest_sphere.position).normalize();
    float intensity = ComputeLighting(P, N, D*-1, closest_sphere.specular);
    Color local_color = closest_sphere.color.MultiplyByIntensity(intensity);
    if (closest_sphere.reflective <= 0 || num_bounces <= 0) {
        return local_color;
    }

    Vec3f R = ReflectRay(D*-1, N);
    Color reflected_color = this->TraceRay(P, R, 0.001, INF, num_bounces - 1);

    return local_color.CombineColor(reflected_color, closest_sphere.reflective);
}

Vec3f RayTracingEngine::CanvasToViewport(int x, int y, int canvas_width, int canvas_height, float viewport_width, float viewport_height, float viewport_distance) {
    return {(float)x * (viewport_width/(float)canvas_width),
            (float)y * (viewport_height/(float)canvas_height),
            viewport_distance};
}

Vec2f RayTracingEngine::IntersectRaySphere(Vec3f O, Vec3f D, Sphere &sphere) {
    Vec3f CO = O - sphere.position;
    float a = D * D;
    float b = 2 * (CO * D);
    float c = (CO*CO) - (sphere.radius * sphere.radius);

    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return {INF, INF};
    }

    float t1 = (-1.*b + sqrt(discriminant)) / (2.*a);
    float t2 = (-1.*b - sqrt(discriminant)) / (2.*a);
    return {t1, t2};
}

float RayTracingEngine::ComputeLighting(Vec3f P, Vec3f N, Vec3f V, float s) {
    float intensity = 0;
    for (auto light : scene_.lights) {
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
            auto intersection_pair = ClosestIntersection(P, L, 0.001, t_max);
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

std::pair<int, float> RayTracingEngine::ClosestIntersection(Vec3f O, Vec3f D, float t_min, float t_max) {
    float closest_t = INF;
    int closest_sphere_idx = -1;
    for (int i = 0; i < scene_.spheres.size(); i++) {
        Vec2f t = IntersectRaySphere(O, D, scene_.spheres.at(i));
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

Vec3f RayTracingEngine::ReflectRay(Vec3f R, Vec3f N) {
    Vec3f reflected_ray = N * 2 * (N*R) - R;
    return reflected_ray;
}

void RayTracingEngine::RenderScene(Image &canvas, int num_bounces) {
    clock_t t_start = clock();
    Vec3f O = camera_.get_position();

    for (int x = -1*canvas.getWidth()/2; x <= canvas.getWidth()/2; x++) {
        for (int y = -1*canvas.getHeight()/2; y <= canvas.getHeight()/2; y++) {
            Vec3f D = CanvasToViewport(x, y,
                                       canvas.getWidth(),
                                       canvas.getHeight(),
                                       camera_.get_viewport_width(),
                                       camera_.get_viewport_height(),
                                       camera_.get_viewport_distance());
            Color color = TraceRay(O, D, 1, INF, num_bounces);
            canvas.setPixel(x, y, color);
        }
    }

    printf("Time taken to complete ray tracing (debug mode will affect this): %.2fs\n", (double) (clock() - t_start)/CLOCKS_PER_SEC);
}
