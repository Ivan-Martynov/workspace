#include "shader_tools/ShaderCreator.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace Marvin
{

static std::string get_shader_type_name(const GLenum shader_type)
{
    switch (shader_type)
    {
    case GL_VERTEX_SHADER:
        return "vertex";

    case GL_FRAGMENT_SHADER:
        return "fragment";

    case GL_GEOMETRY_SHADER:
        return "geometry";

    default:
        return "Unknown shader type";
    }
}

static void handle_shader_failure(GLuint shader_id, GLenum shader_type)
{
    std::cerr << "Failed to compile a " <<
        get_shader_type_name(shader_type) << " shader: ";

    GLint log_length {};
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0)
    {
        std::string info_log (log_length, '\0');
        glGetShaderInfoLog(shader_id, log_length, nullptr, info_log.data());
        std::cerr << info_log;
    }
    else
    {
        std::cerr << "no information log.\n";
    }
}

GLuint ShaderCreator::create_from_string(GLenum shader_type,
    const GLchar** source)
{
    GLuint shader_id {glCreateShader(shader_type)};
    glShaderSource(shader_id, 1, source, nullptr);
    glCompileShader(shader_id);

    GLint success{};
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        handle_shader_failure(shader_id, shader_type);
    }

    return shader_id;
}

GLuint ShaderCreator::create_from_file(GLenum shader_type,
    const GLchar* file_path)
{
    std::ifstream file_stream {file_path};
    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    if (file_stream.is_open())
    {
        std::ostringstream string_stream {};
        string_stream << file_stream.rdbuf();

        const char* source {string_stream.str().data()};

        return ShaderCreator::create_from_string(shader_type, &source);
    }
    else
    {
        std::cerr << "Failed to open file " << file_path << "\n";
        return glCreateShader(shader_type);
    }
}

} // namespace Marvin
