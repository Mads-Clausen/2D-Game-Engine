/******************************************************************
 * <insert name>   :    Game Engine produced by Mads Clausen
 * www             :    http://anickyan.koding.com/ge
 *
 * Game engine using OpenGL 3.2 for hardware-accelerated
 * graphics, for the best performance.
 *
 * Copyright (c) Mads Clausen :: All Rights Reserved
 ******************************************************************/

#include <iostream>

#include "GL/SOIL.h"

#include "math/vec3.hpp"
#include "math/mat3.hpp"
#include "setup.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Texture.hpp"

int main()
{
    if(!setup::initOpenGL())
        return -1;

    if(!setup::openWindow("Testing", 600, 600))
        return -1;

    GLfloat vertices[] =
    {
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f
    };

    std::vector<GLfloat> UV =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    Shader shader("", "");
    shader.compile("#version 330 core\nlayout(location = 0) in vec3 vertexPosition_modelspace;\nlayout(location = 1) in vec2 vertexUV;\nout vec2 UV;\nvoid main() { gl_Position.xyz = vertexPosition_modelspace;gl_Position.w = 1.0;\nUV = vertexUV; }",
                   "#version 330 core\nout vec3 color;\nin vec2 UV;\nuniform sampler2D tex_sampler;\nvoid main() { color = texture(tex_sampler, UV).rgb; }");

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Texture tex(UV);
    tex.load("test.jpg");

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.bind();
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

            tex.push(1, glGetUniformLocation(shader.getID(), "tex_sampler"));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            tex.pop();

            glDisableVertexAttribArray(0);
        shader.unbind();

        glfwSwapBuffers();
    } while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));

    setup::done();

    return 0;
}
