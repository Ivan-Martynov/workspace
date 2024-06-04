#ifndef _H_SHADERCREATOR_H_
#define _H_SHADERCREATOR_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Marvin
{

class ShaderCreator
{
  private:
    ShaderCreator() = delete;

  public:
    static GLuint create_from_string(GLenum, const GLchar**);
    static GLuint create_from_file(GLenum, const GLchar*);
};
    
} // namespace Marvin


#endif //_H_SHADERCREATOR_H_
