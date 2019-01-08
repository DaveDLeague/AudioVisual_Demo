#define GLEW_STATIC

#include "SDL2/SDL.h"
#include "GL/glew.h"
#include "OpenAL/al.h"
#include "OpenAL/alc.h"

#include <stdio.h>
#include <math.h>

#define PAD 1095761920

struct WaveFile {
    unsigned int chunkID;
    unsigned int chunkSize;
    unsigned int format;
    unsigned int subChunk1ID;
    unsigned int subChunk1Size;
    unsigned short audioFormat;
    unsigned short numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    unsigned int subChunk2ID;
    unsigned int subChunk2Size;
    char* data;
};

void printWaveFile(WaveFile file){
    printf("Chunk ID: %c%c%c%c\n", file.chunkID, file.chunkID >> 8, file.chunkID >> 16, file.chunkID >> 24);
    printf("Chunk Size: %u\n", file.chunkSize);
    printf("Format: %c%c%c%c\n", file.format, file.format >> 8, file.format >> 16, file.format >> 24);
    printf("Sub Chunk 1 ID:  %c%c%c%c\n", file.subChunk1ID, file.subChunk1ID >> 8, file.subChunk1ID >> 16, file.subChunk1ID >> 24);
    printf("Sub Chunk 1 Size: %u\n", file.subChunk1Size);
    printf("Audio Format: %u\n", file.audioFormat);
    printf("Num Channels: %u\n", file.numChannels);
    printf("Sample Rate: %u\n", file.sampleRate);
    printf("Byte Rate: %u\n", file.byteRate);
    printf("Block Align: %u\n", file.blockAlign);
    printf("Bits Per Sample: %u\n", file.bitsPerSample);
    printf("Sub Chunk 2 ID:  %c%c%c%c\n", file.subChunk2ID, file.subChunk2ID >> 8, file.subChunk2ID >> 16, file.subChunk2ID >> 24);
    printf("Sub Chunk 2 Size: %u\n", file.subChunk2Size);

    printf("Data: \n");
    // for(int i = 0; i < file.subChunk2Size; i++){
    //     printf("%i", file.data[i]);
    // }
}

int main(int argc, char** argv){

    SDL_RWops *file = SDL_RWFromFile("../../button.wav", "rb");
    long size = SDL_RWsize(file);
    char* data = new char[size];
    SDL_RWread(file, data, 1, size);
    SDL_RWclose(file);

    char* fileptr = data;
    WaveFile wf;
    wf.chunkID = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.chunkSize = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.format = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.subChunk1ID = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.subChunk1Size = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.audioFormat = *(unsigned short*)fileptr;
    fileptr += 2;

    wf.numChannels = *(unsigned short*)fileptr;
    fileptr += 2;

    wf.sampleRate = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.byteRate = *(unsigned int*)fileptr;
    fileptr += 4;

    wf.blockAlign = *(unsigned short*)fileptr;
    fileptr += 2;

    wf.bitsPerSample = *(unsigned short*)fileptr;
    fileptr += 2;

    wf.subChunk2ID = *(unsigned int*)fileptr;
    fileptr += 4;

    while(wf.subChunk2ID != 'atad'){
        if(wf.subChunk2ID == PAD){
           fileptr += 4;
        }
        unsigned int v = *(unsigned int*)fileptr;
        fileptr += 4;
        printf("v: %u\n", v);
        fileptr += v;
        wf.subChunk2ID = *(unsigned int*)fileptr;
        fileptr += 4;
    }
    

    wf.subChunk2Size = *(unsigned int*)fileptr;
    fileptr += 4;

    char* iptr = fileptr;

    wf.data = new char[wf.subChunk2Size];
    if(wf.bitsPerSample == 24){
        for(int i = 0; i < wf.subChunk2Size / 3; i += 2){
            int num = *(int*)iptr / 256;
            iptr += 3;
            wf.data[i] = num << 8;
            wf.data[i + 1] = num >> 8;
        }
    }else{
        for(int i = 0; i < wf.subChunk2Size; i++){
            wf.data[i] = *fileptr++;
        }
    }

    printWaveFile(wf);

    ALCdevice *device = alcOpenDevice(0);
    ALCcontext *audioContext = alcCreateContext(device, 0);
    alcMakeContextCurrent(audioContext);
    unsigned int buffer, source;
    alGenBuffers(1, &buffer);
    alGenSources(1, &source);

    alBufferData(buffer, AL_FORMAT_STEREO16, wf.data, wf.subChunk2Size, wf.sampleRate);
    alSourcei(source, AL_BUFFER, buffer);

    SDL_Window* window = SDL_CreateWindow("AV DEMO", 100, 100, 500, 500, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewInit();
    glClearColor(0, 1, 0, 1);

    SDL_Event event;
    bool running = true;
    while(running){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_KEYDOWN:{
                    if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                        running = false;
                    }else if(event.key.keysym.scancode == SDL_SCANCODE_RETURN){
                        alSourcePlay(source);
                    }
                } 
                break;
                case SDL_KEYUP: break;
                case SDL_QUIT: running = false;
                break;
            }
        }
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    alSourceStop(source);

    alSourceStop(source);
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcDestroyContext(audioContext);
    alcCloseDevice(device);


    return 0;
}