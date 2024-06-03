#include "shader_tools/ShaderCreator.h"

#include <iostream>

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

GLuint ShaderCreator::create_shader(GLenum shader_type, const GLchar** source)
{
    GLuint shader_id {glCreateShader(shader_type)};
    glShaderSource(shader_id, 1, source, nullptr);
    glCompileShader(shader_id);

    GLint success{};
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success)
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

    return shader_id;
}

} // namespace Marvin
