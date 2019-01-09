#define GLEW_STATIC

#include "utilities.h"
#include "graphics_math.h"
#include "graphics_utilities.h"
#include "os_operations.h"
#include "gl_renderer.h"

#define WIDTH 1280
#define HEIGHT 720

s8* vs = " \
#version 330 core\n \
attribute vec3 position; \
void main(){ \
    gl_Position = vec4(position, 1); \
} \
";

s8* fs = " \
#version 330 core\n \
out vec4 pixelColor; \
void main(){ \
    pixelColor = vec4(1, 0.5, 0, 1); \
} \
";

int main(int argc, char** argv){
    Window win = createWindow("AV Demo", 100, 100, WIDTH, HEIGHT);

    initializeGLRenderer();
    setClearColor(0, 0.5, 1, 1);

    compileShaderVF(vs, fs);

    

    while(!win.closeRequested){
        updateWindowEvents(&win);
        clearColorBuffer();
        swapWindowBuffer(&win);
    }

    deleteWindow(&win);

    return 0;
}