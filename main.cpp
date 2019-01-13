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
    Cube cubes[2];
    cubes[0] = generateCube(&t);
    cubes[1] = generateCube(&t2); 
    cubes[0].position = vector3(-15, 1, -20);
    cubes[1].position = vector3(-0, 0, -0);

    f32 cameraMoveSpeed = 10;
    f32 cameraRotateSpeed = 1;

    initializeAudioManager();
    AudioSound song = generateAudioSoundFromWavFile("../../symphony.wav");
    AudioSound bounce = generateAudioSoundFromWavFile("../../snare.wav");

    AudioEmitter songEmitter = generateAudioEmitterWithSound(&song);
    AudioEmitter bounceEmitter = generateAudioEmitterWithSound(&bounce);

    setAudioLooping(&songEmitter, true);
    playAudio(&songEmitter);

    updateAudioEmitterPosition(&songEmitter, &cubes[0].position);
    updateAudioEmitterPosition(&bounceEmitter, &cubes[1].position);
    updateListenerPosition(&camera.position);

    f32 deltaTime;
    u64 startTime = getCurrentSystemTime();
    u64 endTime;

    f32 yVelocity = 0;
    f32 gravity = 0.25;

    while(!win.closeRequested){
        updateWindowEvents(&win);
        if(keyboardInputs[SDL_SCANCODE_ESCAPE]){
            break;
        }
        clearColorAndDepthBuffer(); 
        
        camera.position -= camera.forward * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_W] * deltaTime;
        camera.position += camera.forward * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_S] * deltaTime;
        camera.position += camera.right * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_A] * deltaTime;
        camera.position -= camera.right * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_D] * deltaTime;
        camera.position -= camera.up * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_R] * deltaTime;
        camera.position += camera.up * cameraMoveSpeed * keyboardInputs[SDL_SCANCODE_F] * deltaTime;
        rotate(&camera.orientation, camera.up, keyboardInputs[SDL_SCANCODE_RIGHT] * cameraRotateSpeed * deltaTime);
        rotate(&camera.orientation, camera.up, keyboardInputs[SDL_SCANCODE_LEFT] * -cameraRotateSpeed * deltaTime);
        rotate(&camera.orientation, camera.forward, keyboardInputs[SDL_SCANCODE_Q] * cameraRotateSpeed * deltaTime);
        rotate(&camera.orientation, camera.forward, keyboardInputs[SDL_SCANCODE_E] * -cameraRotateSpeed * deltaTime);
        rotate(&camera.orientation, camera.right, keyboardInputs[SDL_SCANCODE_UP] * -cameraRotateSpeed * deltaTime);
        rotate(&camera.orientation, camera.right, keyboardInputs[SDL_SCANCODE_DOWN] * cameraRotateSpeed * deltaTime);

        cubes[1].position.y += yVelocity * deltaTime;
        yVelocity -= gravity;
        if(cubes[1].position.y <= 1){
            cubes[1].position.y = 1;
            updateAudioEmitterPosition(&bounceEmitter, &cubes[1].position);
            playAudio(&bounceEmitter);
            yVelocity = 100;
        }


        updateCameraView(&camera);

        preparePrimitive3DRenderer();
    
        renderCubes(&camera, cubes, 2);
        renderGroundPlane(&camera, &t3, 100, 0);
        
        updateAudioEmitterPosition(&songEmitter, &cubes[0].position);
        updateListenerPosition(&camera.position);

        swapWindowBuffer(&win);
        endTime = getCurrentSystemTime();
        deltaTime = ((f64)endTime - (f64)startTime) / 1000.0;
        startTime = endTime;
    }

    deleteWindow(&win);

    return 0;
}