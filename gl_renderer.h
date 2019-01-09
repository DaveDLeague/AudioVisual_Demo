#pragma once

#include "GL/glew.h"

struct Shader {
    void* shader;
};

struct Texture {
    void* texture;
};

struct Buffer {
    void* buffer;
};

Shader compileShaderVF(s8* vertexShaderSource, s8* fragmentShaderSource){
    Shader s;
    return s;
}

void clearColorBuffer(){
    glClear(GL_COLOR_BUFFER_BIT);
}

void setClearColor(f32 r, f32 g, f32 b, f32 a){
    glClearColor(r, g, b, a);
}

void initializeGLRenderer(){
    glewInit();
}