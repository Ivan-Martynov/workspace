#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

// include glew to get OpenGL headers
#include <GL/glew.h>

// GL math
//#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// helper function to read a shader file
std::string read_shader_file(const GLchar*);

class Shader {
public:
    // Ctor reads and builds the shader
    Shader();
    explicit Shader(const GLchar*, const GLchar*);
    explicit Shader(const std::string&, const std::string&);
    explicit Shader(const GLchar*, const GLchar*, const GLchar*);
    explicit Shader(const std::string&, const std::string&, const std::string&);

    // get shader's program id
    GLuint id() const { return prog_; }
    // use the program
    Shader& use();

    void set_integer(const GLchar*, const GLint val);
    void set_float(const GLchar*, const GLfloat val);
    void set_vector2f(const GLchar*, const glm::vec2&);
    void set_vector3f(const GLchar*, const glm::vec3&);
    void set_vector4f(const GLchar*, const glm::vec4&);
    void set_matrix4(const GLchar*, const glm::mat4&);
private:
    // The program ID
    GLuint prog_;

    // helper function to initialize the shader
    GLuint init_shader(const GLenum, const GLchar*);
};

// read shader from a file into a string
std::string read_shader_file(const GLchar *shad_file) {
    using namespace std;
    ifstream ifs {shad_file};
    if (!ifs)
        throw runtime_error {"cannot open file " + string(shad_file)};
    ifs.exceptions(ifs.exceptions() | ios_base::badbit);
    // keep the result in a string
    string shad_str {};
    for (string s; getline(ifs, s);) {
        if (ifs.fail()) {
            ifs.unget();
            ifs.clear(ios_base::failbit);
        }
        shad_str += s + '\n';
    }
    return shad_str;
}

// Create a shader object
GLuint Shader::init_shader(const GLenum shad_type, const GLchar *src) {
    // parameter: provide the type of shader
    GLuint shad = glCreateShader(shad_type);
    // attach the shader source code to the shader object
    glShaderSource(shad, 1, &src, nullptr);
    // compile the shader
    glCompileShader(shad);
    // check if compilation was successful
    GLint success;
    GLchar info_log[512];
    glGetShaderiv(shad, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shad, 512, nullptr, info_log);
        throw std::runtime_error {"Shader compilation failed:\n" +
                std::string(info_log)};
    }
    return shad;
}

// default constructor
Shader::Shader(): prog_{} { }

// constructor
Shader::Shader(const GLchar *vert, const GLchar *frag): prog_{} {
    // get vertex and fragment shaders
    std::string vert_src {read_shader_file(vert)};
    std::string frag_src {read_shader_file(frag)};
    const GLuint vert_shad {init_shader(GL_VERTEX_SHADER, vert_src.c_str())};
    const GLuint frag_shad {init_shader(GL_FRAGMENT_SHADER, frag_src.c_str())};
    // shader program
    prog_ = glCreateProgram();
    // attach the compiled shaders to the program
    glAttachShader(prog_, vert_shad);
    glAttachShader(prog_, frag_shad);
    // link the shaders
    glLinkProgram(prog_);
    // check if linking the shader program failed
    GLint success;
    GLchar info_log[512];
    glGetProgramiv(prog_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(prog_, 512, nullptr, info_log);
        throw std::runtime_error {"Shader program linking failed:\n" +
                std::string(info_log)};
    }
    glDeleteShader(vert_shad);
    glDeleteShader(frag_shad);
}

// constructor for three files: geometry shader as the second file
Shader::Shader(const GLchar *vert, const GLchar *geom, const GLchar *frag):
    prog_{} {
    // get vertex, geometry and fragment shaders
    std::string vert_src {read_shader_file(vert)};
    std::string geom_src {read_shader_file(geom)};
    std::string frag_src {read_shader_file(frag)};
    const GLuint vert_shad {init_shader(GL_VERTEX_SHADER, vert_src.c_str())};
    const GLuint geom_shad {init_shader(GL_GEOMETRY_SHADER, geom_src.c_str())};
    const GLuint frag_shad {init_shader(GL_FRAGMENT_SHADER, frag_src.c_str())};
    // shader program
    prog_ = glCreateProgram();
    // attach the compiled shaders to the program
    glAttachShader(prog_, vert_shad);
    glAttachShader(prog_, geom_shad);
    glAttachShader(prog_, frag_shad);
    // link the shaders
    glLinkProgram(prog_);
    // check if linking the shader program failed
    GLint success;
    GLchar info_log[512];
    glGetProgramiv(prog_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(prog_, 512, nullptr, info_log);
        throw std::runtime_error {"Shader program linking failed:\n" +
                std::string(info_log)};
    }
    glDeleteShader(vert_shad);
    glDeleteShader(geom_shad);
    glDeleteShader(frag_shad);
}

// constructors using std::string
Shader::Shader(const std::string &vert, const std::string &frag):
    Shader(vert.c_str(), frag.c_str()) { }
Shader::Shader(const std::string &vert, const std::string &geom,
        const std::string &frag):
    Shader(vert.c_str(), geom.c_str(), frag.c_str()) { }

Shader& Shader::use() {
    glUseProgram(prog_);
    return *this;
}

void Shader::set_integer(const GLchar* name, const GLint val) {
    glUniform1i(glGetUniformLocation(prog_, name), val);
}

void Shader::set_float(const GLchar* name, const GLfloat val) {
    glUniform1f(glGetUniformLocation(prog_, name), val);
}

void Shader::set_vector2f(const GLchar* name, const glm::vec2& vec) {
    glUniform2f(glGetUniformLocation(prog_, name), vec.x, vec.y);
}

void Shader::set_vector3f(const GLchar* name, const glm::vec3& vec) {
    glUniform3f(glGetUniformLocation(prog_, name), vec.x, vec.y, vec.z);
}

void Shader::set_vector4f(const GLchar* name, const glm::vec4& vec) {
    glUniform4f(glGetUniformLocation(prog_, name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::set_matrix4(const GLchar* name, const glm::mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(prog_, name), 1, GL_FALSE,
            glm::value_ptr(mat));
}

#endif

