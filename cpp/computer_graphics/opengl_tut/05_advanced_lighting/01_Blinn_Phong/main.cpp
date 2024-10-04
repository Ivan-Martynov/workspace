/*
 * Following tutorial on OpenGL (materials):
 *
 * http://learnopengl.com/#!Advanced-Lighting/Advanced-Lighting
 *
 * Demonstrating Blinn-Phong lighting
 */

#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>
#include <cassert>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

// GL math
//#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../classes/Shader.h"
#include "../../classes/Camera.h"

// paths to the folder where we keep shaders and textures: global vars
static const std::string shad_path {"../../shaders/"};
static const std::string tex_path {"../../images/"};

// tracking which keys have been pressed/released (for smooth movement)
static bool keys[1024];
static bool keys_pressed[1024];

// global values to keep track of time between the frames
static GLfloat delta_frame_time = 0;
static GLfloat last_frame_time  = 0;

// last cursor position
static GLfloat last_x = 0, last_y = 0;

// avoid sudden jump of the camera at the beginning
static bool first_mouse_move = true;

// using Camera class
static Camera main_cam {glm::vec3{0, 2, 5}};

/*
 * Functions declarations
 */
// initialize stuff
GLFWwindow* init(const GLuint, const GLuint);
// callback functions
void key_callback(GLFWwindow*, const int, const int, const int, const int);
void mouse_callback(GLFWwindow*, const double, const double);
void scroll_callback(GLFWwindow*, const double, const double);
// movement function
void do_movement();
// cleaning up
int clean_up(const int);

// generate VAO and VBOs
void gen_object_vao_vbo(GLuint&, GLuint&, const std::vector<GLfloat>&);
// floor vertices
std::vector<GLfloat> floor_vertices();

// put several calls for a step in the drawing loop into one function
void prepare_buffer();
// testing blinn-phong vs phong
void test_blinn_phong(GLFWwindow*);

// function to compute aspect ratio of screen's width and height
float window_aspect_ratio(GLFWwindow*);
// function to compute sizeof elements lying in the vector container
template <class T>
constexpr size_t size_in_bytes(const std::vector<T>& v) {
    return v.size() * sizeof(T);
}

// here goes the main()
int main() try {

    static constexpr GLuint width {800}, height {600};
    last_x = width >> 1;
    last_y = height >> 1;
    GLFWwindow *win = init(width, height);

    std::cout <<
        "----------------------------------------------------------------\n" <<
        "This program demonstrates Phong vs Blinn-Phong lighting:\n" <<
        "Press key 'B' to change lighting (Phong <=> Blinn-Phong)\n" <<
        "keys A/D, left/right arrow keys control side camera movement\n" <<
        "up/down arrow keys - up and down, W/S - depth\n" <<
        "mouse can also be used to change view/zoom (scroll)\n" <<
        "----------------------------------------------------------------\n";

    test_blinn_phong(win);

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
    GLFWwindow *win = glfwCreateWindow(w, h, "Blinn-Phong", nullptr, nullptr);
    if (win == nullptr)
        throw std::runtime_error {"Failed to create GLFW window"};

    glfwMakeContextCurrent(win);

    // let GLEW know to use a modern approach to retrieving function pointers
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::runtime_error {"Failed to initialize GLEW"};

    // register callbacks
    glfwSetKeyCallback(win, key_callback);
    glfwSetCursorPosCallback(win, mouse_callback);
    glfwSetScrollCallback(win, scroll_callback);

    // disable cursor
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // inform OpenGL about the size of the rendering window
    glViewport(0, 0, w, h);

    // allow modifying the size of pixel
    glEnable(GL_PROGRAM_POINT_SIZE);

    // enable the depth test
    glEnable(GL_DEPTH_TEST);

    return win;
}

// function to calculate aspect ratio of screen's width and height
float window_aspect_ratio(GLFWwindow *win) {
    int win_w, win_h;
    glfwGetFramebufferSize(win, &win_w, &win_h);
    return float(win_w) / win_h;
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
 * Call this function whenever pointer (mouse) moves
 */
void mouse_callback(GLFWwindow*, const double xpos, const double ypos) {
    if (first_mouse_move) {
        last_x = xpos;
        last_y = ypos;
        first_mouse_move = false;
    }

    main_cam.process_mouse_move(xpos - last_x, last_y - ypos);

    last_x = xpos;
    last_y = ypos;
}

/*
 * Call this function during scrolling
 */
void scroll_callback(GLFWwindow*, const double, const double yoffset) {
    main_cam.process_scroll(yoffset);
}

/*
 * Function for smooth movement of the camera
 */
void do_movement() {
    if (keys[GLFW_KEY_UP])
        main_cam.process_keyboard(Camera::up_dir, delta_frame_time);
    else if (keys[GLFW_KEY_DOWN])
        main_cam.process_keyboard(Camera::down_dir, delta_frame_time);
    else if (keys[GLFW_KEY_S])
        main_cam.process_keyboard(Camera::backward_dir, delta_frame_time);
    else if (keys[GLFW_KEY_W])
        main_cam.process_keyboard(Camera::forward_dir, delta_frame_time);
    else if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        main_cam.process_keyboard(Camera::left_dir, delta_frame_time);
    else if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        main_cam.process_keyboard(Camera::right_dir, delta_frame_time);
}

/*
 * Properly clean up and return the value as an indicator of success (0) or
 * failure (1 or other non-zero value)
 */
int clean_up(const int val) {
    glfwTerminate();
    return val;
}

/*
 * Generate objects like Vertex Array Objects, Vertex Buffer Object using
 * pointer to std::vector (trying to achieve more general solution)
 */
void gen_objects(std::vector<GLuint> *VAO_vec, std::vector<GLuint> *VBO_vec) {
    for (size_t i {0}; i < (*VAO_vec).size(); ++i)
        glGenVertexArrays(1, &(*VAO_vec)[i]);
    for (size_t i {0}; i < (*VBO_vec).size(); ++i)
        glGenBuffers(1, &(*VBO_vec)[i]);
}

// generate vao and vbo for a floor object
void gen_object_vao_vbo(GLuint& a, GLuint& b, const std::vector<GLfloat>& v) {
    glGenVertexArrays(1, &a);
    glGenBuffers(1, &b);

    glBindVertexArray(a);

    glBindBuffer(GL_ARRAY_BUFFER, b);
    glBufferData(GL_ARRAY_BUFFER, size_in_bytes(v), v.data(), GL_STATIC_DRAW);

    static const auto size_f = sizeof(GLfloat);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size_f << 3, (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size_f << 3,
            (GLvoid*)(3 * size_f));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size_f << 3,
            (GLvoid*)(6 * size_f));

    glBindVertexArray(0);
}

// load a texture
GLuint load_texture(const std::string& img_fn) {
    GLuint tex_id;
    glGenTextures(1, &tex_id);

    int img_w, img_h;
    unsigned char *img = SOIL_load_image(img_fn.c_str(), &img_w, &img_h, 0,
            SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w, img_h, 0, GL_RGB,
            GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex_id;
}

// floor vertices
std::vector<GLfloat> floor_vertices() {
    return std::vector<GLfloat> {
        // position    normal    tex_coord
         8, -0.5,  8,  0, 1, 0,  5, 0,
        -8, -0.5,  8,  0, 1, 0,  0, 0,
        -8, -0.5, -8,  0, 1, 0,  0, 5,

         8, -0.5,  8,  0, 1, 0,  5, 0,
        -8, -0.5, -8,  0, 1, 0,  0, 5,
         8, -0.5, -8,  0, 1, 0,  5, 5
    };
}

// put several calls for a step in the drawing loop into one function
void prepare_buffer() {
    const auto curr_time = glfwGetTime();
    delta_frame_time = curr_time - last_frame_time;
    last_frame_time  = curr_time;
    glfwPollEvents();
    do_movement();
    glClearColor(0.2, 0.2, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// testing blinn-phong vs phong
void test_blinn_phong(GLFWwindow* win) {
    GLuint vao {}, vbo {};
    gen_object_vao_vbo(vao, vbo, floor_vertices());

    // texture
    const GLuint tex_id {load_texture(tex_path + "hardwood_floor.jpg")};

    // shader
    const Shader shad {shad_path + "blinn_phong_01.vs",
        shad_path + "blinn_phong_01.frag"};

    // light source
    const glm::vec3 lamp_pos {1, 5, -3};

    const auto win_asp = window_aspect_ratio(win);
    const auto idx = shad.id();
    bool is_blinn {false};
    // the game loop
    while (!glfwWindowShouldClose(win)) {
        prepare_buffer();

        // check if button 'B' has been pressed
        if (keys[GLFW_KEY_B] && !keys_pressed[GLFW_KEY_B]) {
            is_blinn = !is_blinn;
            keys_pressed[GLFW_KEY_B] = true;
        }

        shad.use();
        const auto proj = glm::perspective(main_cam.zoom(), win_asp, 0.1f,
                100.0f);
        const auto view = main_cam.view_matrix();
        glUniformMatrix4fv(glGetUniformLocation(idx, "proj"), 1, GL_FALSE,
                glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(idx, "view"), 1, GL_FALSE,
                glm::value_ptr(view));
        glUniform3fv(glGetUniformLocation(idx, "light_pos"), 1, &lamp_pos[0]);
        glUniform3fv(glGetUniformLocation(idx, "view_pos"), 1,
                &(main_cam.pos()[0]));
        glUniform1i(glGetUniformLocation(idx, "is_blinn"), is_blinn);

        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(win);
    }
}

