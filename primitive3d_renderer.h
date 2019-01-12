#pragma once

struct Cube {
    Vector3 position;
    Vector3 scale;
    Quaternion orientation;
    Texture* texture;
};

static const s8* primitive3DVertexShader = " \
#version 410 core\n \
layout(location = 0) in vec3 position; \
layout(location = 1) in vec2 texCoords; \
layout(location = 2) in vec3 normalCoords; \
out vec2 tCrds; \
uniform mat4 projectionMatrix; \
uniform mat4 modelMatrix; \
void main(){ \
    tCrds = texCoords; \n\
    gl_Position = projectionMatrix * modelMatrix * vec4(position, 1); \
} \
";

static const s8* primitive3DFragmentShader = " \
#version 410 core\n \
out vec4 pixelColor; \
in vec2 tCrds; \
uniform sampler2D sampler; \
void main(){ \
    vec4 color = texture(sampler, tCrds);\n \
    pixelColor = color; \n\
} \
";

static const u32 CUBE_INDEX_BUFFER_START = 0;
static const u32 CUBE_INDEX_COUNT = 36;
static const u32 PLANE_INDEX_BUFFER_START = sizeof(u16) * CUBE_INDEX_COUNT;
static const u32 PLANE_INDEX_COUNT = 6;

static s32 primitive3DProjectionViewMatrixId;
static s32 primitive3DModelMatrixId;

static Buffer primitive3DVertexBuffer;
static Buffer primitive3DIndexBuffer;
static VertexAttributeDescriptor primitive3DVertexAttribDesc;
static Shader primitive3DShaderProgram;

Cube generateCube(Texture* tex, u32 size = 1){
    Cube c = {};
    c.scale = vector3(size, size, size);
    c.orientation = quaternion(0, 0, 0, 1);
    c.texture = tex;
    return c;
}

void renderCubes(Camera* cam, Cube* cubes, u32 totalCubes){
    setUniformValueMatrix4(primitive3DProjectionViewMatrixId, &cam->view);

    for(u32 i = 0; i < totalCubes; i++){
        bindTexture(cubes[i].texture);
        
        Matrix4 model = generateIdentityMatrix();

        translate(&model, &cubes[i].position);

        setUniformValueMatrix4(primitive3DModelMatrixId, &model);

        drawIndices(TRIANGLES, CUBE_INDEX_BUFFER_START, CUBE_INDEX_COUNT, U16);
    }
}

void renderGroundPlane(Camera* cam, Texture* texture, f32 size, f32 elevation){
    setUniformValueMatrix4(primitive3DProjectionViewMatrixId, &cam->view);

    bindTexture(texture);
    Matrix4 model = generateIdentityMatrix();
    Vector3 pos = vector3(0, elevation, 0);
    Vector3 sca = vector3(size, size, size);
    translate(&model, &pos);
    scale(&model, &sca);
    setUniformValueMatrix4(primitive3DModelMatrixId, &model);
    drawIndices(TRIANGLES, PLANE_INDEX_BUFFER_START, PLANE_INDEX_COUNT, U16);
}

void preparePrimitive3DRenderer(){
    setBackfaceCulling(true);
    setDepthTesting(true);
    bindShader(&primitive3DShaderProgram);
    bindVertexAttributeDescriptor(&primitive3DVertexAttribDesc);
    bindVertexBuffer(&primitive3DVertexBuffer);
    bindIndexBuffer(&primitive3DIndexBuffer);
}

void initializePrimitive3DRenderer(){
    f32 cubeVertices[28 * 8] = {
        -1, -1, 1,      0, 0,       0, 0, 1,
        -1,  1, 1,      0, 1,       0, 0, 1,
         1,  1, 1,      1, 1,       0, 0, 1,
         1, -1, 1,      1, 0,       0, 0, 1,

        -1, -1, -1,     0, 0,      -1, 0, 0,
        -1,  1, -1,     0, 1,      -1, 0, 0,
        -1,  1,  1,     1, 1,      -1, 0, 0,
        -1, -1,  1,     1, 0,      -1, 0, 0,

         1, -1, -1,     0, 0,       0, 0, -1,
         1,  1, -1,     0, 1,       0, 0, -1,
        -1,  1, -1,     1, 1,       0, 0, -1,
        -1, -1, -1,     1, 0,       0, 0, -1,

         1, -1,  1,     0, 0,       1, 0, 0,
         1,  1,  1,     0, 1,       1, 0, 0,
         1,  1, -1,     1, 1,       1, 0, 0,
         1, -1, -1,     1, 0,       1, 0, 0,

        -1, 1,  1,      0, 0,       0, 1, 0,
        -1, 1, -1,      0, 1,       0, 1, 0,
         1, 1, -1,      1, 1,       0, 1, 0,
         1, 1,  1,      1, 0,       0, 1, 0,

        -1, -1, -1,     0, 0,       0, -1, 0,
        -1, -1,  1,     0, 1,       0, -1, 0,
         1, -1,  1,     1, 1,       0, -1, 0,
         1, -1, -1,     1, 0,       0, -1, 0,

        -1, 0,  1,       0, 0,       0, 1, 0,
        -1, 0, -1,       0, 1,       0, 1, 0,
         1, 0, -1,       1, 1,       0, 1, 0,
         1, 0,  1,       1, 0,       0, 1, 0
    }; 
    
    u16 cubeIndices[CUBE_INDEX_COUNT + PLANE_INDEX_COUNT] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20,
        24, 25, 26, 26, 27, 24,
    };

    primitive3DVertexBuffer = generateVertexBufferWithData(cubeVertices, sizeof(cubeVertices));
    VertexAttributeType vats[] = { FLOAT3, FLOAT2, FLOAT3 };
    primitive3DVertexAttribDesc = generateVertexAttributeDescriptor(3, vats);
    primitive3DIndexBuffer = generateIndexBufferWithData(cubeIndices, sizeof(cubeIndices));
    primitive3DShaderProgram = compileShaderVF(primitive3DVertexShader, primitive3DFragmentShader);

    primitive3DProjectionViewMatrixId = getUniformId(&primitive3DShaderProgram, "projectionMatrix");
    primitive3DModelMatrixId = getUniformId(&primitive3DShaderProgram, "modelMatrix");
}