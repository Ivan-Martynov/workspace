/*
 * Following tutorial on OpenGL (materials):
 * http://learnopengl.com/#!Lighting/Multiple-lights
 *
 * Practicing with multiple lights
 *
 */

#include <iostream>
#include <vector>
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

// global values to keep track of time between the frames
static GLfloat delta_frame_time = 0;
static GLfloat last_frame_time  = 0;

// last cursor position
static GLfloat last_x = 0, last_y = 0;

// avoid sudden jump of the camera at the beginning
static bool first_mouse_move = true;

// using Camera class
static Camera main_cam {glm::vec3{0, 0, 5}};

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
// generate VAO, VBO, EBO...
void gen_objects(std::vector<GLuint>*, std::vector<GLuint>*);
// bind the objects with vertex data
void make_objects(const GLuint, const GLuint, const std::vector<GLfloat>&,
        const bool, const int);

// drawing colored object and "lamp" box with various lighting
void illuminate_cubes(GLFWwindow*, const int = 0);
void draw_objects(GLFWwindow*, const std::vector<GLfloat>&, const Shader&,
        const int = 0, const bool = false);
void light_obj_loop(GLFWwindow*, const std::vector<GLuint>&,
        const std::vector<GLuint>&, const std::vector<Shader>&, const bool,
        const int);
std::vector<glm::vec3> environment_colors(const int);
void set_dir_light(const GLuint, const int);
void set_point_lights(const GLuint, const std::vector<glm::vec3>&,
        const std::vector<glm::vec3>&, const int);
void set_spot_light(const GLuint, const int);
void draw_light_obj(const Shader&, const GLuint, const std::vector<GLuint>&,
        const glm::mat4&, const glm::mat4&, const std::vector<glm::vec3>&,
        const std::vector<glm::vec3>&, const int);
void draw_lamp(const Shader&, const GLuint, const glm::mat4&,
        const glm::mat4&, const glm::vec3&, const glm::vec3& = {1, 1, 1});

// function to compute sizeof elements lying in the vector container
template <class T>
constexpr size_t size_of_elements(const std::vector<T> &v) {
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
        "This program demonstrates how various lighting conditions can be " <<
        "simulated: direct lighting, point lighting (with a lamp object) " <<
        "and spot lighting:\n" <<
        "keys A/D, left/right arrow keys control side camera movement\n" <<
        "keys W/S - up and down, arrows up/down - depth\n" <<
        "mouse can also be used to change view/zoom (scroll)\n" <<
        "----------------------------------------------------------------\n";

    if (argc > 1)
        process_input(win, argv[1]);
    else
        show_menu(win, argv[0]);

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
    static constexpr char num_options {'7'};
    const std::string s {inp};
    const char inp_char {s[0]};
    if (s.length() == 1 && inp_char >= '0' && inp_char < num_options) {
        switch (inp_char - '0') {
            case 6:
                illuminate_cubes(win, 6);
                break;
            case 5:
                illuminate_cubes(win, 5);
                break;
            case 4:
                illuminate_cubes(win, 4);
                break;
            case 3:
                illuminate_cubes(win, 3);
                break;
            case 2:
                illuminate_cubes(win, 2);
                break;
            case 1:
                illuminate_cubes(win, 1);
                break;
            case 0:
            default:
                illuminate_cubes(win, 0);
        }
    } else {
        std::cerr << "Wrong input: drawing default cubes\n";
        illuminate_cubes(win, 0);
    }
}

/*
 * Display a menu of possible actions
 */
void show_menu(GLFWwindow *win, const std::string &prog_name) {
    std::cout << "Note: the program can be run as follows:\n" <<
        prog_name << " int_param, where int_param is:\n" <<
        "0:\twooden boxes and 4 point lights (default)\n" <<
        "1:\twooden boxes with 4 point lights and a flashlight\n" <<
        "2:\twooden boxes with a flashlight and 4 rotating point lights\n" <<
        "3:\twooden boxes in desert environment\n" <<
        "4:\twooden boxes in factory environment\n" <<
        "5:\twooden boxes in horror environment\n" <<
        "6:\twooden boxes in biochemical lab environment\n";
    illuminate_cubes(win, 0);
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
    GLFWwindow *win = glfwCreateWindow(w, h, "Multiple lights", nullptr,
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
    glfwSetCursorPosCallback(win, mouse_callback);
    glfwSetScrollCallback(win, scroll_callback);

    // disable cursor
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // inform OpenGL about the size of the rendering window
    glViewport(0, 0, w, h);

    // enable depth testing for nice 3D output
    glEnable(GL_DEPTH_TEST);

    return win;
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
        main_cam.process_keyboard(Camera::forward_dir, delta_frame_time);
    else if (keys[GLFW_KEY_DOWN])
        main_cam.process_keyboard(Camera::backward_dir, delta_frame_time);
    else if (keys[GLFW_KEY_S])
        main_cam.process_keyboard(Camera::down_dir, delta_frame_time);
    else if (keys[GLFW_KEY_W])
        main_cam.process_keyboard(Camera::up_dir, delta_frame_time);
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

/*
 * Binding objects with vertex data.
 */
void make_objects(const GLuint VAO, const GLuint VBO,
        const std::vector<GLfloat> &vertices, const bool fill_VBO,
        const int option) {

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if (fill_VBO)
        glBufferData(GL_ARRAY_BUFFER, size_of_elements(vertices),
                vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
            (GLvoid*)0);
    glEnableVertexAttribArray(0);

    if (option > 0) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        if (option == 2) {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                    (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
        }
    }

    glBindVertexArray(0);
}

// exploiting std::vector to initialize several textures
void gen_textures(std::vector<GLuint> &tex) {
    for (auto &x: tex)
        glGenTextures(1, &x);
}

/*
 * Binding textures with the image data
 */
void make_textures(const GLuint tex, const std::string& img_fn,
        const std::vector<GLenum> &wrap, const std::vector<GLenum> &filter) {
    int img_w, img_h;
    unsigned char *img = SOIL_load_image(img_fn.c_str(), &img_w, &img_h, 0,
            SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w, img_h, 0, GL_RGB,
            GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter[1]);

    glBindTexture(GL_TEXTURE_2D, 0);
}


// draw a box enlighted by a diffused light
void illuminate_cubes(GLFWwindow *win, const int option) {
    static const std::vector<GLfloat> vertices {
        // pos            normals    tex coords
        -0.5, -0.5, -0.5,  0,  0, -1,  0, 0,
         0.5, -0.5, -0.5,  0,  0, -1,  1, 0,
         0.5,  0.5, -0.5,  0,  0, -1,  1, 1,
         0.5,  0.5, -0.5,  0,  0, -1,  1, 1,
        -0.5,  0.5, -0.5,  0,  0, -1,  0, 1,
        -0.5, -0.5, -0.5,  0,  0, -1,  0, 0,

        -0.5, -0.5,  0.5,  0,  0,  1,  0, 0,
         0.5, -0.5,  0.5,  0,  0,  1,  1, 0,
         0.5,  0.5,  0.5,  0,  0,  1,  1, 1,
         0.5,  0.5,  0.5,  0,  0,  1,  1, 1,
        -0.5,  0.5,  0.5,  0,  0,  1,  0, 1,
        -0.5, -0.5,  0.5,  0,  0,  1,  0, 0,

        -0.5,  0.5,  0.5, -1,  0,  0,  1, 0,
        -0.5,  0.5, -0.5, -1,  0,  0,  1, 1,
        -0.5, -0.5, -0.5, -1,  0,  0,  0, 1,
        -0.5, -0.5, -0.5, -1,  0,  0,  0, 1,
        -0.5, -0.5,  0.5, -1,  0,  0,  0, 0,
        -0.5,  0.5,  0.5, -1,  0,  0,  1, 0,

         0.5,  0.5,  0.5,  1,  0,  0,  1, 0,
         0.5,  0.5, -0.5,  1,  0,  0,  1, 1,
         0.5, -0.5, -0.5,  1,  0,  0,  0, 1,
         0.5, -0.5, -0.5,  1,  0,  0,  0, 1,
         0.5, -0.5,  0.5,  1,  0,  0,  0, 0,
         0.5,  0.5,  0.5,  1,  0,  0,  1, 0,

        -0.5, -0.5, -0.5,  0, -1,  0,  0, 1,
         0.5, -0.5, -0.5,  0, -1,  0,  1, 1,
         0.5, -0.5,  0.5,  0, -1,  0,  1, 0,
         0.5, -0.5,  0.5,  0, -1,  0,  1, 0,
        -0.5, -0.5,  0.5,  0, -1,  0,  0, 0,
        -0.5, -0.5, -0.5,  0, -1,  0,  0, 1,

        -0.5,  0.5, -0.5,  0,  1,  0,  0, 1,
         0.5,  0.5, -0.5,  0,  1,  0,  1, 1,
         0.5,  0.5,  0.5,  0,  1,  0,  1, 0,
         0.5,  0.5,  0.5,  0,  1,  0,  1, 0,
        -0.5,  0.5,  0.5,  0,  1,  0,  0, 0,
        -0.5,  0.5, -0.5,  0,  1,  0,  0, 1
    };
    switch (option) {
        case 6: case 5: case 4: case 3:
            draw_objects(win, vertices,
                    Shader {shad_path + "light_shader_direct_01.vs",
                    shad_path + "light_shader_multilight_03.frag"}, option);
            break;
        case 2:
            draw_objects(win, vertices,
                    Shader {shad_path + "light_shader_direct_01.vs",
                    shad_path + "light_shader_multilight_02.frag"}, option,
                    true);
            break;
        case 1:
            draw_objects(win, vertices,
                    Shader {shad_path + "light_shader_direct_01.vs",
                    shad_path + "light_shader_multilight_02.frag"}, option);
            break;
        case 0:
        default:
            draw_objects(win, vertices,
                    Shader {shad_path + "light_shader_direct_01.vs",
                    shad_path + "light_shader_multilight_01.frag"}, option);
    }
}

// helper function to draw lighting objects
void draw_objects(GLFWwindow *win, const std::vector<GLfloat> &vertices,
        const Shader& obj_shader, const int option, const bool rot_lamp) {

    Shader lamp_shader;
    if (option >= 3)
        lamp_shader = Shader {shad_path + "lamp_shader_01.vs",
            shad_path + "lamp_shader_02.frag"};
    else
        lamp_shader = Shader {shad_path + "lamp_shader_01.vs",
            shad_path + "lamp_shader_01.frag"};
    std::vector<std::string> tex_imgs {{tex_path + "container2.png"},
        {tex_path + "container2_specular.png"}};

    GLuint VAO_obj {}, VAO_lamp {}, VBO {};

    // VAOs and VBOs
    std::vector<GLuint> VAO_vec {VAO_obj, VAO_lamp};
    std::vector<GLuint> VBO_vec {VBO};

    gen_objects(&VAO_vec, &VBO_vec);
    make_objects(VAO_vec[0], VBO_vec[0], vertices, true, 2);
    make_objects(VAO_vec[1], VBO_vec[0], vertices, false, 0);

    // texture
    int num_tex {2};
    std::vector<GLuint> textures(num_tex);
    gen_textures(textures);
    for (int i {0}; i < num_tex; ++i)
        make_textures(textures[i], tex_imgs[i], {GL_REPEAT, GL_REPEAT},
                {GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_NEAREST});

    obj_shader.use();
    glUniform1i(glGetUniformLocation(obj_shader.id(), "mater.diffuse_map"), 0);
    glUniform1i(glGetUniformLocation(obj_shader.id(), "mater.specular_map"), 1);

    light_obj_loop(win, VAO_vec, textures, std::vector<Shader>{obj_shader,
            lamp_shader}, rot_lamp, option);
}

// main loop for drawing light objects
void light_obj_loop(GLFWwindow *win, const std::vector<GLuint> &VAO,
        const std::vector<GLuint> &tex_maps, const std::vector<Shader> &shad,
        const bool rot_lamp, const int option) {
    int win_w, win_h;
    glfwGetFramebufferSize(win, &win_w, &win_h);
    const auto win_asp = float(win_w) / win_h;
    //glm::vec3 lamp_pos {1, 0, -1.5};
    static std::vector<glm::vec3> lamps_pos = {
        glm::vec3{0.7, 0.2, 2}, glm::vec3{2.3, -3.3, -4},
        glm::vec3{-4, 2, -12}, glm::vec3{0, 0, -3}
    };
    // colors for different environments
    const std::vector<glm::vec3> clrs = environment_colors(option);
    while (!glfwWindowShouldClose(win)) {
        switch (option) {
            case 6: // biochemical lab
                glClearColor(0.9, 0.9, 0.9, 1);
                break;
            case 5: // horror
                glClearColor(0, 0, 0, 1);
                break;
            case 4: // factory
                glClearColor(0.1, 0.1, 0.1, 1);
                break;
            case 3: // desert
                glClearColor(0.75, 0.52, 0.3, 1);
                break;
            default:
                glClearColor(0.1, 0.1, 0.1, 1);
        };
        const auto curr_time = glfwGetTime();
        if (rot_lamp)
            for (GLuint i = 0; i < lamps_pos.size(); ++i) {
                lamps_pos[i].x = 1 + sin(curr_time) * 2;
                lamps_pos[i].y = sin(curr_time * 0.5);
            }
        delta_frame_time = curr_time - last_frame_time;
        last_frame_time  = curr_time;
        glfwPollEvents();
        do_movement();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        const auto view = main_cam.view_matrix();
        const auto proj = glm::perspective(main_cam.zoom(), win_asp, 0.1f,
                100.0f);
        draw_light_obj(shad[0], VAO[0], tex_maps, view, proj, lamps_pos,
                clrs, option);
        for (GLuint i = 0; i < lamps_pos.size(); ++i)
            draw_lamp(shad[1], VAO[1], view, proj, lamps_pos[i], clrs[i]);
        glfwSwapBuffers(win);
    }
}

// get environment_colors depending on the option (environment)
std::vector<glm::vec3> environment_colors(const int option) {
    static std::vector<glm::vec3> clrs; // colors for different environments
    switch (option) {
        case 6: // biochemical lab
            return {
                glm::vec3{0.4, 0.7, 0.1}, glm::vec3{0.4, 0.7, 0.1},
                    glm::vec3{0.4, 0.7, 0.1}, glm::vec3{0.4, 0.7, 0.1}
            };
        case 5: // horror
            return {
                glm::vec3{0.1, 0.1, 0.1}, glm::vec3{0.1, 0.1, 0.1},
                    glm::vec3{0.1, 0.1, 0.1}, glm::vec3{0.3, 0.1, 0.1}
            };
        case 4: // factory
            return {
                glm::vec3{0.2, 0.2, 0.6}, glm::vec3{0.3, 0.3, 0.7},
                    glm::vec3{0, 0, 0.3}, glm::vec3{0.4, 0.4, 0.4}
            };
        case 3: // desert
            return {
                glm::vec3{1, 0.6, 0}, glm::vec3{1, 0, 0},
                    glm::vec3{1, 1, 0}, glm::vec3{0.2, 0.2, 1}
            };
        default:
            return {
                glm::vec3{1, 1, 1}, glm::vec3{1, 1, 1},
                    glm::vec3{1, 1, 1}, glm::vec3{1, 1, 1}
            };
    };
}

// set directional light
void set_dir_light(const GLuint idx, const int option) {
    // directional light
    glUniform3f(glGetUniformLocation(idx, "dir_light.direction"),
            -0.2, -1, -0.3);
    switch (option) {
        case 6: // biochemical lab
            glUniform3f(glGetUniformLocation(idx, "dir_light.ambient"),
                    0.5, 0.5, 0.5);
            glUniform3f(glGetUniformLocation(idx, "dir_light.diffuse"),
                    1, 1, 1);
            glUniform3f(glGetUniformLocation(idx, "dir_light.specular"),
                    1, 1, 1);
            break;
        case 5: // horror
            glUniform3f(glGetUniformLocation(idx, "dir_light.ambient"),
                    0, 0, 0);
            glUniform3f(glGetUniformLocation(idx, "dir_light.diffuse"),
                    0.05, 0.05, 0.05);
            glUniform3f(glGetUniformLocation(idx, "dir_light.specular"),
                    0.2, 0.2, 0.2);
            break;
        case 4: // factory
            glUniform3f(glGetUniformLocation(idx, "dir_light.ambient"),
                    0.05, 0.05, 0.1);
            glUniform3f(glGetUniformLocation(idx, "dir_light.diffuse"),
                    0.2, 0.2, 0.7);
            glUniform3f(glGetUniformLocation(idx, "dir_light.specular"),
                    0.7, 0.7, 0.7);
            break;
        case 3: // desert
            glUniform3f(glGetUniformLocation(idx, "dir_light.ambient"),
                    0.3, 0.24, 0.14);
            glUniform3f(glGetUniformLocation(idx, "dir_light.diffuse"),
                    0.7, 0.42, 0.26);
            glUniform3f(glGetUniformLocation(idx, "dir_light.specular"),
                    0.5, 0.5, 0.5);
            break;
        default:
            glUniform3f(glGetUniformLocation(idx, "dir_light.ambient"),
                    0.05, 0.05, 0.05);
            glUniform3f(glGetUniformLocation(idx, "dir_light.diffuse"),
                    0.4, 0.4, 0.4);
            glUniform3f(glGetUniformLocation(idx, "dir_light.specular"),
                    0.5, 0.5, 0.5);
    };
}

// set the point lights (lamps) in a separate function; idx: shader program id
void set_point_lights(const GLuint idx, const std::vector<glm::vec3> &lamps_pos,
        const std::vector<glm::vec3> &clrs, const int option) {
    if (clrs.size() != lamps_pos.size())
        throw std::runtime_error{"number of lamps != number of colors"};
    for (GLuint i = 0; i < lamps_pos.size(); ++i) {
        const std::string ins {"point_lights[" + std::to_string(i) + "]"};
        glUniform3f(glGetUniformLocation(idx, (ins + ".pos").c_str()),
                lamps_pos[i].x, lamps_pos[i].y, lamps_pos[i].z);
        if (option >= 3) {
            glUniform3f(glGetUniformLocation(idx, (ins + ".ambient").c_str()),
                    clrs[i].x * 0.1, clrs[i].y * 0.1, clrs[i].z * 0.1);
            glUniform3f(glGetUniformLocation(idx, (ins + ".diffuse").c_str()),
                    clrs[i].x, clrs[i].y, clrs[i].z);
            glUniform3f(glGetUniformLocation(idx, (ins + ".specular").c_str()),
                    clrs[i].x, clrs[i].y, clrs[i].z);
            glUniform3f(glGetUniformLocation(idx, "light_color"),
                    clrs[i].x, clrs[i].y, clrs[i].z);
        } else {
            glUniform3f(glGetUniformLocation(idx, (ins + ".ambient").c_str()),
                    0.05, 0.05, 0.05);
            glUniform3f(glGetUniformLocation(idx, (ins + ".diffuse").c_str()),
                    0.8, 0.8, 0.8);
            glUniform3f(glGetUniformLocation(idx, (ins + ".specular").c_str()),
                    1, 1, 1);
        }
        glUniform1f(glGetUniformLocation(idx, (ins + ".constant_term").c_str()),
                1);
        if (option == 4) {
            if (i == 2) {
                glUniform1f(glGetUniformLocation(idx, (ins +
                                ".linear_term").c_str()), 0.22);
                glUniform1f(glGetUniformLocation(idx,
                            (ins + ".quadratic_term").c_str()), 0.2);
            } else {
                glUniform1f(glGetUniformLocation(idx, (ins +
                                ".linear_term").c_str()), 0.14);
                glUniform1f(glGetUniformLocation(idx,
                            (ins + ".quadratic_term").c_str()), 0.07);
            }
        } else if (option == 6) {
            glUniform1f(glGetUniformLocation(idx, (ins +
                            ".linear_term").c_str()), 0.07);
            glUniform1f(glGetUniformLocation(idx,
                        (ins + ".quadratic_term").c_str()), 0.017);
        } else {
            glUniform1f(glGetUniformLocation(idx, (ins +
                            ".linear_term").c_str()), 0.09);
            glUniform1f(glGetUniformLocation(idx,
                        (ins + ".quadratic_term").c_str()), 0.032);
        }
    }
}

// set the spot light (flashlight)
void set_spot_light(const GLuint idx, const int option) {
    // idx: shader program's id
    glUniform3f(glGetUniformLocation(idx, "spot_light.pos"),
            main_cam.pos().x, main_cam.pos().y, main_cam.pos().z);
    glUniform3f(glGetUniformLocation(idx, "spot_light.direction"),
            main_cam.front().x, main_cam.front().y, main_cam.front().z);
    glUniform3f(glGetUniformLocation(idx, "spot_light.ambient"), 0, 0, 0);
    switch (option) {
        case 6: // biochemical lab
            glUniform3f(glGetUniformLocation(idx, "spot_light.diffuse"),
                    0, 1, 0);
            glUniform3f(glGetUniformLocation(idx, "spot_light.specular"),
                    0, 1, 0);
            break;
        case 5: // horror
        case 4: // factory
            glUniform3f(glGetUniformLocation(idx, "spot_light.diffuse"),
                    1, 1, 1);
            glUniform3f(glGetUniformLocation(idx, "spot_light.specular"),
                    1, 1, 1);
            break;
        case 3: // desert
            glUniform3f(glGetUniformLocation(idx, "spot_light.diffuse"),
                    0.8, 0.8, 0.8);
            glUniform3f(glGetUniformLocation(idx, "spot_light.specular"),
                    0.8, 0.8, 0.8);
            break;
        default:
            glUniform3f(glGetUniformLocation(idx, "spot_light.diffuse"),
                    1, 1, 1);
            glUniform3f(glGetUniformLocation(idx, "spot_light.specular"),
                    1, 1, 1);
    };
    if (option == 6) {
        glUniform1f(glGetUniformLocation(idx, "spot_light.constant_term"), 1);
        glUniform1f(glGetUniformLocation(idx, "spot_light.linear_term"), 0.07);
        glUniform1f(glGetUniformLocation(idx, "spot_light.quadratic_term"),
                0.017);
    } else {
        glUniform1f(glGetUniformLocation(idx, "spot_light.constant_term"), 1);
        glUniform1f(glGetUniformLocation(idx, "spot_light.linear_term"), 0.09);
        glUniform1f(glGetUniformLocation(idx, "spot_light.quadratic_term"),
                0.032);
    }
    glUniform1f(glGetUniformLocation(idx, "spot_light.cutoff"),
            glm::cos(glm::radians(12.5)));
    glUniform1f(glGetUniformLocation(idx, "spot_light.outer_cutoff"),
            glm::cos(glm::radians(15.5)));
}

// helper function for drawing colored object
void draw_light_obj(const Shader &shad, const GLuint VAO,
        const std::vector<GLuint> &tex_maps, const glm::mat4 &view,
        const glm::mat4 &proj, const std::vector<glm::vec3> &lamps_pos,
        const std::vector<glm::vec3> &clrs, const int option) {
    shad.use();
    const auto idx = shad.id();
    glUniform3f(glGetUniformLocation(idx, "view_pos"),
            main_cam.pos().x, main_cam.pos().y, main_cam.pos().z);
    glUniform1f(glGetUniformLocation(idx, "mater.shininess"), 32);

    // set lighting
    set_dir_light(idx, option);
    set_point_lights(idx, lamps_pos, clrs, option);
    set_spot_light(idx, option);

    // matrices
    glUniformMatrix4fv(glGetUniformLocation(idx, "view"), 1, GL_FALSE,
            glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(idx, "proj"), 1, GL_FALSE,
            glm::value_ptr(proj));

    for (size_t i {0}; i < tex_maps.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, tex_maps[i]);
    }

    const static std::vector<glm::vec3> cubes_pos = {
        glm::vec3{ 0.0,  0.0,  0.0}, glm::vec3{ 2.0,  5.0, -15.0},
        glm::vec3{-1.5, -2.2, -2.5}, glm::vec3{-3.8, -2.0, -12.3},
        glm::vec3{ 2.4, -0.4, -3.5}, glm::vec3{-1.7,  3.0, -7.5},
        glm::vec3{ 1.3, -2.0, -2.5}, glm::vec3{ 1.5,  2.0, -2.5},
        glm::vec3{ 1.5,  0.2, -1.5}, glm::vec3{-1.3,  1.0, -1.5}
    };

    const GLuint model_loc = glGetUniformLocation(idx, "model");
    glm::mat4 model;
    glBindVertexArray(VAO);
    for (GLuint i = 0; i < cubes_pos.size(); ++i) {
        model = glm::translate(glm::mat4{1}, cubes_pos[i]);
        model = glm::rotate(model, 20.0f * i, glm::vec3{1, 0.3, 0.5});
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0 , 36);
    }
    glBindVertexArray(0);
}

// helper function for drawing "lamp" object
void draw_lamp(const Shader &shad, const GLuint VAO, const glm::mat4 &view,
        const glm::mat4 &proj, const glm::vec3 &lamp_pos,
        const glm::vec3 &lamp_color) {
    shad.use();
    const auto idx = shad.id();

    glUniform3f(glGetUniformLocation(idx, "lamp_color"),
            lamp_color.x, lamp_color.y, lamp_color.z);

    glm::mat4 model = glm::translate(glm::mat4{1}, lamp_pos);
    model = glm::scale(model, glm::vec3{0.2});

    glUniformMatrix4fv(glGetUniformLocation(idx, "view"), 1, GL_FALSE,
            glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(idx, "proj"), 1, GL_FALSE,
            glm::value_ptr(proj));
    glUniformMatrix4fv(glGetUniformLocation(idx, "model"), 1, GL_FALSE,
            glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

