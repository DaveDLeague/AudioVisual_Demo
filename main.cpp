#include "utilities.h"
#include "graphics_math.h"
#include "graphics_utilities.h"
#include "os_operations.h"
#include "gl_renderer.h"

#define WIDTH 1280
#define HEIGHT 720

const s8* vs = " \
#version 410 core\n \
in vec3 position; \
uniform mat4 projectionMatrix; \
void main(){ \
    gl_Position = projectionMatrix * vec4(position, 1); \
} \
";

const s8* fs = " \
#version 410 core\n \
out vec4 pixelColor; \
void main(){ \
    pixelColor = vec4(1, 0.5, 0, 1); \
} \
";

int main(int argc, char** argv){
    Window win = createWindow("AV Demo", 100, 100, WIDTH, HEIGHT);

    initializeGLRenderer();

    setClearColor(0, 0.5, 1, 1);

    Shader shader = compileShaderVF(vs, fs);
    bindShader(&shader);

    f32 vertices[] = {
        -0.5, -0.5, 0.0,
         0.0,  0.5, 0.0,
         0.5, -0.5, 0.0
    };

    u8 indices[] = {
        0, 1, 2
    };

    Buffer b = generateVertexBufferWithData(vertices, sizeof(vertices));
    VertexAttributeType vat = FLOAT3;
    VertexAttributeDescriptor vad = generateVertexAttributeDescriptor(1, &vat);

    Buffer b2 = generateIndexBufferWithData(indices, sizeof(indices));

    Matrix4 m;
    setIdentityMatrix(&m);

    s32 projMatId = getUniformId(&shader, "projectionMatrix");
    setUniformValueMatrix4(projMatId, &m);

    while(!win.closeRequested){
        updateWindowEvents(&win);
        clearColorBuffer();

        drawIndices(TRIANGLES, 0, 3, U8);
        swapWindowBuffer(&win);
    }

    deleteWindow(&win);

    return 0;
}