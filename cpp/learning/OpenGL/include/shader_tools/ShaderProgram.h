#ifndef _H_SHADERPROGRAM_H_
#define _H_SHADERPROGRAM_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <initializer_list>

namespace Marvin
{

class ShaderProgram
{
  private:
    GLuint program_id;
   
  public:
    explicit ShaderProgram(std::initializer_list<GLuint>&&);

    ~ShaderProgram() { glDeleteProgram(program_id); }

    void use() const { glUseProgram(program_id); }

    GLuint get_id() const { return program_id; }
};
    
} // namespace Marving


#endif //_H_SHADERPROGRAM_H_
