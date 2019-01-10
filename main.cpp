#include "utilities.h"
#include "graphics_math.h"
#include "graphics_utilities.h"
#include "os_operations.h"
#include "gl_renderer.h"

#define WIDTH 1280
#define HEIGHT 720

const s8* vs = " \
#version 410 core\n \
layout(location = 0) in vec3 position; \
layout(location = 1) in vec2 texCoords; \
out vec2 tCrds; \
uniform mat4 projectionMatrix; \
uniform mat4 modelMatrix; \
void main(){ \
    tCrds = texCoords; \n\
    gl_Position = projectionMatrix * vec4(position, 1); \
} \
";

const s8* fs = " \
#version 410 core\n \
out vec4 pixelColor; \
in vec2 tCrds; \
uniform sampler2D sampler; \
void main(){ \
    vec4 color = texture(sampler, tCrds);\n \
    pixelColor = color; \n\
} \
";

int main(int argc, char** argv){
    Window win = createWindow("AV Demo", 100, 100, WIDTH, HEIGHT);

    initializeGLRenderer();

    setClearColor(0, 0.5, 1, 1);

    Shader shader = compileShaderVF(vs, fs);
    bindShader(&shader);

    u8 texture[] = {
        0, 0, 255, 255, 255, 0, 0, 255,
        255, 0, 0, 255, 0, 0, 255, 255
    };

    f32 vertices[] = {
        -0.5, -0.5, 0.0,  0, 0,
         0.0,  0.5, 0.0,  0.5, 1,
         0.5, -0.5, 0.0,  1, 0
    };

    u8 indices[] = {
        0, 1, 2
    };


    Buffer b = generateVertexBufferWithData(vertices, sizeof(vertices));
    
    VertexAttributeType vats[] = { FLOAT3, FLOAT2 };
    VertexAttributeDescriptor vad = generateVertexAttributeDescriptor(2, vats);
    Buffer b2 = generateIndexBufferWithData(indices, sizeof(indices));

    Texture t = generateTextureWidthData(texture, 2, 2);

    Camera camera;
    camera.orientation = quaternion(0, 0, 0, 1);
    camera.forward = vector3(0, 0, 1);
    camera.up = vector3(0, 1, 0);
    camera.right = vector3(1, 0, 0);
    camera.position = vector3(0, 0, -15);
    createPerspectiveProjection(&camera, 70.0, (f32)WIDTH/(f32)HEIGHT, 0.001, 1000.0);
    
    Matrix4 view = generateIdentityMatrix();
    translate(&view, &camera.position);

    Matrix4 projectionView = multiply(&camera.projection, &view);

    s32 projMatId = getUniformId(&shader, "projectionMatrix");
    setUniformValueMatrix4(projMatId, &projectionView);

    f32 cameraMoveSpeed = 0.01;
    f32 cameraRotateSpeed = 0.001;

    while(!win.closeRequested){
        updateWindowEvents(&win);
        clearColorBuffer();
        
        camera.position -= camera.forward * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_W];
        camera.position += camera.forward * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_S];
        camera.position += camera.right * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_A];
        camera.position -= camera.right * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_D];
        camera.position -= camera.up * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_R];
        camera.position += camera.up * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_F];
        rotate(&camera.orientation, camera.up, keyboardInputs[SDL_SCANCODE_RIGHT] * cameraRotateSpeed);
        rotate(&camera.orientation, camera.up, keyboardInputs[SDL_SCANCODE_LEFT] * -cameraRotateSpeed);
        rotate(&camera.orientation, camera.forward, keyboardInputs[SDL_SCANCODE_Q] * cameraRotateSpeed);
        rotate(&camera.orientation, camera.forward, keyboardInputs[SDL_SCANCODE_E] * -cameraRotateSpeed);
        rotate(&camera.orientation, camera.right, keyboardInputs[SDL_SCANCODE_UP] * -cameraRotateSpeed);
        rotate(&camera.orientation, camera.right, keyboardInputs[SDL_SCANCODE_DOWN] * cameraRotateSpeed);

        view = generateIdentityMatrix();
        translate(&view, &camera.position);

        Matrix4 rot = quaternionToMatrix4(camera.orientation);
        view = multiply(&rot, &view);
        Vector3 newUp = vector3(view.m[0][1], view.m[1][1], view.m[2][1]);
        Vector3 newRight = vector3(view.m[0][0], view.m[1][0], view.m[2][0]);
        Vector3 newForward = vector3(-view.m[0][2], -view.m[1][2], -view.m[2][2]);
        normalize(&newUp);
        normalize(&newRight);
        normalize(&newForward);
        camera.up = newUp;
        camera.right = newRight; 
        camera.forward = newForward;

        projectionView = multiply(&camera.projection, &view);
        setUniformValueMatrix4(projMatId, &projectionView);

        win.closeRequested = keyboardInputs[SDL_SCANCODE_ESCAPE];

        drawIndices(TRIANGLES, 0, 3, U8);
        swapWindowBuffer(&win);
    }

    deleteWindow(&win);

    return 0;
}