#ifndef H_INCLUDE_SOUND_DEVICE_H_H
#define H_INCLUDE_SOUND_DEVICE_H_H

#include <AL/alc.h>
#include <iostream>

namespace Marvin
{

class SoundDevice
{
  public:
    static SoundDevice* get() { return new SoundDevice {}; }

  private:
    SoundDevice() : p_ALC_device {alcOpenDevice(nullptr)}, p_ALC_context {}
    {
        if (!p_ALC_device)
        {
            throw std::runtime_error("Failed to get sound device");
        }

        p_ALC_context = alcCreateContext(p_ALC_device, nullptr);
        if (!p_ALC_context)
        {
            throw std::runtime_error("Failed to get sound context");
        }

        if (!alcMakeContextCurrent(p_ALC_context))
        {
            throw std::runtime_error("Failed to make context current");
        }

        const ALCchar* name {nullptr};
        if (alcIsExtensionPresent(p_ALC_device, "ALC_ENUMERATE_ALL_EXT"))
        {
            name = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
        }

        if (!name || (alcGetError(p_ALC_device) != ALC_NO_ERROR))
        {
            name = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
        }

        std::cout << "Opened " << name << "\n";
    }

    SoundDevice(const SoundDevice&) = delete;
    SoundDevice& operator=(const SoundDevice&) = delete;

    ~SoundDevice()
    {
        if (!alcMakeContextCurrent(nullptr))
        {
            std::cerr << "Failed to set context to nullptr\n";
            return;
            // throw std::runtime_error("Failed to set context to nullptr");
        }

        alcDestroyContext(p_ALC_context);

        if (p_ALC_context)
        {
            std::cerr << "Failed to unset during close\n";
            return;
            // throw std::runtime_error("Failed to unset during close");
        }

        if (!alcCloseDevice(p_ALC_device))
        {
            std::cerr << "Failed to close sound device\n";
            // throw std::runtime_error("Failed to close sound device");
        }
    }

    ALCdevice* p_ALC_device;
    ALCcontext* p_ALC_context;
};

} // namespace Marvin

#endif // H_INCLUDE_SOUND_DEVICE_H_H
