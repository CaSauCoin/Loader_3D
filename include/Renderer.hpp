#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shape.hpp"

class Renderer {
private:
    GLuint shaderProgram, VAO, VBO, EBO;
    GLFWwindow* window;

    void compileShaders();

public:
    Renderer(GLFWwindow* win);
    ~Renderer();
    void setupBuffers(const Shape& shape);
    void render(const Shape& shape);
};