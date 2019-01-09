#pragma once

#include "GL/glew.h"

struct Shader {
    u32 id;
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
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        DEBUG_PRINT("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
        DEBUG_PRINT("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
        DEBUG_PRINT("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glValidateProgram(shaderProgram);
    s.id = shaderProgram;
    return s;
}

void bindShader(Shader* s){
    glUseProgram(s->id);
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