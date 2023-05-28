/*
 * Created by Matthew on 5/27/2023
 */

#ifndef RAYTRACING_ENGINE_H
#define RAYTRACING_ENGINE_H

#include "../image/color.h"
#include "../image/image.h"
#include "../scene/scene.h"
#include "../geometry/geometry.h"
#include "../scene/camera.h"

class RayTracingEngine {
private:
    Scene scene_;
    Color background_color_;
    Camera camera_;

public:
    explicit RayTracingEngine(Scene &scene, Camera &camera, Color background_color=Color(135, 206, 235, 255));

    void setScene(Scene &scene);
    void setBackgroundColor(Color color);
    
    Color TraceRay(Vec3f O, Vec3f D, float t_min, float t_max, int num_bounces);

    Vec3f CanvasToViewport(int x, int y, int canvas_width, int canvas_height);

    Vec2f IntersectRaySphere(Vec3f O, Vec3f D, Sphere &sphere);

    float ComputeLighting(Vec3f P, Vec3f N, Vec3f V, float s);

    std::pair<int, float> ClosestIntersection(Vec3f O, Vec3f D, float t_min, float t_max);

    static Vec3f ReflectRay(Vec3f R, Vec3f N);
    
    void RenderScene(Image &canvas, int num_bounces);
};

#endif // RAYTRACING_ENGINE_H
