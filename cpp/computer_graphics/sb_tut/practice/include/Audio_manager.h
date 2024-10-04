#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <iostream>
#include <cstring>
#include <AL/al.h>
#include <AL/alc.h>

#include <audio/wave.h>

namespace OpenGL_tutorial {

class Audio_manager {
public:
    Audio_manager() = default;

    explicit Audio_manager(const Audio_manager&) = default;
    Audio_manager& operator=(const Audio_manager&) = default;
    explicit Audio_manager(Audio_manager&&) = default;
    Audio_manager& operator=(Audio_manager&&) = default;

    ~Audio_manager();

    void init_default();

    void test();

private:
    ALCdevice * device_ {nullptr};
    ALCcontext * context_ {nullptr};
    ALuint source_ {};
    ALuint buffer_ {};

    void list_audio_devices(const ALchar*);

    ALenum to_al_format(short, short);
};

void Audio_manager::list_audio_devices(const ALchar * devices) {
    const auto * device = devices;
    const auto * next = devices + 1;

    auto len = 0;

    std::cout << "List of devices:\n";

    while (device && *device != '\0' && next && *next != '\0') {
        std::cout << device << '\n';

        len += std::strlen(device);

        device += len + 1;

        next += len + 2;
    }
}

Audio_manager::~Audio_manager() {
    alDeleteSources(1, &source_);

    device_ = alcGetContextsDevice(context_);

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context_);
    alcCloseDevice(device_);
}

void Audio_manager::init_default() {
    device_ = alcOpenDevice(nullptr);

    if (!device_) {
        std::cerr << "AL Error: unable to open audio device\n";
        return;
    }

    context_ = alcCreateContext(device_, nullptr);

    if (!alcMakeContextCurrent(context_)) {
        std::cerr << "AL Error: unable to make audio context\n";
        return;
    }

    list_audio_devices(alcGetString(nullptr, ALC_DEVICE_SPECIFIER));

    //alGetError();
    auto error = alGetError();

    if (error != AL_NO_ERROR) {
        std::cerr << "AL Error:\t" << error << '\n';
        return;
    }

    alGenSources(1, &source_);

    if (error != AL_NO_ERROR) {
        std::cerr << "AL Error:\t" << error << '\n';
        return;
    }

    alGenBuffers(1, &buffer_);

    if (error != AL_NO_ERROR) {
        std::cerr << "AL Error:\t" << error << '\n';
        return;
    }
}

ALenum Audio_manager::to_al_format(short channels, short samples) {
    bool stereo = (channels > 1);

    switch (samples) {
        case 16:
            if (stereo)
                return AL_FORMAT_STEREO16;
            else
                return AL_FORMAT_MONO16;
        case 8:
            if (stereo)
                return AL_FORMAT_STEREO8;
            else
                return AL_FORMAT_MONO8;
        default:
            return -1;
    }
}

void Audio_manager::test() {
    alSourcef(source_, AL_PITCH, 1);
    alSourcef(source_, AL_GAIN, 1);
    alSource3f(source_, AL_POSITION, 0, 0 ,0);
    alSource3f(source_, AL_VELOCITY, 0, 0 ,0);
    alSourcei(source_, AL_LOOPING, AL_FALSE);

    auto * wave = WaveOpenFileForReading("../sound/bleep.wav");

    if (!wave) {
        std::cerr << "Failed to read wave file\n";
        return;
    }

    auto ret = WaveSeekFile(0, wave);

    if (ret) {
        std::cerr << "Failed to seek wave file\n";
        return;
    }

    auto * buffer_data = malloc(wave->dataSize);
    if (!buffer_data) {
        std::cerr << "Failed to malloc wave file\n";
        return;
    }

    ret = WaveReadFile((char*)buffer_data, wave->dataSize, wave);

    if (ret != (int)wave->dataSize) {
        std::cerr << "Failed to waveread wave file\n";
        return;
    }

    alBufferData(buffer_, to_al_format(wave->channels, wave->bitsPerSample),
                buffer_data, wave->dataSize, wave->sampleRate);

    alSourcei(source_, AL_BUFFER, buffer_);

    auto error = alGetError();
    if (error != AL_NO_ERROR) {
        std::cerr << "AL Error:\t" << error << '\n';
        return;
    }

    alSourcePlay(source_);

    ALenum source_state;
    alGetSourcei(source_, AL_SOURCE_STATE, &source_state);

    while (source_state == AL_PLAYING) {
        alGetSourcei(source_, AL_SOURCE_STATE, &source_state);
    }
}

}

#endif /* AUDIO_MANAGER_H */

