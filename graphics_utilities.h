#pragma once

struct Camera {
    Matrix4 projection;
    Matrix4 view;
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

void updateCameraView(Camera* camera){
    camera->view = generateIdentityMatrix();
    Matrix4 rot = quaternionToMatrix4(camera->orientation);
    translate(&camera->view, &camera->position);
    camera->view = multiply(&rot, &camera->view);
    Vector3 newUp = vector3(camera->view.m[0][1], camera->view.m[1][1], camera->view.m[2][1]);
    Vector3 newRight = vector3(camera->view.m[0][0], camera->view.m[1][0], camera->view.m[2][0]);
    Vector3 newForward = vector3(-camera->view.m[0][2], -camera->view.m[1][2], -camera->view.m[2][2]);
    normalize(&newUp);
    normalize(&newRight);
    normalize(&newForward);
    camera->up = newUp;
    camera->right = newRight; 
    camera->forward = newForward;
    camera->view = multiply(&camera->projection, &camera->view);
}