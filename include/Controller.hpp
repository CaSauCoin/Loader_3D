#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "Shape.hpp"

class Controller {
private:
    GLFWwindow* window;
    Shape& shape;
    bool rotating = false;
    glm::vec2 lastMousePos;
    bool isCube = true;
    bool toggleShape = false; // Track button press for one frame

public:
    Controller(GLFWwindow* win, Shape& s) : window(win), shape(s) {}
    void handleMouseInput();
    void renderImGui();
    bool isCubeShape() const { return isCube; }
    bool shouldToggleShape() const { return toggleShape; }
    void clearToggle() { toggleShape = false; }
    void setCubeShape(bool value) { isCube = value; }
};