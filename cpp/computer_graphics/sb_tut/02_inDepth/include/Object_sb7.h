#ifndef OBJECT_SB7_H
#define OBJECT_SB7_H

#include <fstream>
#include <memory>
#include <glad/glad.h>
#include <Helper_structures.h>

namespace OpenGL_tutorial {

class Object_sb7 {
public:
    Object_sb7() { }

    inline void render(unsigned int instance_count = 1,
            unsigned int base_instance = 0)
    {
        render_sub_object(0, instance_count, base_instance);
    }

    void render_sub_object(const size_t, const size_t = 1, const size_t = 0);

    void get_sub_object_info(unsigned int index, GLuint &first, GLuint &count)
    {
        if (index >= num_sub_objects_)
        {
            first = 0;
            count = 0;
        }
        else
        {
            first = sub_object[index].first;
            count = sub_object[index].count;
        }
    }

    constexpr unsigned int get_sub_object_count() const {
        return num_sub_objects_;
    }

    constexpr GLuint get_vao() const { return vao_; }

    void load(const char*);
    void load(const std::string&);
    void free();
private:

    GLuint                  data_buffer_ {0};
    GLuint                  vao_ {0};
    GLuint                  index_type_ {0};
    GLuint                  index_offset_ {};

    static constexpr size_t max_sub_objects_ {256};

    unsigned int            num_sub_objects_ {};
    //SB6M_SUB_OBJECT_DECL_t sub_object[max_sub_objects_] {};
    std::array<SB6M_SUB_OBJECT_DECL_t, max_sub_objects_> sub_object {};
};

void Object_sb7::load(const std::string& filename) {
    Object_sb7::load(filename.c_str());
}

void Object_sb7::load(const char* filename) {
    using namespace std;

    free();

    // read the file and use exceptions settings
    ifstream ifs {filename, ios_base::binary};
    if (!ifs)
        throw runtime_error{string{"cannot open file "} + filename};
    ifs.exceptions(ifs.exceptions() | ios_base::badbit);

    //if (!ifs.read(reinterpret_cast<char*>(&header_), sizeof(header_)) ||
    //        !std::equal(identifier_.begin(), identifier_.end(),
    //            header_.identifier.begin()))
    //    throw runtime_error{filename + string{": failed to read header"}};

    auto start = ifs.tellg();

    ifs.seekg(0, ios_base::end);
    const auto end = ifs.tellg();
    ifs.seekg(start);

    const auto file_size = end - start;

    const auto data = make_unique<char[]>(file_size);

    auto ptr = data.get();

    ifs.read(ptr, file_size);

    //auto header = make_unique<SB6M_HEADER_t>();

    //header.get() = reinterpret_cast<SB6M_HEADER_t*>(data.release());
    //reinterpret_cast<SB6M_HEADER_t>(data.release()));

    //data += header.get().size;

    const auto header = reinterpret_cast<SB6M_HEADER_t*>(ptr);
    ptr += header->size;

    SB6M_VERTEX_ATTRIB_CHUNK_t* vertex_attrib_chunk = nullptr;
    SB6M_CHUNK_VERTEX_DATA_t* vertex_data_chunk = nullptr;
    SB6M_CHUNK_INDEX_DATA_t* index_data_chunk = nullptr;
    SB6M_CHUNK_SUB_OBJECT_LIST_t* sub_object_chunk = nullptr;
    SB6M_DATA_CHUNK_t* data_chunk = nullptr;

    for (size_t i {0}; i < header->num_chunks; ++i) {
        const auto chunk = reinterpret_cast<SB6M_CHUNK_HEADER_t*>(ptr);
        ptr += chunk->size;

        switch (chunk->chunk_type) {
            case SB6M_CHUNK_TYPE_VERTEX_ATTRIBS:
                vertex_attrib_chunk =
                    reinterpret_cast<SB6M_VERTEX_ATTRIB_CHUNK_t*>(chunk);
                break;
            case SB6M_CHUNK_TYPE_VERTEX_DATA:
                vertex_data_chunk =
                    reinterpret_cast<SB6M_CHUNK_VERTEX_DATA_t*>(chunk);
                break;
            case SB6M_CHUNK_TYPE_INDEX_DATA:
                index_data_chunk =
                    reinterpret_cast<SB6M_CHUNK_INDEX_DATA_t*>(chunk);
                break;
            case SB6M_CHUNK_TYPE_SUB_OBJECT_LIST:
                sub_object_chunk =
                    reinterpret_cast<SB6M_CHUNK_SUB_OBJECT_LIST_t*>(chunk);
                break;
            case SB6M_CHUNK_TYPE_DATA:
                data_chunk = reinterpret_cast<SB6M_DATA_CHUNK_t*>(chunk);
                break;
            default:
                break; // goto failed;
        }
    }

    glCreateVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    if (data_chunk != nullptr) {
        glCreateBuffers(1, &data_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, data_buffer_);
        glBufferData(GL_ARRAY_BUFFER, data_chunk->data_length,
                reinterpret_cast<unsigned char*>(data_chunk) +
                data_chunk->data_offset, GL_STATIC_DRAW);
    } else {
        size_t data_size {0};
        size_t size_used {0};

        if (vertex_data_chunk != nullptr)
            data_size += vertex_data_chunk->data_size;

        if (index_data_chunk != nullptr)
            data_size += index_data_chunk->index_count *
                ((index_data_chunk->index_type == GL_UNSIGNED_SHORT) ?
                 sizeof(GLushort) : sizeof(GLubyte));

        glCreateBuffers(1, &data_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, data_buffer_);
        glBufferData(GL_ARRAY_BUFFER, data_size, nullptr, GL_STATIC_DRAW);

        if (vertex_data_chunk != nullptr) {
            glBufferSubData(GL_ARRAY_BUFFER, 0, vertex_data_chunk->data_size,
                    data.get() + vertex_data_chunk->data_offset);
            size_used += vertex_data_chunk->data_offset;
        }


        if (index_data_chunk != nullptr) {
            glBufferSubData(GL_ARRAY_BUFFER, size_used,
                    index_data_chunk->index_count *
                    (index_data_chunk->index_type == GL_UNSIGNED_SHORT ?
                     sizeof(GLushort) : sizeof(GLubyte)), data.get() +
                    index_data_chunk->index_data_offset);
        }
    }

    for (size_t i {0}; i < vertex_attrib_chunk->attrib_count; ++i) {
        SB6M_VERTEX_ATTRIB_DECL_t& attrib_decl =
            vertex_attrib_chunk->attrib_data[i];
        glVertexAttribPointer(i, attrib_decl.size, attrib_decl.type,
                attrib_decl.flags& SB6M_VERTEX_ATTRIB_FLAG_NORMALIZED ? GL_TRUE
                : GL_FALSE, attrib_decl.stride, (GLvoid
                    *)(uintptr_t)attrib_decl.data_offset);
        glEnableVertexAttribArray(i);
    }

    if (index_data_chunk != nullptr) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data_buffer_);
        index_type_ = index_data_chunk->index_type;
        index_offset_ = index_data_chunk->index_data_offset;
    } else {
        index_type_ = GL_NONE;
    }

    if (sub_object_chunk != nullptr) {
        if (sub_object_chunk->count > max_sub_objects_)
            sub_object_chunk->count = max_sub_objects_;

        for (size_t i {0}; i < sub_object_chunk->count; i++)
            sub_object[i] = sub_object_chunk->sub_object[i];

        num_sub_objects_ = sub_object_chunk->count;
    } else {
        sub_object[0].first = 0;
        sub_object[0].count = index_type_ != GL_NONE ?
            index_data_chunk->index_count : vertex_data_chunk->total_vertices;
        num_sub_objects_ = 1;
    }

    //delete[] data;
    //fclose(infile);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Object_sb7::free() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &data_buffer_);

    vao_ = 0;
    data_buffer_ = 0;
}

void Object_sb7::render_sub_object(const size_t object_index,
        const size_t instance_count, const size_t base_instance)
{
    glBindVertexArray(vao_);

    if (index_type_ != GL_NONE) {
        glDrawElementsInstancedBaseInstance(GL_TRIANGLES,
                sub_object[object_index].count, index_type_,
                reinterpret_cast<void*>(sub_object[object_index].first),
                instance_count, base_instance);
    } else {
        glDrawArraysInstancedBaseInstance(GL_TRIANGLES,
                sub_object[object_index].first, sub_object[object_index].count,
                instance_count, base_instance);
    }
}

}

#endif /* OBJECT_SB7_H */

