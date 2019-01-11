#pragma once

#define GLEW_STATIC
#include "GL/glew.h"

enum VertexAttributeType {
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    MATRIX4,
};

enum IndexSize {
    U8 = GL_UNSIGNED_BYTE,
    U16 = GL_UNSIGNED_SHORT,
    U32 = GL_UNSIGNED_INT
};

enum RenderType {
    POINTS = GL_POINTS,
    LINES = GL_LINES,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN = GL_TRIANGLE_FAN
};

struct Shader {
    u32 id;
};

struct Texture {
    u32 id;
};

struct Buffer {
    u32 id;
};

struct VertexAttributeDescriptor {
    u32 id;
};

Shader compileShaderVF(const s8* vertexShaderSource, const s8* fragmentShaderSource){
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

s32 getUniformId(Shader* shad, const s8* uniformName){
    return glGetUniformLocation(shad->id, uniformName);
}

void setUniformValueMatrix4(s32 uniformId, Matrix4* mat){
    glUniformMatrix4fv(uniformId, 1, GL_FALSE, &mat->m[0][0]);
}

void clearColorBuffer(){
    glClear(GL_COLOR_BUFFER_BIT);
}

void setClearColor(f32 r, f32 g, f32 b, f32 a){
    glClearColor(r, g, b, a);
}

void setBackfaceCulling(bool mode){
    if(mode){
        glEnable(GL_CULL_FACE);
    }else{
        glDisable(GL_CULL_FACE);
    }
}

Buffer generateVertexBufferWithData(void* vertexData, u32 vertexDataSize){
    Buffer b;
    glGenBuffers(1, &b.id);
    glBindBuffer(GL_ARRAY_BUFFER, b.id);
    glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
    return b;
}

void bindVertexBuffer(Buffer* b){
    glBindBuffer(GL_ARRAY_BUFFER, b->id);
}

Buffer generateIndexBufferWithData(void* indexData, u32 indexDataSize){
    Buffer b;
    glGenBuffers(1, &b.id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexData, GL_STATIC_DRAW);
    return b;
}

void bindIndexBuffer(Buffer* b){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b->id);
}

Texture generateTextureWidthData(u8* data, u32 width, u32 height){
    Texture t;
    glGenTextures(1, &t.id);
    glBindTexture(GL_TEXTURE_2D, t.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return t;
}

void bindTexture(Texture* t){
    glBindTexture(GL_TEXTURE_2D, t->id);
}

VertexAttributeDescriptor generateVertexAttributeDescriptor(u32 totalAttributes, VertexAttributeType* vaTypes){
    VertexAttributeDescriptor vad;
    glGenVertexArrays(1, &vad.id);
    glBindVertexArray(vad.id);
    u64 offset = 0;
    u64 stride = 0;
    for(int i = 0; i < totalAttributes; i++){
        switch(vaTypes[i]){
            case FLOAT : stride += sizeof(f32); break;
            case FLOAT2 : stride += sizeof(f32) * 2; break;
            case FLOAT3 : stride += sizeof(f32) * 3; break;
            case FLOAT4 : stride += sizeof(f32) * 4; break;
            case MATRIX4: break;
        }
    }

    for(int i = 0; i < totalAttributes; i++){
        switch(vaTypes[i]){
            case FLOAT:{
                u32 sz = sizeof(f32);
                glVertexAttribPointer(i, 1, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
                offset += sz;
                break;
            }
            case FLOAT2:{
                u32 sz = sizeof(f32) * 2;
                glVertexAttribPointer(i, 2, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
                offset += sz;
                break;
            }
            case FLOAT3:{
                u32 sz = sizeof(f32) * 3;
                glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
                offset += sz;
                break;
            }
            case FLOAT4:{
                u32 sz = sizeof(f32) * 4;
                glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
                offset += sz;
                break;
            }
            case MATRIX4:{}
            default:{}
        }

        
        glEnableVertexAttribArray(i);
    }
    return vad;
}

void bindVertexAttributeDescriptor(VertexAttributeDescriptor* vad){
    glBindVertexArray(vad->id);
}

void drawArrays(RenderType renderType, u32 startIndex, u32 totalVertices){
    glDrawArrays(renderType, startIndex, totalVertices);
}

void drawIndices(RenderType renderType, u64 bufferOffset, u32 count, IndexSize indSz){
    glDrawElements(renderType, count, indSz, (const void*)bufferOffset);
}

void initializeGLRenderer(){
    glewInit();
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
}