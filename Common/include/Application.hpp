#pragma once
#include "Shape.hpp"
#include "Cube.hpp"
#include "Pyramid.hpp"
#include "Globular.hpp"
#include "Renderer.hpp"
#include "Controller.hpp"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Application {
private:
    GLFWwindow* window;
    Shape* shape;
    Renderer* renderer;
    Controller* controller;
    float prevSize = 1.0f;
    FillMode prevFillMode = FillMode::Solid;
    glm::vec3 prevColors[6];
    int prevShapeType = 0;
    int prevColorCount = 6;

    void initGLFW();
    void initGLEW();
    void initImGui();

public:
    Application();
    ~Application();
    void run();
};