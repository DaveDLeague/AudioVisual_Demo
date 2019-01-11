#pragma once

void generateCubeVerticesWithTexture(f32** vertices, u32* vertCount, u16** indices, u32* indCount, f32 size = 1){
    *vertCount = 4 * 6;
    *indCount = 6 * 6;
    *vertices = new f32[*vertCount * 5];
    *indices = new u16[*indCount];
    u32 ctr = 0;
    f32 halfSize = size / 2;
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 0; 
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 0; 

    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 0; 
    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 0; 

    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 0; 
    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 0;

    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 0; 
    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 0;

    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 0; 
    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 0;

    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 0; 
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize;
    vertices[0][ctr++] = 0; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 1; 
    vertices[0][ctr++] = halfSize; vertices[0][ctr++] = -halfSize; vertices[0][ctr++] = -halfSize;
    vertices[0][ctr++] = 1; vertices[0][ctr++] = 0;

    ctr = 0;
    indices[0][ctr++] = 0; indices[0][ctr++] = 1; indices[0][ctr++] = 2; 
    indices[0][ctr++] = 2; indices[0][ctr++] = 3; indices[0][ctr++] = 0;
    indices[0][ctr++] = 4; indices[0][ctr++] = 5; indices[0][ctr++] = 6; 
    indices[0][ctr++] = 6; indices[0][ctr++] = 7; indices[0][ctr++] = 4;
    indices[0][ctr++] = 8; indices[0][ctr++] = 9; indices[0][ctr++] = 10; 
    indices[0][ctr++] = 10; indices[0][ctr++] = 11; indices[0][ctr++] = 8;
    indices[0][ctr++] = 12; indices[0][ctr++] = 13; indices[0][ctr++] = 14; 
    indices[0][ctr++] = 14; indices[0][ctr++] = 15; indices[0][ctr++] = 12;
    indices[0][ctr++] = 16; indices[0][ctr++] = 17; indices[0][ctr++] = 18; 
    indices[0][ctr++] = 18; indices[0][ctr++] = 19; indices[0][ctr++] = 16;
    indices[0][ctr++] = 20; indices[0][ctr++] = 21; indices[0][ctr++] = 22; 
    indices[0][ctr++] = 22; indices[0][ctr++] = 23; indices[0][ctr++] = 20;
}