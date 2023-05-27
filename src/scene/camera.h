//
// Created by Matthew on 5/27/23.
//

#ifndef SLOTH_RAYTRACER_CAMERA_H
#define SLOTH_RAYTRACER_CAMERA_H

#include "../geometry/geometry.h"

class Camera {
private:
    float viewport_width_;
    float viewport_height_;
    float viewport_distance_;
    Vec3f position_;
    Vec3f rotation_;

public:
    Camera(Vec3f position, Vec3f rotation, float viewport_width, float viewport_height, float viewport_distance) :
    viewport_width_(viewport_width),
    viewport_height_(viewport_height),
    viewport_distance_(viewport_distance),
    position_(position),
    rotation_(rotation) {}

    [[nodiscard]] float get_viewport_distance() const { return viewport_distance_; }
    [[nodiscard]] float get_viewport_width() const { return viewport_width_; }
    [[nodiscard]] float get_viewport_height() const { return viewport_height_; }
    [[nodiscard]] Vec3f get_position() const { return position_; }
    [[nodiscard]] Vec3f get_rotation() const { return rotation_; }
};

#endif //SLOTH_RAYTRACER_CAMERA_H
