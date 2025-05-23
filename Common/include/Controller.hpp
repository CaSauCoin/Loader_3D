#pragma once
#include <imgui.h>
#include "Shape.hpp"
#include "Renderer.hpp"
#include <GLFW/glfw3.h>

class Controller {
private:
    GLFWwindow* window;
    Shape& shape;
    Renderer& renderer;
    bool rotating = false;
    glm::vec2 lastMousePos;
    bool isCube = true;
    bool toggleShape = false;

public:
    Controller(GLFWwindow* win, Shape& s, Renderer& r) : window(win), shape(s), renderer(r) {}
    void handleMouseInput();
    void renderImGui();
    bool isCubeShape() const { return isCube; }
    bool shouldToggleShape() const { return toggleShape; }
    void clearToggle() { toggleShape = false; }
    void setCubeShape(bool value) { isCube = value; }
};