#pragma once

struct Camera {
    Matrix4 projection;
    Vector3 position;
};

void createPerspectiveProjection(Camera* cam, f32 fov, f32 aspect, f32 near, f32 far){

}