#ifndef TEXTURE_HELPER_H
#define TEXTURE_HELPER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <memory>
//#include <algorithm>
//#include <experimental/filesystem>

#include <glad/glad.h>

#include <Helper_structures.h>

namespace OpenGL_tutorial {

class Texture_helper {
public:
    template<size_t WIDTH, size_t HEIGHT>
        static std::array<float, WIDTH * HEIGHT * 4> texture_pattern_rgba();

    template<size_t WIDTH, size_t HEIGHT>
        static std::array<GLubyte, WIDTH * HEIGHT * 4> checker_pattern();

    static GLuint texture_from_ktx(const std::string&, GLuint = 0);
    static GLuint texture_from_ktx(const char*, GLuint = 0);

    static GLuint load_checker_pattern(GLuint = 0);
    static GLuint load_default_texture(GLuint = 0);
    static GLuint load_texture(const std::string&, GLuint texture = 0);

private:
    Texture_helper() { }

    static constexpr std::size_t identifier_size {12};

    static ktx_header header_;

    static constexpr std::array<unsigned char, identifier_size> identifier_ {
        0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A
    };

    static unsigned int swap32(const unsigned int);
    static unsigned int calculate_stride(unsigned int, unsigned int = 4);
};

constexpr std::array<unsigned char, Texture_helper::identifier_size>
Texture_helper::identifier_;

ktx_header Texture_helper::header_ = ktx_header{};

template<size_t WIDTH, size_t HEIGHT>
std::array<float, WIDTH * HEIGHT * 4> Texture_helper::texture_pattern_rgba() {
    auto tex = std::array<float, WIDTH * HEIGHT * 4>();

    for (size_t y {0}; y < HEIGHT; ++y) {
        for (size_t x {0}; x < WIDTH; ++x) {
            const auto index = (y * WIDTH + x) << 2;
            tex[index] = ((x & y) & 0xFF) / 255.0f;
            tex[index + 1] = ((x | y) & 0xFF) / 255.0f;
            tex[index + 2] = ((x ^ y) & 0xFF) / 255.0f;
            tex[index + 3] = 1.0f;
        }
    }

    return tex;
}

template<size_t WIDTH, size_t HEIGHT>
std::array<GLubyte, WIDTH * HEIGHT * 4> Texture_helper::checker_pattern() {
    auto tex = std::array<GLubyte, WIDTH * HEIGHT * 4>();

    for (size_t y {0}; y < HEIGHT; ++y) {
        for (size_t x {0}; x < WIDTH; ++x) {
            const auto index = (y * WIDTH + x) << 2;
            if (((y + x) % 2) == 0) {
                tex[index] = 0;
                tex[index + 1] = 0;
                tex[index + 2] = 0;
            } else {
                tex[index] = 255;
                tex[index + 1] = 255;
                tex[index + 2] = 255;
            }
            tex[index + 3] = 255;
        }
    }

    return tex;
}

GLuint Texture_helper::load_checker_pattern(GLuint texture) {
    constexpr int texture_width {16};
    constexpr int texture_height {16};

    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureStorage2D(texture, 1, GL_RGB8, texture_width, texture_width);
    glBindTexture(GL_TEXTURE_2D, texture);

    const auto tex = checker_pattern<texture_width, texture_height>();

    glTextureSubImage2D(texture, 0, 0, 0, texture_width, texture_height,
            GL_RGBA, GL_UNSIGNED_BYTE, tex.data());

    return texture;
}

GLuint Texture_helper::load_default_texture(GLuint texture) {
    constexpr int texture_width {256};
    constexpr int texture_height {256};

    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureStorage2D(texture, 1, GL_RGBA32F, texture_width, texture_width);
    glBindTexture(GL_TEXTURE_2D, texture);

    const auto tex = texture_pattern_rgba<texture_width, texture_height>();

    glTextureSubImage2D(texture, 0, 0, 0, texture_width, texture_height,
            GL_RGBA, GL_FLOAT, tex.data());

    return texture;
}

GLuint Texture_helper::load_texture(const std::string& filename,
        GLuint texture) {
    try {
        texture = Texture_helper::texture_from_ktx(filename);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n' << "loading default texture\n";
        texture = Texture_helper::load_default_texture(texture);
    }

    //namespace fs = std::experimental::filesystem;
    //std::string ext {fs::path(filename).extension()};
    //std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

    //if (ext == ".ktx") {
    //    try {
    //        texture = Texture_helper::texture_from_ktx(filename);
    //    } catch (const std::runtime_error &e) {
    //        std::cerr << e.what() << '\n' << "loading default texture\n";
    //        texture = Texture_helper::load_default_texture(texture);
    //    }
    //} else {
    //    std::cerr << "Wrong file extension: loading default texture\n";
    //    texture = Texture_helper::load_default_texture(texture);
    //}

    return texture;
}

GLuint Texture_helper::texture_from_ktx(const std::string& filename,
        GLuint texture) {
    return Texture_helper::texture_from_ktx(filename.c_str(), texture);
}

GLuint Texture_helper::texture_from_ktx(const char* filename, GLuint texture) {
    using namespace std;

    // read the file and use exceptions settings
    ifstream ifs {filename, ios_base::binary};
    if (!ifs)
        throw runtime_error{string{"cannot open file "} + filename};
    ifs.exceptions(ifs.exceptions() | ios_base::badbit);

    if (!ifs.read(reinterpret_cast<char*>(&header_), sizeof(header_)) ||
            !std::equal(identifier_.begin(), identifier_.end(),
                header_.identifier.begin()))
        throw runtime_error{filename + string{": failed to read header"}};

    if(header_.endianness == 0x01020304) {
        // Swap needed
        header_.endianness            = swap32(header_.endianness);
        header_.gltype                = swap32(header_.gltype);
        header_.gltypesize            = swap32(header_.gltypesize);
        header_.glformat              = swap32(header_.glformat);
        header_.glinternalformat      = swap32(header_.glinternalformat);
        header_.glbaseinternalformat  = swap32(header_.glbaseinternalformat);
        header_.pixelwidth            = swap32(header_.pixelwidth);
        header_.pixelheight           = swap32(header_.pixelheight);
        header_.pixeldepth            = swap32(header_.pixeldepth);
        header_.arrayelements         = swap32(header_.arrayelements);
        header_.faces                 = swap32(header_.faces);
        header_.miplevels             = swap32(header_.miplevels);
        header_.keypairbytes          = swap32(header_.keypairbytes);
    } else if (header_.endianness != 0x04030201) {
        throw runtime_error{filename + string{": failed endianness"}};
    }

    GLenum target {GL_NONE};

    // Guess target (texture type)
    if (header_.pixelheight == 0)
    {
        target = (header_.arrayelements == 0) ? GL_TEXTURE_1D :
            GL_TEXTURE_1D_ARRAY;
    }
    else if (header_.pixeldepth == 0)
    {
        if (header_.arrayelements == 0)
            target = (header_.faces == 0) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
        else
            target = (header_.faces == 0) ? GL_TEXTURE_2D_ARRAY :
                GL_TEXTURE_CUBE_MAP_ARRAY;
    }
    else
    {
        target = GL_TEXTURE_3D;
    }

    if (target == GL_NONE || (header_.pixelwidth == 0) ||
            (header_.pixelheight == 0 && header_.pixeldepth != 0))
        throw runtime_error{filename + string{": failed texture type/size"}};

    glCreateTextures(target, 1, &texture);
    glBindTexture(target, texture);

    auto start = ifs.tellg();
    start += static_cast<decltype(start)>(header_.keypairbytes);

    ifs.seekg(0, ios_base::end);
    const auto end = ifs.tellg();
    ifs.seekg(start);

    const auto data_size = end - start;

    const auto data = std::make_unique<char[]>(data_size);
    ifs.read(data.get(), data_size);
    //auto data = std::make_unique<unsigned char[]>(data_size);
    //ifs.read(reinterpret_cast<char*>(data.get()), data_size);

    if (header_.miplevels == 0)
        header_.miplevels = 1;

    switch (target)
    {
        case GL_TEXTURE_1D:
            glTextureStorage1D(texture, header_.miplevels,
                    header_.glinternalformat, header_.pixelwidth);
            glTextureSubImage1D(texture, 0, 0, header_.pixelwidth,
                    header_.glformat, header_.glinternalformat, data.get());
            break;
        case GL_TEXTURE_2D:
            //glTexImage2D(GL_TEXTURE_2D, 0, header_.glinternalformat,
            //header_.pixelwidth, header_.pixelheight, 0, header_.glformat,
            //    header_.gltype, data.get());
            if (header_.gltype == GL_NONE)
            {
                glCompressedTexImage2D(texture, 0, header_.glinternalformat,
                        header_.pixelwidth, header_.pixelheight, 0, 420 * 380 /
                        2, data.get());
            }
            else
            {
                glTextureStorage2D(texture, header_.miplevels,
                        header_.glinternalformat, header_.pixelwidth,
                        header_.pixelheight);
                {
                    auto ptr = data.get();
                    unsigned int height = header_.pixelheight;
                    unsigned int width = header_.pixelwidth;
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                    for (unsigned int i = 0; i < header_.miplevels; i++)
                    {
                        glTextureSubImage2D(texture, i, 0, 0, width, height,
                                header_.glformat, header_.gltype, ptr);
                        ptr += height * calculate_stride(width, 1);
                        height >>= 1;
                        width >>= 1;
                        if (height == 0)
                            height = 1;
                        if (width == 0)
                            width = 1;
                    }
                }
            }
            break;
        case GL_TEXTURE_3D:
            glTextureStorage3D(texture, header_.miplevels,
                    header_.glinternalformat, header_.pixelwidth,
                    header_.pixelheight, header_.pixeldepth);
            glTextureSubImage3D(texture, 0, 0, 0, 0, header_.pixelwidth,
                    header_.pixelheight, header_.pixeldepth, header_.glformat,
                    header_.gltype, data.get());
            break;
        case GL_TEXTURE_1D_ARRAY:
            glTextureStorage2D(texture, header_.miplevels,
                    header_.glinternalformat, header_.pixelwidth,
                    header_.arrayelements);
            glTextureSubImage2D(texture, 0, 0, 0, header_.pixelwidth,
                    header_.arrayelements, header_.glformat, header_.gltype,
                    data.get());
            break;
        case GL_TEXTURE_2D_ARRAY:
            glTextureStorage3D(texture, header_.miplevels,
                    header_.glinternalformat, header_.pixelwidth,
                    header_.pixelheight, header_.arrayelements);
            glTextureSubImage3D(texture, 0, 0, 0, 0, header_.pixelwidth,
                    header_.pixelheight, header_.arrayelements,
                    header_.glformat, header_.gltype, data.get());
            break;
        case GL_TEXTURE_CUBE_MAP:
            glTextureStorage2D(texture, header_.miplevels,
                    header_.glinternalformat, header_.pixelwidth,
                    header_.pixelheight);
            //glTextureSubImage3D(texture, 0, 0, 0, 0, header_.pixelwidth,
            //header_.pixelheight, header_.faces, header_.glformat,
            //    header_.gltype, data.get());
            {
                const auto face_size =
                    calculate_stride(header_.pixelwidth) * header_.pixelheight;

                for (unsigned int i {0}; i < header_.faces; i++)
                {
                    glTextureSubImage2D(texture + i, 0, 0, 0,
                            header_.pixelwidth, header_.pixelheight,
                            header_.glformat, header_.gltype, data.get() +
                            face_size * i);
                }
            }
            break;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            glTextureStorage3D(texture, header_.miplevels,
                    header_.glinternalformat, header_.pixelwidth,
                    header_.pixelheight, header_.arrayelements);
            glTextureSubImage3D(texture, 0, 0, 0, 0, header_.pixelwidth,
                    header_.pixelheight, header_.faces * header_.arrayelements,
                    header_.glformat, header_.gltype, data.get());
            break;
        default: // Should never happen
            throw runtime_error{filename + string{": failed to read data"}};
    }

    if (header_.miplevels == 1)
        glGenerateMipmap(target);

    return texture;
}

unsigned int Texture_helper::swap32(const unsigned int u32)
{
    union
    {
        unsigned int u32;
        unsigned char u8[4];
    } source, target;

    source.u32 = u32;
    target.u8[0] = source.u8[3];
    target.u8[1] = source.u8[2];
    target.u8[2] = source.u8[1];
    target.u8[3] = source.u8[0];

    return target.u32;
}

unsigned int Texture_helper::calculate_stride(const unsigned int width,
        const unsigned int pad) {
    unsigned int channels {0};

    switch (header_.glbaseinternalformat)
    {
        case GL_RED:
            channels = 1;
            break;
        case GL_RG:
            channels = 2;
            break;
        case GL_BGR:
        case GL_RGB:
            channels = 3;
            break;
        case GL_BGRA:
        case GL_RGBA:
            channels = 4;
            break;
    }

    unsigned int stride {header_.gltypesize * channels * width};

    return (stride + (pad - 1)) & ~(pad - 1);
}

}

#endif /* TEXTURE_HELPER_H */

