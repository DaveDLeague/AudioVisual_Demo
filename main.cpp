#include "utilities.h"
#include "graphics_math.h"
#include "graphics_utilities.h"
#include "os_operations.h"
#include "gl_renderer.h"
#include "primitive3d_renderer.h"
#include "audio_manager.h"

#define WIDTH 1280
#define HEIGHT 720

int main(int argc, char** argv){
    Window win = createWindow("AV Demo", 50, 50, WIDTH, HEIGHT);

    initializeGLRenderer();
    setClearColor(0, 0.5, 1, 1);

    u8 texture[] = {
        0, 0, 255, 255, 255, 0, 0, 255,
        255, 0, 0, 255, 0, 0, 255, 255
    };

    u8 texture2[] = {
        0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255,
        255, 255, 255, 255, 0, 0, 0, 255, 255, 255, 255, 255,
        0, 0, 0, 255, 255, 255, 255, 255, 0, 0, 0, 255
    };

    u32 t3Size = 512 * 512 * 4;
    u8* texture3 = new u8[t3Size];
    u32 state = getCurrentSystemTime();
    for(int i = 0; i < t3Size; i += 4){
        texture3[i] = 0;
        texture3[i + 1] = xorshift32(&state) % 256;
        texture3[i + 2] = 0;
        texture3[i + 3] = 255;
    }

    Texture t = generateTextureWidthData(texture, 2, 2);
    setTextureModeNearest();
    Texture t2 = generateTextureWidthData(texture2, 3, 3);
    setTextureModeNearest();
    Texture t3 = generateTextureWidthData(texture3, 512, 512);
    setTextureModeLinear();
    delete[] texture3;

    Camera camera;
    camera.orientation = quaternion(0, 0, 0, 1);
    camera.forward = vector3(0, 0, 1);
    camera.up = vector3(0, 1, 0);
    camera.right = vector3(1, 0, 0);
    camera.position = vector3(0, -5, -15);
    createPerspectiveProjection(&camera, 70.0, (f32)WIDTH/(f32)HEIGHT, 0.001, 1000.0);

    initializePrimitive3DRenderer();
    Cube cube = generateCube(&t);
    Cube cube2 = generateCube(&t2); 
    cube.position.y += 1;
    cube2.position = vector3(0, 4, 0);

    f32 cameraMoveSpeed = 0.1;
    f32 cameraRotateSpeed = 0.01;

    initializeAudioManager();
    AudioSound as = generateAudioSoundFromWavFile("../../symphony.wav");
    AudioEmitter ae = generateAudioEmitterWithSound(&as);
    setAudioLooping(&ae, true);
    playAudio(&ae);

    while(!win.closeRequested){
        updateWindowEvents(&win);
        if(keyboardInputs[SDL_SCANCODE_ESCAPE]){
            break;
        }
        clearColorAndDepthBuffer(); 
        
        camera.position -= camera.forward * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_W];
        camera.position += camera.forward * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_S];
        camera.position += camera.right * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_A];
        camera.position -= camera.right * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_D];
        camera.position -= camera.up * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_R];
        camera.position += camera.up * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_F];
        rotate(&camera.orientation, camera.up, keyboardInputs[SDL_SCANCODE_RIGHT] * cameraRotateSpeed);
        rotate(&camera.orientation, camera.up, keyboardInputs[SDL_SCANCODE_LEFT] * -cameraRotateSpeed);
        rotate(&camera.orientation, camera.forward, keyboardInputs[SDL_SCANCODE_Q] * cameraRotateSpeed);
        rotate(&camera.orientation, camera.forward, keyboardInputs[SDL_SCANCODE_E] * -cameraRotateSpeed);
        rotate(&camera.orientation, camera.right, keyboardInputs[SDL_SCANCODE_UP] * -cameraRotateSpeed);
        rotate(&camera.orientation, camera.right, keyboardInputs[SDL_SCANCODE_DOWN] * cameraRotateSpeed);

        cube.position.x -= 1;

        updateCameraView(&camera);

        preparePrimitive3DRenderer();
    
        renderCubes(&camera, &cube, 1);
        renderCubes(&camera, &cube2, 1);
        renderGroundPlane(&camera, &t3, 100, 0);
        
        updateAudioEmitterPosition(&ae, &cube.position);
        updateListenerPosition(&camera.position);

        swapWindowBuffer(&win);
    }

    deleteWindow(&win);

    return 0;
}