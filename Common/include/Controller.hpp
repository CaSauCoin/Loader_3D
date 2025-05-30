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
    bool rotatingOxyz = false;
    glm::vec2 lastMousePos;
    int shapeType = 0; // 0: Cube, 1: Pyramid, 2: Globular
    bool toggleShape = false;
    bool autoRotate = false;
    float rotateSpeed = 0.5f;
    float snapThreshold = 20.0f;
    bool coordSystemMode = false;
    FillMode prevFillMode = FillMode::Solid;
    int selectedAxis = -1; // -1: none, 0: X, 1: Y, 2: Z

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
    bool getCoordSystemMode() const { return coordSystemMode; }
    void setCoordSystemMode(bool value) { coordSystemMode = value; }
    int getSelectedAxis() const { return selectedAxis; }
    void setSelectedAxis(int axis) { selectedAxis = axis; }
};