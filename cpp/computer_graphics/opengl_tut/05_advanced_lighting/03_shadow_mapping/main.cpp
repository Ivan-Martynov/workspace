/*
 * Following tutorial on OpenGL (materials):
 *
 * http://learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
 *
 * Demonstrating shadow mapping
 *
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
static Camera main_cam {glm::vec3{0, 0, 3}};

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
void gen_object_vao_vbo(GLuint&, GLuint&, const std::vector<GLfloat>&,
        const GLuint, const GLuint, const bool);
// load a texture
GLuint load_texture(const std::string&, const bool = false);
// generate depth map texture for the framebuffer
GLuint make_texture_fb(const int, const int);
GLuint make_texture_fb(const int , const int, const std::vector<GLfloat>&);
// create framebuffer and attach it to the framebuffer
GLuint make_framebuffer(const int, const int, const GLuint);
GLuint make_framebuffer(GLFWwindow*, const GLuint);

// quad vertices
std::vector<GLfloat> quad_vertices();
// cube vertices
std::vector<GLfloat> cube_vertices();
// floor vertices
std::vector<GLfloat> floor_vertices();

// put several calls for a step in the drawing loop into one function
void prepare_buffer();
// testing blinn-phong vs phong
void test_shadow_map(GLFWwindow*, const int = 0);
// main drawing loop
void game_loop(GLFWwindow*, const std::vector<Shader>&,
        const std::vector<GLuint>&, const glm::vec3&, const glm::mat4&,
        const float, const float, const int);
// drawing the scene
void draw_scene(const Shader&, const GLuint, const GLuint, const float,
        const glm::vec3&, const glm::mat4&, const GLuint, const GLuint,
        const int);
// display depth map by rendering to a plane
void draw_quad(const Shader&, const GLuint, const GLuint, const float,
        const float, const float, const int);

// function to compute aspect ratio of screen's width and height
float window_aspect_ratio(GLFWwindow*);
// function to compute sizeof elements lying in the vector container
template <class T>
constexpr size_t size_in_bytes(const std::vector<T>& v) {
    return v.size() * sizeof(T);
}

// process user input
void process_input(GLFWwindow*, const std::string&);
// display menu of possible actions and process them
void show_menu(GLFWwindow*, const std::string&);

// here goes the main()
int main(int argc, char *argv[]) try {

    static constexpr GLuint width {800}, height {600};
    last_x = width >> 1;
    last_y = height >> 1;
    GLFWwindow *win = init(width, height);

    std::cout <<
        "----------------------------------------------------------------\n" <<
        "This program demonstrates gamma correction:\n" <<
        "Press key 'G' to change scene (without / with gamma correction)\n" <<
        "keys A/D, left/right arrow keys control side camera movement\n" <<
        "up/down arrow keys - up and down, W/S - depth\n" <<
        "mouse can also be used to change view/zoom (scroll)\n" <<
        "----------------------------------------------------------------\n";

    if (argc > 1)
        process_input(win, argv[1]);
    else
        show_menu(win, argv[0]);

    test_shadow_map(win, 0);

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
 * Process user input
 */
void process_input(GLFWwindow *win, const std::string &inp) {
    static constexpr char num_options {'5'};
    const std::string s {inp};
    const char inp_char {s[0]};
    if (s.length() == 1 && inp_char >= '0' && inp_char < num_options) {
        test_shadow_map(win, inp_char - '0');
    } else {
        std::cerr << "Wrong input: drawing default scene\n";
        test_shadow_map(win, 0);
    }
}

/*
 * Display a menu of possible actions
 */
void show_menu(GLFWwindow *win, const std::string &prog_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        prog_name << " int_param, where int_param is:\n" <<
        "0:\tDepth buffer as a 2D grayscale image (default)\n" <<
        "1:\tcubes casting shadows (acne defect present)\n" <<
        "2:\tshadows (acne defect removed)\n" <<
        "3:\tshadows (no extra shadow on floor)\n" <<
        "4:\tpcf shadows on/off by pressing the space key\n";
    test_shadow_map(win, 0);
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
    GLFWwindow *win = glfwCreateWindow(w, h, "Shadow mapping",
            nullptr, nullptr);
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
void gen_object_vao_vbo(GLuint& a, GLuint& b, const std::vector<GLfloat>& v,
        const GLuint stride, const GLuint offset_val, const bool add_attr) {
    glGenVertexArrays(1, &a);
    glGenBuffers(1, &b);

    glBindVertexArray(a);

    glBindBuffer(GL_ARRAY_BUFFER, b);
    glBufferData(GL_ARRAY_BUFFER, size_in_bytes(v), v.data(), GL_STATIC_DRAW);

    static const auto size_f = sizeof(GLfloat);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * size_f,
            (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, offset_val, GL_FLOAT, GL_FALSE, stride * size_f,
            (GLvoid*)(3 * size_f));

    if (add_attr) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size_f << 3,
                (GLvoid*)(6 * size_f));
    }

    glBindVertexArray(0);
}

// load a texture with checking whether it is in sRGB space (gamma correction)
GLuint load_texture(const std::string& img_fn, const bool is_gamma) {
    GLuint tex_id;
    glGenTextures(1, &tex_id);

    int img_w, img_h;
    unsigned char *img = SOIL_load_image(img_fn.c_str(), &img_w, &img_h, 0,
            SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    // using GL_SRGB assuming that texture is in sRGB
    glTexImage2D(GL_TEXTURE_2D, 0, is_gamma ? GL_SRGB : GL_RGB, img_w, img_h,
            0, GL_RGB, GL_UNSIGNED_BYTE, img);
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

GLuint load_texture(const std::string& img_fn, const std::vector<GLfloat>& v) {
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, v.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex_id;
}


// generate depth map texture for the framebuffer
GLuint make_texture_fb(const int shadow_w, const int shadow_h) {
    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_w, shadow_h, 0,
            GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex_id;
}

// generate a depth map texture with setting border color
GLuint make_texture_fb(const int shadow_w, const int shadow_h,
        const std::vector<GLfloat>& brd_clr) {
    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_w, shadow_h, 0,
            GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, brd_clr.data());

    glBindTexture(GL_TEXTURE_2D, 0);

    return tex_id;
}

// create framebuffer and attach it to the framebuffer
GLuint make_framebuffer(const GLuint tex_id) {
    // create a framebuffer object
    GLuint fbo {};
    glGenFramebuffers(1, &fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
            tex_id, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error {"Framebuffer is not complete"};
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return fbo;
}

// quad vertices
std::vector<GLfloat> quad_vertices() {
    return std::vector<GLfloat> {
        // pos      tex coords
        -1,  1, 0,  0, 1,
        -1, -1, 0,  0, 0,
         1,  1, 0,  1, 1,
         1, -1, 0,  1, 0,
    };
}

// cube vertices
std::vector<GLfloat> cube_vertices() {
    return std::vector<GLfloat> {
        // Back face
        -0.5, -0.5, -0.5, 0, 0, -1, 0, 0, // Bottom-left
         0.5,  0.5, -0.5, 0, 0, -1, 1, 1, // top-right
         0.5, -0.5, -0.5, 0, 0, -1, 1, 0, // bottom-right
         0.5,  0.5, -0.5, 0, 0, -1, 1, 1, // top-right
        -0.5, -0.5, -0.5, 0, 0, -1, 0, 0, // bottom-left
        -0.5,  0.5, -0.5, 0, 0, -1, 0, 1, // top-left
        // Front face
        -0.5, -0.5, 0.5, 0, 0, 1, 0, 0,   // bottom-left
         0.5, -0.5, 0.5, 0, 0, 1, 1, 0,   // bottom-right
         0.5,  0.5, 0.5, 0, 0, 1, 1, 1,   // top-right
         0.5,  0.5, 0.5, 0, 0, 1, 1, 1,   // top-right
        -0.5,  0.5, 0.5, 0, 0, 1, 0, 1,   // top-left
        -0.5, -0.5, 0.5, 0, 0, 1, 0, 0,   // bottom-left
        // Left face
        -0.5,  0.5,  0.5, -1, 0, 0, 1, 0, // top-right
        -0.5,  0.5, -0.5, -1, 0, 0, 1, 1, // top-left
        -0.5, -0.5, -0.5, -1, 0, 0, 0, 1, // bottom-left
        -0.5, -0.5, -0.5, -1, 0, 0, 0, 1, // bottom-left
        -0.5, -0.5,  0.5, -1, 0, 0, 0, 0, // bottom-right
        -0.5,  0.5,  0.5, -1, 0, 0, 1, 0, // top-right
        // Right face
         0.5,  0.5,  0.5, 1, 0, 0, 1, 0,  // top-left
         0.5, -0.5, -0.5, 1, 0, 0, 0, 1,  // bottom-right
         0.5,  0.5, -0.5, 1, 0, 0, 1, 1,  // top-right
         0.5, -0.5, -0.5, 1, 0, 0, 0, 1,  // bottom-right
         0.5,  0.5,  0.5, 1, 0, 0, 1, 0,  // top-left
         0.5, -0.5,  0.5, 1, 0, 0, 0, 0,  // bottom-left
        // Bottom face
        -0.5, -0.5, -0.5, 0, -1, 0, 0, 1, // top-right
         0.5, -0.5, -0.5, 0, -1, 0, 1, 1, // top-left
         0.5, -0.5,  0.5, 0, -1, 0, 1, 0, // bottom-left
         0.5, -0.5,  0.5, 0, -1, 0, 1, 0, // bottom-left
        -0.5, -0.5,  0.5, 0, -1, 0, 0, 0, // bottom-right
        -0.5, -0.5, -0.5, 0, -1, 0, 0, 1, // top-right
        // Top face
        -0.5, 0.5, -0.5, 0, 1, 0, 0, 1,   // top-left
         0.5, 0.5,  0.5, 0, 1, 0, 1, 0,   // bottom-right
         0.5, 0.5, -0.5, 0, 1, 0, 1, 1,   // top-right
         0.5, 0.5,  0.5, 0, 1, 0, 1, 0,   // bottom-right
        -0.5, 0.5, -0.5, 0, 1, 0, 0, 1,   // top-left
        -0.5, 0.5,  0.5, 0, 1, 0, 0, 0    // bottom-left
    };
}

// floor vertices
std::vector<GLfloat> floor_vertices() {
    return std::vector<GLfloat> {
        // positions     normals   tex coords
         25, -0.5,  25,  0, 1, 0,  25,  0,
        -25, -0.5, -25,  0, 1, 0,   0, 25,
        -25, -0.5,  25,  0, 1, 0,   0,  0,

         25, -0.5,  25,  0, 1, 0,  25,  0,
         25, -0.5, -25,  0, 1, 0,  25, 25,
        -25, -0.5, -25,  0, 1, 0,   0, 25
    };
}

// testing shadow mapping
void test_shadow_map(GLFWwindow* win, const int option) {
    // objects setup
    constexpr GLuint n {3};
    std::vector<GLuint> vao_vec(n), vbo_vec(n);
    gen_object_vao_vbo(vao_vec[0], vbo_vec[0], floor_vertices(), 8, 3, true);
    gen_object_vao_vbo(vao_vec[1], vbo_vec[1], cube_vertices(),  8, 3, true);
    gen_object_vao_vbo(vao_vec[2], vbo_vec[2], quad_vertices(),  5, 2, false);

    // shaders
    std::vector<Shader> shads {Shader{shad_path + "shadow_depth_01.vs",
        shad_path + "shadow_depth_01.frag"},
        Shader{shad_path + "shadow_depth_02.vs",
            shad_path + "shadow_depth_02.frag"}};
    if (option) {
        switch (option) {
            case 1:
                shads.push_back(Shader{shad_path + "shadow_depth_03.vs",
                        shad_path + "shadow_depth_03.frag"});
                break;
            case 2:
                shads.push_back(Shader{shad_path + "shadow_depth_03.vs",
                        shad_path + "shadow_depth_04.frag"});
                break;
            case 3:
                shads.push_back(Shader{shad_path + "shadow_depth_03.vs",
                        shad_path + "shadow_depth_05.frag"});
                break;
            case 4:
                shads.push_back(Shader{shad_path + "shadow_depth_03.vs",
                        shad_path + "shadow_depth_06.frag"});
                break;
            default:
                break;
        }
        shads[2].use();
        glUniform1i(glGetUniformLocation(shads[2].id(), "diffuse_tex"), 0);
        glUniform1i(glGetUniformLocation(shads[2].id(), "shadow_map"), 1);
    }

    // light source setup
    const GLfloat near_plane {1.0}, far_plane {7.5};
    const glm::vec3 lamp_pos = {-2, 4, -1};
    const auto lamp_proj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f,
            near_plane, far_plane);
    const auto lamp_view = glm::lookAt(lamp_pos, glm::vec3{0},
            glm::vec3{0, 1, 0});
    const auto lamp_mat = lamp_proj * lamp_view;
    shads[0].use();
    glUniformMatrix4fv(glGetUniformLocation(shads[0].id(), "light_matrix"), 1,
            GL_FALSE, glm::value_ptr(lamp_mat));

    game_loop(win, shads, vao_vec, lamp_pos, lamp_mat, near_plane, far_plane,
            option);
}

// put several calls for a step in the drawing loop into one function
void prepare_buffer() {
    const auto curr_time = glfwGetTime();
    delta_frame_time = curr_time - last_frame_time;
    last_frame_time  = curr_time;
    glfwPollEvents();
    do_movement();
}

// main game loop
void game_loop(GLFWwindow* win, const std::vector<Shader>& shads,
        const std::vector<GLuint>& vao_vec, const glm::vec3& lamp_pos,
        const glm::mat4& lamp_mat, const float near_plane,
        const float far_plane, const int option) {
    int win_w, win_h;
    glfwGetFramebufferSize(win, &win_w, &win_h);
    const GLuint shadow_width {1024}, shadow_height {1024};
    const GLuint tex_id = load_texture(tex_path + "hardwood_floor.jpg");
    const GLuint tex_fb = (option >= 3) ? make_texture_fb(shadow_width,
            shadow_height, {1, 1, 1, 1}) : make_texture_fb(shadow_width,
                shadow_height);
    const GLuint fbo    = make_framebuffer(tex_fb);
    const float win_asp {float(win_w) / win_h};

    glClearColor(0.2, 0.2, 0.2, 1);
    bool is_shadow {true};
    // the game loop
    while (!glfwWindowShouldClose(win)) {
        prepare_buffer();

        // drawing scene
        glViewport(0, 0, shadow_width, shadow_height);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT); // peter panning removal trick
        draw_scene(shads[0], vao_vec[0], vao_vec[1], win_asp, lamp_pos,
                lamp_mat, tex_id, tex_fb, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glCullFace(GL_BACK);

        glViewport(0, 0, win_w, win_h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (option)
            draw_scene(shads[2], vao_vec[0], vao_vec[1], win_asp, lamp_pos,
                    lamp_mat, tex_id, tex_fb, option);

        if (option == 4) {
            glUniform1i(glGetUniformLocation(shads[2].id(), "is_shadow"),
                    is_shadow);
            // turn shadows on/off by pressing the space key
            if (keys[GLFW_KEY_SPACE] && !keys_pressed[GLFW_KEY_SPACE]) {
                is_shadow = !is_shadow;
                keys_pressed[GLFW_KEY_SPACE] = true;
            }
        }

        draw_quad(shads[1], vao_vec[2], tex_fb, near_plane, far_plane, win_asp,
                option);

        glfwSwapBuffers(win);
    }
}

// drawing the scene
void draw_scene(const Shader& s, const GLuint vao1, const GLuint vao2,
        const float win_asp, const glm::vec3& lamp_pos,
        const glm::mat4& lamp_mat, const GLuint tex_id, const GLuint tex_depth,
        const int option) {
    s.use();
    const auto idx = s.id();

    glUniformMatrix4fv(glGetUniformLocation(idx, "light_matrix"), 1,
            GL_FALSE, glm::value_ptr(lamp_mat));
    if (option) {
        const auto proj = glm::perspective(main_cam.zoom(), win_asp, 0.1f,
                100.0f);
        const auto view = main_cam.view_matrix();
        glUniformMatrix4fv(glGetUniformLocation(idx, "proj"), 1, GL_FALSE,
                glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(idx, "view"), 1, GL_FALSE,
                glm::value_ptr(view));
        glUniform3fv(glGetUniformLocation(idx, "light_pos"), 1, &lamp_pos[0]);
        glUniform3fv(glGetUniformLocation(idx, "view_pos"), 1,
                &main_cam.pos()[0]);
        glUniformMatrix4fv(glGetUniformLocation(idx, "light_matrix"), 1,
                GL_FALSE, glm::value_ptr(lamp_mat));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex_depth);
    }

    // model matrices
    static const std::vector<glm::mat4> mod_mats {glm::mat4{1}, glm::translate(
        glm::mat4{1}, glm::vec3{0, 1.5, 0}), glm::translate(glm::mat4{1},
        glm::vec3{2, 0, 1}), glm::scale(glm::rotate(glm::translate(glm::mat4{1},
        glm::vec3{-1, 0, 2}), 60.0f, glm::normalize(glm::vec3{1, 0, 1})),
        glm::vec3{0.5})};
    for (GLuint i {0}; i < mod_mats.size(); ++i) {
        glUniformMatrix4fv(glGetUniformLocation(idx, "model"), 1, GL_FALSE,
                glm::value_ptr(mod_mats[i]));
        glBindVertexArray(i ? vao2 : vao1);
        glDrawArrays(GL_TRIANGLES, 0, i ? 36 : 6);
        glBindVertexArray(0);
    }
}

// display depth map by rendering to a plane
void draw_quad(const Shader& s, const GLuint vao, const GLuint tex_id,
        const float near_plane, const float far_plane, const float win_asp,
        const int option) {
    s.use();
    const auto idx = s.id();

    glUniform1f(glGetUniformLocation(idx, "near_plane"), near_plane);
    glUniform1f(glGetUniformLocation(idx, "far_plane"), far_plane);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    if (option == 0) {
        glUniformMatrix4fv(glGetUniformLocation(idx, "model"), 1, GL_FALSE,
                glm::value_ptr(glm::perspective(main_cam.zoom(), win_asp, 0.1f,
                        100.0f) * main_cam.view_matrix()));


        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
}

