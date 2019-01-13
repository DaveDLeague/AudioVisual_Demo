#pragma once

#include "OpenAL/al.h"
#include "OpenAL/alc.h"

#include "wav_importer.h"

struct AudioSound {
    u32 bufferId;
};

struct AudioEmitter {
    u32 sourceId;
    AudioSound* sound;
};

static ALCdevice* audioManagerDevice;
static ALCcontext* audioManagerContext;

AudioEmitter generateAudioEmitterWithSound(AudioSound* sound){
    AudioEmitter ae;
    alGenSources(1, &ae.sourceId);
    alSourcei(ae.sourceId, AL_BUFFER, sound->bufferId);
    ae.sound = sound;
    return ae;
}

AudioSound generateAudioSoundFromWavFile(const s8* filename){
    AudioSound as;
    alGenBuffers(1, &as.bufferId);
    s8* fileData;
    u64 fileSize;
    loadDataFromFile(filename, &fileData, &fileSize);
    WavFile wf = loadWavFileData(fileData);
    convertStereoToMonoWav(&wf);

    if(wf.numChannels == 1){
        if(wf.bitsPerSample == 8){
            alBufferData(as.bufferId, AL_FORMAT_MONO8, wf.data, wf.subChunk2Size, wf.sampleRate);
        }else{
            alBufferData(as.bufferId, AL_FORMAT_MONO16, wf.data, wf.subChunk2Size, wf.sampleRate);
        }
    }else if(wf.numChannels == 2){
        if(wf.bitsPerSample == 8){
            alBufferData(as.bufferId, AL_FORMAT_STEREO8, wf.data, wf.subChunk2Size, wf.sampleRate);
        }else if(wf.bitsPerSample == 24){
            alBufferData(as.bufferId, AL_FORMAT_STEREO16, wf.data, ((f32)wf.subChunk2Size * 2.0) / 3.0, wf.sampleRate);
        }else{
            alBufferData(as.bufferId, AL_FORMAT_STEREO16, wf.data, wf.subChunk2Size, wf.sampleRate);
        }
    }
    
    freeWavFileData(wf);

    return as;
}

void updateAudioEmitterPosition(AudioEmitter* ae, Vector3* position){
    alSource3f(ae->sourceId, AL_POSITION, -position->x, -position->y, -position->z);
}

void setAudioLooping(AudioEmitter* ae, bool looping){
    alSourcei(ae->sourceId, AL_LOOPING, looping);
}

void playAudio(AudioEmitter* ae){
    alSourcePlay(ae->sourceId);
}

void stopAudio(AudioEmitter* ae){
    alSourceStop(ae->sourceId);
}

void updateListenerPosition(Vector3* position){
    alListener3f(AL_POSITION, position->x, position->y, position->z);
}

void updateListenerVelocity(Vector3* velocity){
    alListener3f(AL_VELOCITY, velocity->x, velocity->y, velocity->z);
}

void initializeAudioManager(){
    audioManagerDevice = alcOpenDevice(0);
    audioManagerContext = alcCreateContext(audioManagerDevice, 0);
    alcMakeContextCurrent(audioManagerContext);
}