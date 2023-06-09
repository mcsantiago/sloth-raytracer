//
// Created by Mac on 5/18/23.
//

#include "scene.h"

Scene::Scene()= default;

Sphere::Sphere(Vec3f position, float radius, Color color, float specular, float reflective) :
position(position), radius(radius), color(color), specular(specular), reflective(reflective) {}

Sphere::Sphere(Vec3f position, float radius, Color color) :
        position(position), radius(radius), color(color), specular(-1) {}

Light::Light(LightType type, const Vec3f &position, float intensity) : type(type), position(position),
                                                                       intensity(intensity) {}
