#include "SDL2/SDL.h"

struct Window {
    void* window;
    void* graphicsContext;
    bool closeRequested;
};

void loadDataFromFile(s8* fileName, s8** data, u64* fileSize){
    SDL_RWops *file = SDL_RWFromFile(fileName, "rb");
    *fileSize = SDL_RWsize(file);
    *data = new s8[*fileSize];
    SDL_RWread(file, *data, 1, *fileSize);
    SDL_RWclose(file);
}

void freeFileData(s8** data){
    if(*data){
        delete[]* data;
        *data = 0;
    }
}

void updateWindowEvents(Window* win){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_KEYDOWN:{
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                    win->closeRequested = true;
                }
            } 
            break;
            case SDL_KEYUP: break;
            case SDL_QUIT: win->closeRequested = true;
            break;
        }
    }
}

void swapWindowBuffer(Window* win){
    SDL_GL_SwapWindow((SDL_Window*)win->window);
}

void deleteWindow(Window* win){
    SDL_GL_DeleteContext(win->graphicsContext);
    SDL_DestroyWindow((SDL_Window*)win->window);
}

Window createWindow(s8* title, u32 startX, u32 startY, u32 width, u32 height){
    Window win;
    win.window = (void*)SDL_CreateWindow("AV DEMO", startX, startY, width, height, SDL_WINDOW_OPENGL);
    win.graphicsContext = SDL_GL_CreateContext((SDL_Window*)win.window);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    win.closeRequested = false;
    return win;
}