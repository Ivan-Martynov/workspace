#include "shader_tools/ShaderProgram.h"

#include <iostream>

namespace Marvin
{

static void handle_program_failure(const GLuint program_id)
{
    std::cerr << "Failed to link a shader program: ";

    GLint log_length {};
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

    if (log_length > 0)
    {
        std::string info_log (log_length, '\0');
        glGetProgramInfoLog(program_id, log_length, nullptr, info_log.data());
        std::cerr << info_log;
    }
    else
    {
        std::cerr << "no information log.\n";
    }
}

ShaderProgram::ShaderProgram(std::initializer_list<GLuint>&& shader_ids):
    program_id {glCreateProgram()}
{
    for (const auto shader_id : shader_ids)
    {
        glAttachShader(program_id, shader_id);
    }

    glLinkProgram(program_id);

    GLint success {};
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        handle_program_failure(program_id);
    }

    for (const auto shader_id : shader_ids)
    {
        glDeleteShader(shader_id);
    }
}
    
}