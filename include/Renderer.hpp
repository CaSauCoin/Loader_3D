#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.hpp"

class Renderer {
private:
    GLuint shaderProgram, VAO, VBO, EBO;
    GLFWwindow* window;

    void compileShaders();

public:
    Renderer(GLFWwindow* win);
    ~Renderer();
    void setupBuffers(const Cube& cube);
    void render(const Cube& cube);
};