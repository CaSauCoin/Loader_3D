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
    int shapeType = 0;
    bool toggleShape = false;
    bool autoRotate = false; 
    float rotateSpeed = 0.0075f;
    float snapThreshold = 20.0f;

public:
    Controller(GLFWwindow* win, Shape& s, Renderer& r) : window(win), shape(s), renderer(r) {}
    void handleMouseInput();
    void renderImGui();
    int getShapeType() const { return shapeType; }
    bool shouldToggleShape() const { return toggleShape; }
    void clearToggle() { toggleShape = false; }
    void setShapeType(int type) { shapeType = type; }
    bool getAutoRotate() const { return autoRotate; }
    void setAutoRotate(bool value) { autoRotate = value; }
    float getRotateSpeed() const { return rotateSpeed; }
    void setRotateSpeed(float speed) { rotateSpeed = speed; }
};