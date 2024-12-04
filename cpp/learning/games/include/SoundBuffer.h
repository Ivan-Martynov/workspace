#ifndef H_INCLUDE_SOUNDBUFFER_H_H
#define H_INCLUDE_SOUNDBUFFER_H_H

#include <vector>
#include <sndfile.h>
#include <inttypes.h>
#include <AL/alext.h>
#include <AL/al.h>
#include <iostream>
#include <cstdlib>
#include <limits.h>

namespace Marvin
{

class SoundBuffer
{
  public:
    static SoundBuffer* get() { return new SoundBuffer {}; }

    ALuint add_sound_effect(const char* filepath)
    {
        enum FormatType
        {
            Int16,
            Float,
            IMA4,
            MSADPCM
        };

        /* Open the audio file and check that it's usable. */
        // SNDFILE* sndfile;
        SF_INFO sfinfo {};
        auto sndfile {sf_open(filepath, SFM_READ, &sfinfo)};
        if (!sndfile)
        {
            std::cerr << "Could not open audio in " << filepath << ": "
                      << sf_strerror(sndfile) << "\n";
            return 0;
        }
        if (sfinfo.frames < 1)
        {
            std::cerr << "Bad sample count in " << filepath << " (%"
                      << sfinfo.frames << ")\n";
            sf_close(sndfile);
            return 0;
        }

        /* Detect a suitable format to load. Formats like Vorbis and Opus use
         * float natively, so load as float to avoid clipping when possible.
         * Formats larger than 16-bit can also use float to preserve a bit more
         * precision.
         */
        auto sample_format {Int16};
        switch ((sfinfo.format & SF_FORMAT_SUBMASK))
        {
            case SF_FORMAT_PCM_24:
            case SF_FORMAT_PCM_32:
            case SF_FORMAT_FLOAT:
            case SF_FORMAT_DOUBLE:
            case SF_FORMAT_VORBIS:
            case SF_FORMAT_OPUS:
            case SF_FORMAT_ALAC_20:
            case SF_FORMAT_ALAC_24:
            case SF_FORMAT_ALAC_32:
            case 0x0080 /*SF_FORMAT_MPEG_LAYER_I*/:
            case 0x0081 /*SF_FORMAT_MPEG_LAYER_II*/:
            case 0x0082 /*SF_FORMAT_MPEG_LAYER_III*/:
                if (alIsExtensionPresent("AL_EXT_FLOAT32"))
                {
                    sample_format = Float;
                }
                break;

            case SF_FORMAT_IMA_ADPCM:
                /* ADPCM formats require setting a block alignment as specified
                 * in the file, which needs to be read from the wave 'fmt '
                 * chunk manually since libsndfile doesn't provide it in a
                 * format-agnostic way.
                 */
                if ((sfinfo.channels <= 2)
                    && ((sfinfo.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV)
                    && alIsExtensionPresent("AL_EXT_IMA4")
                    && alIsExtensionPresent("AL_SOFT_block_alignment"))
                {
                    sample_format = IMA4;
                }
                break;

            case SF_FORMAT_MS_ADPCM:
                if ((sfinfo.channels <= 2)
                    && ((sfinfo.format & SF_FORMAT_TYPEMASK) == SF_FORMAT_WAV)
                    && alIsExtensionPresent("AL_SOFT_MSADPCM")
                    && alIsExtensionPresent("AL_SOFT_block_alignment"))
                {
                    sample_format = MSADPCM;
                }
                break;
        }

        ALint byteblockalign {0};
        ALint splblockalign {0};
        if ((sample_format == IMA4) || (sample_format == MSADPCM))
        {
            /* For ADPCM, lookup the wave file's "fmt " chunk, which is a
             * WAVEFORMATEX-based structure for the audio format.
             */
            SF_CHUNK_INFO inf {"fmt ", 4, 0, nullptr};
            auto* iter {sf_get_chunk_iterator(sndfile, &inf)};

            /* If there's an issue getting the chunk or block alignment, load as
             * 16-bit and have libsndfile do the conversion.
             */
            if (!iter || (sf_get_chunk_size(iter, &inf) != SF_ERR_NO_ERROR)
                || (inf.datalen < 14))
            {
                sample_format = Int16;
            }
            else
            {
                const auto fmtbuf {
                    static_cast<ALubyte*>(std::calloc(inf.datalen, 1))};
                inf.data = fmtbuf;
                if (sf_get_chunk_data(iter, &inf) != SF_ERR_NO_ERROR)
                {
                    sample_format = Int16;
                }
                else
                {
                    /* Read the nBlockAlign field, and convert from bytes- to
                     * samples-per-block (verifying it's valid by converting
                     * back and comparing to the original value).
                     */
                    byteblockalign = fmtbuf[12] | (fmtbuf[13] << 8);
                    if (sample_format == IMA4)
                    {
                        splblockalign
                            = (byteblockalign / sfinfo.channels - 4) / 4 * 8
                              + 1;
                        if ((splblockalign < 1)
                            || (((splblockalign - 1) / 2 + 4) * sfinfo.channels
                                != byteblockalign))
                        {
                            sample_format = Int16;
                        }
                    }
                    else
                    {
                        splblockalign
                            = (byteblockalign / sfinfo.channels - 7) * 2 + 2;
                        if ((splblockalign < 2)
                            || (((splblockalign - 2) / 2 + 7) * sfinfo.channels
                                != byteblockalign))
                        {
                            sample_format = Int16;
                        }
                    }
                }
                free(fmtbuf);
            }
        }

        if (sample_format == Int16)
        {
            splblockalign = 1;
            byteblockalign = sfinfo.channels * 2;
        }
        else if (sample_format == Float)
        {
            splblockalign = 1;
            byteblockalign = sfinfo.channels * 4;
        }

        /* Figure out the OpenAL format from the file and desired sample type.
         */
        ALenum format {AL_NONE};
        if (sfinfo.channels == 1)
        {
            if (sample_format == Int16)
            {
                format = AL_FORMAT_MONO16;
            }
            else if (sample_format == Float)
            {
                format = AL_FORMAT_MONO_FLOAT32;
            }
            else if (sample_format == IMA4)
            {
                format = AL_FORMAT_MONO_IMA4;
            }
            else if (sample_format == MSADPCM)
            {
                format = AL_FORMAT_MONO_MSADPCM_SOFT;
            }
        }
        else if (sfinfo.channels == 2)
        {
            if (sample_format == Int16)
            {
                format = AL_FORMAT_STEREO16;
            }
            else if (sample_format == Float)
            {
                format = AL_FORMAT_STEREO_FLOAT32;
            }
            else if (sample_format == IMA4)
            {
                format = AL_FORMAT_STEREO_IMA4;
            }
            else if (sample_format == MSADPCM)
            {
                format = AL_FORMAT_STEREO_MSADPCM_SOFT;
            }
        }
        else if (sfinfo.channels == 3)
        {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0)
                == SF_AMBISONIC_B_FORMAT)
            {
                if (sample_format == Int16)
                {
                    format = AL_FORMAT_BFORMAT2D_16;
                }
                else if (sample_format == Float)
                {
                    format = AL_FORMAT_BFORMAT2D_FLOAT32;
                }
            }
        }
        else if (sfinfo.channels == 4)
        {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, nullptr, 0)
                == SF_AMBISONIC_B_FORMAT)
            {
                if (sample_format == Int16)
                {
                    format = AL_FORMAT_BFORMAT3D_16;
                }
                else if (sample_format == Float)
                {
                    format = AL_FORMAT_BFORMAT3D_FLOAT32;
                }
            }
        }
        if (!format)
        {
            std::cerr << "Unsupported channel count:" << sfinfo.channels
                      << "\n";
            sf_close(sndfile);
            return 0;
        }

        if (sfinfo.frames / splblockalign
            > static_cast<sf_count_t>(INT_MAX / byteblockalign))
        {
            std::cerr << "Too many samples in " << filepath << " (%"
                      << sfinfo.frames << ")\n";
            sf_close(sndfile);
            return 0;
        }

        /* Decode the whole audio file to a buffer. */
        auto membuf {static_cast<short*>(
            malloc(sfinfo.frames / splblockalign * byteblockalign))};

        sf_count_t num_frames {};
        if (sample_format == Int16)
        {
            num_frames = sf_readf_short(
                sndfile, static_cast<short*>(membuf), sfinfo.frames);
        }
        else if (sample_format == Float)
        {
            num_frames = sf_readf_float(
                sndfile, reinterpret_cast<float*>(membuf), sfinfo.frames);
        }
        else
        {
            sf_count_t count = sfinfo.frames / splblockalign * byteblockalign;
            num_frames = sf_read_raw(sndfile, membuf, count);
            if (num_frames > 0)
            {
                num_frames = num_frames / byteblockalign * splblockalign;
            }
        }
        if (num_frames < 1)
        {
            free(membuf);
            sf_close(sndfile);
            std::cerr << "Failed to read samples in " << filepath << " (%"
                      << num_frames << ")\n";
            return 0;
        }

        // const char *FormatName(ALenum format)
        auto FormatName_lambda {[](ALenum name_format)
            {
                switch (name_format)
                {
                    case AL_FORMAT_MONO8:
                        return "Mono, U8";
                    case AL_FORMAT_MONO16:
                        return "Mono, S16";
                    case AL_FORMAT_MONO_FLOAT32:
                        return "Mono, Float32";
                    case AL_FORMAT_MONO_MULAW:
                        return "Mono, muLaw";
                    case AL_FORMAT_MONO_ALAW_EXT:
                        return "Mono, aLaw";
                    case AL_FORMAT_MONO_IMA4:
                        return "Mono, IMA4 ADPCM";
                    case AL_FORMAT_MONO_MSADPCM_SOFT:
                        return "Mono, MS ADPCM";
                    case AL_FORMAT_STEREO8:
                        return "Stereo, U8";
                    case AL_FORMAT_STEREO16:
                        return "Stereo, S16";
                    case AL_FORMAT_STEREO_FLOAT32:
                        return "Stereo, Float32";
                    case AL_FORMAT_STEREO_MULAW:
                        return "Stereo, muLaw";
                    case AL_FORMAT_STEREO_ALAW_EXT:
                        return "Stereo, aLaw";
                    case AL_FORMAT_STEREO_IMA4:
                        return "Stereo, IMA4 ADPCM";
                    case AL_FORMAT_STEREO_MSADPCM_SOFT:
                        return "Stereo, MS ADPCM";
                    case AL_FORMAT_QUAD8:
                        return "Quadraphonic, U8";
                    case AL_FORMAT_QUAD16:
                        return "Quadraphonic, S16";
                    case AL_FORMAT_QUAD32:
                        return "Quadraphonic, Float32";
                    case AL_FORMAT_QUAD_MULAW:
                        return "Quadraphonic, muLaw";
                    case AL_FORMAT_51CHN8:
                        return "5.1 Surround, U8";
                    case AL_FORMAT_51CHN16:
                        return "5.1 Surround, S16";
                    case AL_FORMAT_51CHN32:
                        return "5.1 Surround, Float32";
                    case AL_FORMAT_51CHN_MULAW:
                        return "5.1 Surround, muLaw";
                    case AL_FORMAT_61CHN8:
                        return "6.1 Surround, U8";
                    case AL_FORMAT_61CHN16:
                        return "6.1 Surround, S16";
                    case AL_FORMAT_61CHN32:
                        return "6.1 Surround, Float32";
                    case AL_FORMAT_61CHN_MULAW:
                        return "6.1 Surround, muLaw";
                    case AL_FORMAT_71CHN8:
                        return "7.1 Surround, U8";
                    case AL_FORMAT_71CHN16:
                        return "7.1 Surround, S16";
                    case AL_FORMAT_71CHN32:
                        return "7.1 Surround, Float32";
                    case AL_FORMAT_71CHN_MULAW:
                        return "7.1 Surround, muLaw";
                    case AL_FORMAT_BFORMAT2D_8:
                        return "B-Format 2D, U8";
                    case AL_FORMAT_BFORMAT2D_16:
                        return "B-Format 2D, S16";
                    case AL_FORMAT_BFORMAT2D_FLOAT32:
                        return "B-Format 2D, Float32";
                    case AL_FORMAT_BFORMAT2D_MULAW:
                        return "B-Format 2D, muLaw";
                    case AL_FORMAT_BFORMAT3D_8:
                        return "B-Format 3D, U8";
                    case AL_FORMAT_BFORMAT3D_16:
                        return "B-Format 3D, S16";
                    case AL_FORMAT_BFORMAT3D_FLOAT32:
                        return "B-Format 3D, Float32";
                    case AL_FORMAT_BFORMAT3D_MULAW:
                        return "B-Format 3D, muLaw";
                    case AL_FORMAT_UHJ2CHN8_SOFT:
                        return "UHJ 2-channel, U8";
                    case AL_FORMAT_UHJ2CHN16_SOFT:
                        return "UHJ 2-channel, S16";
                    case AL_FORMAT_UHJ2CHN_FLOAT32_SOFT:
                        return "UHJ 2-channel, Float32";
                    case AL_FORMAT_UHJ3CHN8_SOFT:
                        return "UHJ 3-channel, U8";
                    case AL_FORMAT_UHJ3CHN16_SOFT:
                        return "UHJ 3-channel, S16";
                    case AL_FORMAT_UHJ3CHN_FLOAT32_SOFT:
                        return "UHJ 3-channel, Float32";
                    case AL_FORMAT_UHJ4CHN8_SOFT:
                        return "UHJ 4-channel, U8";
                    case AL_FORMAT_UHJ4CHN16_SOFT:
                        return "UHJ 4-channel, S16";
                    case AL_FORMAT_UHJ4CHN_FLOAT32_SOFT:
                        return "UHJ 4-channel, Float32";
                }
                return "Unknown Format";
            }};

        const auto num_bytes {
            static_cast<ALsizei>(num_frames / splblockalign * byteblockalign)};
        std::cout << "Loading: " << filepath << " (" << FormatName_lambda(format)
                  << ", " << sfinfo.samplerate << "hz)\n";
        // printf("Loading: %s (%s, %dhz)\n", filepath, FormatName(format),
        //     sfinfo.samplerate);
        // fflush(stdout);

        /* Buffer the audio data into a new buffer object, then free the data
         * and close the file.
         */
        ALuint buffer {0};
        alGenBuffers(1, &buffer);
        if (splblockalign > 1)
        {
            alBufferi(buffer, AL_UNPACK_BLOCK_ALIGNMENT_SOFT, splblockalign);
        }
        alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

        free(membuf);
        sf_close(sndfile);

        /* Check if an error occurred, and clean up if so. */
        // auto err {alGetError()};
        if (const auto err {alGetError()}; err != AL_NO_ERROR)
        {
            std::cerr << "OpenAL Error: " << err << ": " << alGetString(err)
                      << "\n";
            if (buffer && alIsBuffer(buffer))
            {
                alDeleteBuffers(1, &buffer);
            }
            return 0;
        }

        m_sound_effect_buffers.push_back(buffer);

        return buffer;
    }

    bool remove_sound_effect(ALuint buffer)
    {
        for (auto it {m_sound_effect_buffers.begin()};
             it != m_sound_effect_buffers.end(); ++it)
        {
            if (*it == buffer)
            {
                alDeleteBuffers(1, &*it);
                m_sound_effect_buffers.erase(it);
                return true;
            }
        }
        return false;
    }

  private:
    std::vector<ALuint> m_sound_effect_buffers;

    SoundBuffer() : m_sound_effect_buffers {} {}

    SoundBuffer(const SoundBuffer&) = delete;
    SoundBuffer& operator=(const SoundBuffer&) = delete;

    ~SoundBuffer()
    {
        alDeleteBuffers(
            m_sound_effect_buffers.size(), m_sound_effect_buffers.data());
    }
};

} // namespace Marvin

#endif // H_INCLUDE_SOUNDBUFFER_H_H
