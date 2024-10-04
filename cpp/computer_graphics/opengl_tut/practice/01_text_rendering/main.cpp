/*
 * Following tutorial on OpenGL (materials):
 *
 * https://learnopengl.com/#!In-Practice/Text-Rendering
 *
 * Demonstrating text rendering with FreeType font
 */

#include <iostream>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../classes/Shader.h"

// set up headers for freetype font
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

// struct for a FreeType character
struct Character {
    GLuint tex_id {}; // id for glyph texture
    glm::ivec2 size {}; // glyph size
    glm::ivec2 bearing {}; // offset from baseline to left/top of glyph
    GLuint advance {}; // offset to advance to next glyph
};
std::map<GLchar, Character> Characters;

// paths to the folder where we keep shaders and textures: global vars
static const std::string shad_path {"../../shaders/"};

// tracking which keys have been pressed/released (for smooth movement)
static bool keys[1024];
static bool keys_pressed[1024];

/*
 * Functions declarations
 */
// initialize stuff
GLFWwindow* init(const GLuint, const GLuint);
// callback functions
void key_callback(GLFWwindow*, const int, const int, const int, const int);
// cleaning up
int clean_up(const int);

// initialize freetype font
void fftype_init();
// add characters to the collection (map)
void collect_chars(const FT_Face& face);
// make an ortho_view from window's width and height
auto ortho_view(GLFWwindow*);
// generate vao and vbo for text
void gen_text_vao_vbo(GLuint&, GLuint&);
// rendering text
void render_text(const Shader&, const GLuint, const GLuint, const std::string&,
        GLfloat, const GLfloat, const GLfloat, const glm::vec3);
// game loop
void text_game_loop(GLFWwindow*);

// here goes the main()
int main() try {

    // initialize window
    static constexpr GLuint width {800}, height {600};
    GLFWwindow *win = init(width, height);

    // initialize font
    fftype_init();

    std::cout <<
        "----------------------------------------------------------------\n" <<
        "This program demonstrates text rendering with FreeType:\n" <<
        "Press key 'B' to draw quads instead of characters\n" <<
        "----------------------------------------------------------------\n";

    // main loop
    text_game_loop(win);

    // clean up and exit properly
    return clean_up(0);

} catch (const std::runtime_error &e) {
    std::cerr << e.what() << '\n';
    return clean_up(1);
} catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return clean_up(2);
} catch (...) {
    std::cerr << "Unknown exception\n";
    return clean_up(3);
}

/*
 * Function to initialize stuff. With comparison to the first tutorial here we
 * put more stuff inside a single function, though trying not to make it too
 * long (keeping the idea of fitting a single function on a screen in mind)
 */
GLFWwindow* init(const GLuint w, const GLuint h) {
    glfwInit();

    // configuring GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // create a window object
    GLFWwindow *win = glfwCreateWindow(w, h, "Text Rendering", nullptr,
            nullptr);
    if (win == nullptr)
        throw std::runtime_error {"Failed to create GLFW window"};

    glfwMakeContextCurrent(win);

    // let GLEW know to use a modern approach to retrieving function pointers
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::runtime_error {"Failed to initialize GLEW"};

    // register callbacks
    glfwSetKeyCallback(win, key_callback);

    // inform OpenGL about the size of the rendering window
    glViewport(0, 0, w, h);

    // enable blending to render characters (transparent background )
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return win;
}

// initialize freetype font
void fftype_init() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cerr << "FREETYPE: Cannot init FreeType library\n";

    FT_Face face;
    if (FT_New_Face(ft, "../01_text_rendering/arial.ttf", 0, &face))
        std::cerr << "FREETYPE: Cannot load font\n";

    FT_Set_Pixel_Sizes(face, 0, 48);
    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
        std::cerr << "FREETYPE: failed to load glyph\n";

    collect_chars(face);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

// put a set of FreeType characters into a map container
void collect_chars(const FT_Face& face) {
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c {0}; c < 128; ++c) {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "FREETYPE: failed to load glyph\n";
            continue;
        }

        // generate texture
        GLuint tex_id;
        glGenTextures(1, &tex_id);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer);

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // create a character and place to the map for later use
        const Character ch {tex_id,
            glm::ivec2{face->glyph->bitmap.width, face->glyph->bitmap.rows},
            glm::ivec2{face->glyph->bitmap_left, face->glyph->bitmap_top},
            static_cast<GLuint>(face->glyph->advance.x)};
        Characters.emplace(c, ch);
    }
}

/*
 * Call this function whenever a key is pressed / released
 */
void key_callback(GLFWwindow *win, const int key, const int,
        const int action, const int) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
            glfwSetWindowShouldClose(win, GL_TRUE);
        else
            keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
        keys_pressed[key] = false;
    }
}

/*
 * Properly clean up and return the value as an indicator of success (0) or
 * failure (1 or other non-zero value)
 */
int clean_up(const int val) { glfwTerminate(); return val; }

// generate vao and vbo for text
void gen_text_vao_vbo(GLuint& a, GLuint& b) {
    glGenVertexArrays(1, &a);
    glGenBuffers(1, &b);

    glBindVertexArray(a);

    static const auto size_f = sizeof(GLfloat);
    glBindBuffer(GL_ARRAY_BUFFER, b);
    glBufferData(GL_ARRAY_BUFFER, size_f * 24, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, size_f << 2, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// rendering text
void render_text(const Shader& s, const GLuint vao, const GLuint vbo,
        const std::string& text, GLfloat x, const GLfloat y,
        const GLfloat scale, const glm::vec3 clr) {
    s.use();
    glUniform3f(glGetUniformLocation(s.id(), "text_color"),
            clr.x, clr.y, clr.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    // iterate through characters
    for (const auto c: text) {
        Character ch = Characters[c];

        const GLfloat xpos {x + ch.bearing.x * scale};
        const GLfloat ypos {y - (ch.size.y - ch.bearing.y) * scale};
        const GLfloat w {ch.size.x * scale}, h {ch.size.y * scale};

        // vbo for each character
        GLfloat verts[6][4] {
            { xpos,     ypos + h, 0, 0},
            { xpos,     ypos,     0, 1},
            { xpos + w, ypos,     1, 1},
            { xpos,     ypos + h, 0, 0},
            { xpos + w, ypos,     1, 1},
            { xpos + w, ypos + h, 1, 0}
        };

        // render glyph texture over the quad
        glBindTexture(GL_TEXTURE_2D, ch.tex_id);
        // update content of vbo memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render the quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // advance cursor to next glyph (1/64th pixels: bitshift by six to get
        // value in pixels (2 ^ 6 = 64);
        x += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// make an ortho_view from window's width and height
auto ortho_view(GLFWwindow* win) {
    int win_w, win_h;
    glfwGetFramebufferSize(win, &win_w, &win_h);
    return glm::ortho(0.0f, static_cast<GLfloat>(win_w),
            0.0f, static_cast<GLfloat>(win_h));
}

// main game loop for rendering text
void text_game_loop(GLFWwindow* win) {
    // set vao and vbo for text
    GLuint vao, vbo;
    gen_text_vao_vbo(vao, vbo);

    // shader for character glyphs rendering
    const Shader shad {shad_path + "glyph_01.vs", shad_path + "glyph_01.frag"};

    // ortho view
    const auto proj = ortho_view(win);
    shad.use();
    glUniformMatrix4fv(glGetUniformLocation(shad.id(), "proj"), 1, GL_FALSE,
            glm::value_ptr(proj));

    // flag to toggle whether to draw text or quads
    bool disp_text {true};

    // main loop
    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        glClearColor(0.2, 0.3, 0.3, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // toggle displaing text or quads by pressing button 'B'
        if (keys[GLFW_KEY_B] && !keys_pressed[GLFW_KEY_B]) {
            disp_text = !disp_text;
            keys_pressed[GLFW_KEY_B] = true;
        }
        (disp_text) ? glEnable(GL_BLEND) : glDisable(GL_BLEND);

        // display text
        render_text(shad, vao, vbo, "Greetings, processed text", 25, 25, 1,
                glm::vec3{0.5, 0.8, 0.2});
        render_text(shad, vao, vbo, "Rendering with OpenGL", 540, 570, 0.5,
                glm::vec3{0.3, 0.7, 0.9});

        glfwSwapBuffers(win);
    }
}

