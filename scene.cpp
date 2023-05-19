//
// Created by Mac on 5/18/23.
//

#include "scene.h"

Scene::Scene(float width, float height, float depth) :
viewport_width(width), viewport_height(height), viewport_depth(depth){}

Sphere::Sphere(float x, float y, float z, float radius, int r, int g, int b) : x(x), y(y), z(z), radius(radius), r(r), g(g),
                                                                       b(b) {}
