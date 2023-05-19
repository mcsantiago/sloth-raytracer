//
// Created by Matthew on 5/18/23.
//

#ifndef SLOTH_RAYTRACER_SCENE_H
#define SLOTH_RAYTRACER_SCENE_H

#include <vector>

struct Sphere {
    Sphere(float x, float y, float z, float radius, int r, int g, int b);

    float x, y, z, radius;
    int r, g, b;
};

struct Scene {
    Scene(float width, float height, float depth);

    float viewport_width, viewport_height, viewport_depth;
    std::vector<Sphere> spheres;
};

#endif //SLOTH_RAYTRACER_SCENE_H
