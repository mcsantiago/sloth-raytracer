//
// Created by Mac on 5/18/23.
//

#include "scene.h"

Scene::Scene(float width, float height, float depth) :
viewport_width(width), viewport_height(height), viewport_depth(depth){}

Sphere::Sphere(Vec3f position, float radius, Vec3i color) : position(position), radius(radius), color(color) {}

Light::Light(LightType type, const Vec3f &position, float intensity) : type(type), position(position),
                                                                       intensity(intensity) {}
