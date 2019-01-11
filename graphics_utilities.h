#pragma once

struct Camera {
    Matrix4 projection;
    Quaternion orientation;
    Vector3 position;
    Vector3 forward;
    Vector3 up;
    Vector3 right;
};

void createPerspectiveProjection(Camera* cam, f32 fov, f32 aspect, f32 near, f32 far){
    cam->projection = generateIdentityMatrix();
    cam->projection.m[0][0] = 1.0 / (aspect * tan(fov / 2));
    cam->projection.m[1][1] = 1.0 / (tan(fov / 2));
    cam->projection.m[2][2] = -(far + near) / (far - near);
    cam->projection.m[2][3] = -1;
    cam->projection.m[3][2] = -(2 * far * near) / (far - near);
}