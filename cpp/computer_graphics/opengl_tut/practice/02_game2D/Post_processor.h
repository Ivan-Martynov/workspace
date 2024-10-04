#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include "Shader.h"
#include "Texture.h"
#include "Sprite_renderer.h"

class Post_processor {
public:

    Shader post_process_shader_;
    Texture2D texture_;
    GLuint width_, height_;
    GLboolean is_confuse_, is_chaos_, is_shake_;

    Post_processor() = default;
    Post_processor(const Shader&, const GLuint, const GLuint);

    explicit Post_processor(const Post_processor&) = default;
    Post_processor& operator=(const Post_processor&) = default;
    explicit Post_processor(Post_processor&&) = default;
    Post_processor& operator=(Post_processor&&) = default;

    ~Post_processor() = default;

    // prepare postprocessing framebuffer operations
    void begin_render();
    // call after rendering the game
    void end_render();
    // render the post processor texture quad
    void render(const GLfloat);

private:
    GLuint msfbo_, fbo_; // multisampled fbo
    GLuint rbo_; // for multisampled color buffer
    GLuint vao_;
    // initialize quad for rendering postprocessing texture
    void init_render_data();
};

Post_processor::Post_processor(const Shader& shad, const GLuint w,
        const GLuint h): post_process_shader_ {shad}, texture_ {}, width_ {w},
    height_ {h}, is_confuse_ {GL_FALSE}, is_chaos_ {GL_FALSE},
    is_shake_ {GL_FALSE}, msfbo_ {}, fbo_ {}, rbo_ {}, vao_ {} {
        // initialize renderbuffer/framebuffer objects
        glGenFramebuffers(1, &msfbo_);
        glGenFramebuffers(1, &fbo_);
        glGenRenderbuffers(1, &rbo_);
        glBindFramebuffer(GL_FRAMEBUFFER, msfbo_);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width_,
                height_);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_RENDERBUFFER, rbo_);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR::Post_processor: Failed to initialize MSFBO\n";

        // initialize fbo/texture to blit multisampled color buffer to
        glBindFramebuffer(GL_FRAMEBUFFER, msfbo_);
        texture_.generate(width_, height_, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D, texture_.id, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR::Post_processor: Failed to initialize FBO\n";

        // initialize render data and uniforms
        init_render_data();
        post_process_shader_.use();
        const auto shad_id = post_process_shader_.id();
        post_process_shader_.set_integer("scene", 0);
        const GLfloat offset_val {1.0f / 300};
        const GLfloat offsets[9][2] = {
            { -offset_val,  offset_val }, // top-left
            {           0,  offset_val }, // top-center
            {  offset_val,  offset_val }, // top-right
            { -offset_val,           0 }, // center-left
            {           0,           0 }, // center-center
            {  offset_val,           0 }, // center-right
            { -offset_val, -offset_val }, // bottom-left
            {           0, -offset_val }, // bottom-center
            {  offset_val, -offset_val }  // bottom-right
        };

        glUniform2fv(glGetUniformLocation(shad_id, "offsets"), 9,
                (GLfloat*)offsets);
        const GLint edge_kernel[9] = {
            -1, -1, -1,
            -1 , 8, -1,
            -1, -1, -1
        };
        glUniform1iv(glGetUniformLocation(shad_id, "edge_kernel"), 9,
                edge_kernel);
        const GLfloat blur_kernel[9] = {
            1.0f / 16, 2.0f / 16, 1.0f / 16,
            2.0f / 16, 4.0f / 16, 2.0f / 16,
            1.0f / 16, 2.0f / 16, 1.0f / 16
        };
        glUniform1fv(glGetUniformLocation(shad_id, "blur_kernel"), 9,
                blur_kernel);
    }

void Post_processor::begin_render() {
    glBindFramebuffer(GL_FRAMEBUFFER, msfbo_);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Post_processor::end_render() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, msfbo_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
    glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_,
            GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Post_processor::render(const GLfloat dt) {
    post_process_shader_.use();
    post_process_shader_.set_float("time_val", dt);
    post_process_shader_.set_integer("is_confuse", is_confuse_);
    post_process_shader_.set_integer("is_chaos", is_chaos_);
    post_process_shader_.set_integer("is_shake", is_shake_);
    glActiveTexture(GL_TEXTURE0);
    texture_.bind();
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Post_processor::init_render_data() {
    GLuint vbo;
    const GLfloat vertices[] = {
        -1, -1, 0, 0,
         1,  1, 1, 1,
        -1,  1, 0, 1,

        -1, -1, 0, 0,
         1, -1, 1, 0,
         1,  1, 1, 1
    };
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) << 2,
            (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

#endif /* POST_PROCESSOR_H */

