#define GLEW_STATIC

#include "utilities.h"
#include "graphics_math.h"
#include "graphics_utilities.h"
#include "os_operations.h"
#include "gl_renderer.h"

#define WIDTH 1280
#define HEIGHT 720

int main(int argc, char** argv){
    Window win = createWindow("AV Demo", 100, 100, WIDTH, HEIGHT);

    initializeGLRenderer();
    setClearColor(0, 0.5, 1, 1);

    bool running = true;
    while(!win.closeRequested){
        updateWindowEvents(&win);
        clearColorBuffer();
        swapWindowBuffer(&win);
    }

    deleteWindow(&win);

    return 0;
}