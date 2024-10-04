#ifndef HELPER_STRUCTURES_H
#define HELPER_STRUCTURES_H

#include <array>

namespace OpenGL_tutorial {

class Helper_structures {
public:
    static constexpr unsigned int SB6M_FOURCC(const char, const char,
            const char, const char);
private:
    Helper_structures() { };
};

constexpr size_t SB6M_VERTEX_ATTRIB_FLAG_NORMALIZED {0x00000001};
constexpr size_t SB6M_VERTEX_ATTRIB_FLAG_INTEGER {0x00000002};

constexpr unsigned int Helper_structures::SB6M_FOURCC(const char a,
        const char b, const char c, const char d) {
    return
        (static_cast<unsigned int>(a) << 0) |
        (static_cast<unsigned int>(b) << 8) |
        (static_cast<unsigned int>(c) << 16) |
        (static_cast<unsigned int>(d) << 24);
}

struct ktx_header {
    std::array<unsigned char, 12> identifier;
    unsigned int  endianness;
    unsigned int  gltype;
    unsigned int  gltypesize;
    unsigned int  glformat;
    unsigned int  glinternalformat;
    unsigned int  glbaseinternalformat;
    unsigned int  pixelwidth;
    unsigned int  pixelheight;
    unsigned int  pixeldepth;
    unsigned int  arrayelements;
    unsigned int  faces;
    unsigned int  miplevels;
    unsigned int  keypairbytes;
};

enum SB6M_CHUNK_TYPE_t
{
    SB6M_CHUNK_TYPE_INDEX_DATA      =
        Helper_structures::SB6M_FOURCC('I','N','D','X'),
    SB6M_CHUNK_TYPE_VERTEX_DATA     =
        Helper_structures::SB6M_FOURCC('V','R','T','X'),
    SB6M_CHUNK_TYPE_VERTEX_ATTRIBS  =
        Helper_structures::SB6M_FOURCC('A','T','R','B'),
    SB6M_CHUNK_TYPE_SUB_OBJECT_LIST =
        Helper_structures::SB6M_FOURCC('O','L','S','T'),
    SB6M_CHUNK_TYPE_COMMENT         =
        Helper_structures::SB6M_FOURCC('C','M','N','T'),
    SB6M_CHUNK_TYPE_DATA            =
        Helper_structures::SB6M_FOURCC('D','A','T','A')
};

struct SB6M_HEADER_t
{
    union
    {
        unsigned int    magic;
        char            magic_name[4];
    };
    unsigned int        size;
    unsigned int        num_chunks;
    unsigned int        flags;
};

struct SB6M_CHUNK_HEADER_t
{
    union
    {
        unsigned int    chunk_type;
        char            chunk_name[4];
    };
    unsigned int        size;
};

struct SB6M_CHUNK_INDEX_DATA_t
{
    SB6M_CHUNK_HEADER_t header;
    unsigned int        index_type;
    unsigned int        index_count;
    unsigned int        index_data_offset;
};

struct SB6M_CHUNK_VERTEX_DATA_t
{
    SB6M_CHUNK_HEADER_t header;
    unsigned int        data_size;
    unsigned int        data_offset;
    unsigned int        total_vertices;
};

struct SB6M_VERTEX_ATTRIB_DECL_t
{
    char                name[64];
    unsigned int        size;
    unsigned int        type;
    unsigned int        stride;
    unsigned int        flags;
    unsigned int        data_offset;
};

struct SB6M_VERTEX_ATTRIB_CHUNK_t
{
    SB6M_CHUNK_HEADER_t         header;
    unsigned int                attrib_count;
    SB6M_VERTEX_ATTRIB_DECL_t   attrib_data[1];
};

enum SB6M_DATA_ENCODING_t
{
    SB6M_DATA_ENCODING_RAW              = 0
};

struct SB6M_DATA_CHUNK_t
{
    SB6M_CHUNK_HEADER_t         header;
    unsigned int                encoding;
    unsigned int                data_offset;
    unsigned int                data_length;
};

struct SB6M_SUB_OBJECT_DECL_t
{
    unsigned int                first;
    unsigned int                count;
};

struct SB6M_CHUNK_SUB_OBJECT_LIST_t
{
    SB6M_CHUNK_HEADER_t         header;
    unsigned int                count;
    SB6M_SUB_OBJECT_DECL_t      sub_object[1];
};

struct SB6M_CHUNK_COMMENT_t
{
    SB6M_CHUNK_HEADER_t         header;
    char                        comment[1];
};

}

#endif /* HELPER_STRUCTURES_H */

