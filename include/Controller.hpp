#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "Cube.hpp"

class Controller {
private:
    GLFWwindow* window;
    Cube& cube;
    bool rotating = false;
    glm::vec2 lastMousePos;

public:
    Controller(GLFWwindow* win, Cube& c) : window(win), cube(c) {}
    void handleMouseInput();
    void renderImGui();
};