//
// Created by Matthew on 5/18/23.
//

#ifndef SLOTH_RAYTRACER_SCENE_H
#define SLOTH_RAYTRACER_SCENE_H

#include <vector>
#include "geometry.h"

struct Sphere {
    Sphere(Vec3f position, float radius, Vec3i color, float specular, float reflective=0);
    Sphere(Vec3f position, float radius, Vec3i color);

    Vec3f position;
    Vec3i color;
    float radius;
    float specular;
    float reflective;
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
    Scene(float width, float height, float depth);

    float viewport_width, viewport_height, viewport_depth;
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
};

#endif //SLOTH_RAYTRACER_SCENE_H
