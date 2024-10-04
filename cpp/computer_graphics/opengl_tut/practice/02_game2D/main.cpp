/*
 * Following tutorial on OpenGL (materials):
 *
 * http://learnopengl.com/#!In-Practice/2D-Game
 *
 */

#define GLEW_STATIC

#include "Game.h"
#include "Resource_manager.h"
#include "Game_level.h"
#include <GLFW/glfw3.h>

// initialize stuff
GLFWwindow* init(const GLuint, const GLuint);

// cleaning up
int clean_up(const int);

// callback functions
void key_callback(GLFWwindow*, const int, const int, const int, const int);

// game loop
void game_loop(GLFWwindow*);

// initialize the game as a global variable
static constexpr GLuint scr_width {800}, scr_height {600};
Game breakout {scr_width, scr_height};

// here goes the main()
int main() try {

    GLFWwindow *win = init(scr_width, scr_height);
    breakout.init();
    //breakout.state = Game_state::active;

    game_loop(win);

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
    GLFWwindow *win = glfwCreateWindow(w, h, "Breakout", nullptr, nullptr);
    if (win == nullptr)
        throw std::runtime_error {"Failed to create GLFW window"};

    glfwMakeContextCurrent(win);

    // let GLEW know to use a modern approach to retrieving function pointers
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw std::runtime_error {"Failed to initialize GLEW"};

    // call this once to catch glewInit() bug
    glGetError();

    // register callbacks
    glfwSetKeyCallback(win, key_callback);

    // configure OpenGL options
    glViewport(0, 0, w, h);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return win;
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
 * Call this function whenever a key is pressed / released
 */
void key_callback(GLFWwindow *win, const int key, const int,
        const int action, const int) {
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
        glfwSetWindowShouldClose(win, GL_TRUE);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            breakout.keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            breakout.keys[key] = false;
    }
}

// put several calls for a step in the drawing loop into one function
//void prepare_buffer(GLfloat& delta_frame_time, GLfloat& last_frame_time) {
//    const auto curr_time = glfwGetTime();
//    delta_frame_time = curr_time - last_frame_time;
//    last_frame_time  = curr_time;
//    glfwPollEvents();
//    glClearColor(0.2, 0.2, 0.2, 1);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}


// main game loop
void game_loop(GLFWwindow* win) {

    GLfloat delta_frame_time {0.0}, last_frame_time {0.0};

    while (!glfwWindowShouldClose(win)) {
        const auto curr_time = glfwGetTime();
        delta_frame_time = curr_time - last_frame_time;
        last_frame_time  = curr_time;
        glfwPollEvents();


        // manager user input
        breakout.process_input(delta_frame_time);

        // update game state
        breakout.update(delta_frame_time);

        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        breakout.render();

        glfwSwapBuffers(win);
    }
}

