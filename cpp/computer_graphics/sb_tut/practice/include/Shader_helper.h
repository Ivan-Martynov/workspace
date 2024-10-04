#ifndef SHADER_HELPER_H
#define SHADER_HELPER_H

#include <fstream>
#include <vector>
#include <map>
#include <glad/glad.h>

namespace OpenGL_tutorial {

class Shader_helper {
public:
    static GLuint create_program(const std::map<GLenum, std::string>&,
            GLboolean = GL_TRUE);
    static std::string shader_type_to_string(const GLenum);

private:
    constexpr explicit Shader_helper() { }

    // helper function to initialize the shader
    static GLuint init_shader(const GLenum, const GLchar*);
    static GLuint init_shader_from_file(const GLenum, const std::string&);

    // helper function to read a shader file
    static std::string read_shader_file(const GLchar*);
    static std::string read_shader_file(const std::string&);
};

// read shader from a file into a string
std::string Shader_helper::read_shader_file(const GLchar* shad_file) {
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

std::string Shader_helper::read_shader_file(const std::string& shad_file) {
    return read_shader_file(shad_file.c_str());
}

std::string Shader_helper::shader_type_to_string(const GLenum type) {
    switch (type) {
        case GL_VERTEX_SHADER:
            return "GL_VERTEX_SHADER";
        case GL_TESS_CONTROL_SHADER:
            return "GL_TESS_CONTROL_SHADER";
        case GL_TESS_EVALUATION_SHADER:
            return "GL_TESS_EVALUATION_SHADER";
        case GL_GEOMETRY_SHADER:
            return "GL_GEOMETRY_SHADER";
        case GL_FRAGMENT_SHADER:
            return "GL_FRAGMENT_SHADER";
        case GL_COMPUTE_SHADER:
            return "GL_COMPUTE_SHADER";
        default:
            return "UNKNOWN_SHADER_TYPE";
    }
}

// Create a shader object
GLuint Shader_helper::init_shader(const GLenum shad_type, const GLchar* src) {
    // parameter: provide the type of shader
    GLuint shad = glCreateShader(shad_type);

    // attach the shader source code to the shader object
    glShaderSource(shad, 1, &src, nullptr);

    // compile the shader
    glCompileShader(shad);

    // check if compilation was successful
    GLint param {};
    glGetShaderiv(shad, GL_COMPILE_STATUS, &param);

    if (!param) {
        glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &param);

        std::string info_log (param, ' ');
        glGetShaderInfoLog(shad, param, nullptr, &info_log[0]);

        glGetShaderiv(shad, GL_SHADER_TYPE, &param);

        throw std::runtime_error {shader_type_to_string(shad_type) +
                " compilation failed:\n" + info_log};
    }

    return shad;
}

// Create a shader object
GLuint Shader_helper::init_shader_from_file(const GLenum shad_type,
        const std::string& file_name) {
    const std::string shad_src {read_shader_file(file_name)};

    return init_shader(shad_type, shad_src.c_str());
}

GLuint Shader_helper::create_program(const std::map<GLenum, std::string>&
        sources, GLboolean provide_binary) {
    GLuint program_id {glCreateProgram()};

    std::vector<GLuint> shad_ids = std::vector<GLuint>();
    for (const auto& src : sources) {
        const GLuint shad {init_shader_from_file(src.first, src.second)};

        glAttachShader(program_id, shad);

        shad_ids.push_back(shad);
    }

    glProgramParameteri(program_id, GL_PROGRAM_BINARY_RETRIEVABLE_HINT,
            provide_binary);

    glLinkProgram(program_id);

    // check if linking the shader program failed
    GLint param;
    glGetProgramiv(program_id, GL_LINK_STATUS, &param);

    if (!param) {
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &param);
        std::string info_log (param, ' ');
        glGetProgramInfoLog(program_id, param, nullptr, &info_log[0]);

        glGetProgramiv(program_id, GL_ATTACHED_SHADERS, &param);

        throw std::runtime_error {"Shader program linking failed:\nShaders: " +
            std::to_string(param) + '\n' + info_log};
    }

    for (const auto x : shad_ids)
        glDeleteShader(x);

    return program_id;
}

} // end namespace OpenGL_tutorial

#endif /* SHADER_HELPER_H */

