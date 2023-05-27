//
// Created by Matthew on 5/18/23.
//

#ifndef SLOTH_RAYTRACER_SCENE_H
#define SLOTH_RAYTRACER_SCENE_H

#include <vector>
#include "../geometry/geometry.h"
#include "../image/color.h"

struct Sphere {
    Sphere(Vec3f position, float radius, Color color, float specular, float reflective=0);
    Sphere(Vec3f position, float radius, Color color);

    Vec3f position;
    Color color;
    float radius;
    float specular;
    float reflective{};
};

enum class LightType {
    AMBIENT = 0,
    POINT = 1,
    DIRECTIONAL = 2
};

struct Light {

    Light(LightType type, const Vec3f &position, float intensity);

    LightType type;
    Vec3f position;
    float intensity;
};

struct Scene {
    Scene();
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
};

#endif //SLOTH_RAYTRACER_SCENE_H
