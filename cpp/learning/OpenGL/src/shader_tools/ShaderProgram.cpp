#include "shader_tools/ShaderProgram.h"

namespace Marvin
{

ShaderProgram::ShaderProgram(std::initializer_list<GLuint>&& shader_ids):
    program_id {glCreateProgram()}
{
    for (const auto shader_id : shader_ids)
    {
        glAttachShader(program_id, shader_id);
    }

    glLinkProgram(program_id);

    for (const auto shader_id : shader_ids)
    {
        glDeleteShader(shader_id);
    }
}
    
}