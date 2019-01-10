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
void main(){ \
    tCrds = texCoords; \n\
    gl_Position = vec4(position, 1); \
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