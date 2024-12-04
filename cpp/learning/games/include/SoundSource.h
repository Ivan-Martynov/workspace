#ifndef H_INCLUDE_SOUNDSOURCE_H_H
#define H_INCLUDE_SOUNDSOURCE_H_H

#include <AL/al.h>

namespace Marvin
{

class SoundSource
{
  public:
    SoundSource()
    {
        alGenSources(1, &m_source);
#if 0
        alSourcef(m_source, AL_PITCH, m_pitch);
        alSourcef(m_source, AL_GAIN, m_gain);
        alSource3f(
            m_source, AL_POSITION, m_position[0], m_position[1], m_position[2]);
        alSource3f(
            m_source, AL_VELOCITY, m_velocity[0], m_velocity[1], m_velocity[2]);
        alSourcei(m_source, AL_LOOPING, m_is_looping);
        alSourcei(m_source, AL_BUFFER, m_buffer);
#endif
    }

    ~SoundSource() { alDeleteSources(1, &m_source); }

    explicit SoundSource(const SoundSource& other) = delete;
    SoundSource& operator=(const SoundSource& other) = delete;

    void play()
    {

        ALint state {};
        alGetSourcei(m_source, AL_SOURCE_STATE, &state);
        std::cout << "State = " << state << "\n";
        alSourcePlay(m_source);
    }

    void play(ALuint buffer)
    {
        if (m_buffer != buffer)
        {
            m_buffer = buffer;
            alSourcei(m_source, AL_BUFFER, m_buffer);
        }
        play();
    }

    void pause() { alSourcePause(m_source); }
    void stop() { alSourceStop(m_source); }

    void set_looping(bool is_looping)
    {
        m_is_looping = is_looping;
        alSourcei(m_source, AL_LOOPING, m_is_looping);
    }

    void set_gain(float gain)
    {
        m_gain = gain;
        alSourcef(m_source, AL_GAIN, m_gain);
    }

  private:
    ALuint m_source {};
    float m_pitch {1.0f};
    float m_gain {1.0f};
    float m_position[3] {0.0f, 0.0f, 0.0f};
    float m_velocity[3] {0.0f, 0.0f, 0.0f};
    bool m_is_looping {false};
    ALuint m_buffer {0};
};

} // namespace Marvin

#endif // H_INCLUDE_SOUNDSOURCE_H_H
