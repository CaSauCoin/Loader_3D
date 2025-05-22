#pragma once
#include "Shape.hpp"
#include "Cube.hpp"
#include "Pyramid.hpp"
#include "Renderer.hpp"
#include "Controller.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Application {
private:
    GLFWwindow* window;
    Shape* shape;
    Renderer* renderer;
    Controller* controller;
    float prevSize = 1.0f;
    bool prevFilled = true;
    glm::vec3 prevColors[6];
    bool prevIsCube = true;
    int prevColorCount = 6;
    
    void initGLFW();
    void initGLEW();
    void initImGui();

public:
    Application();
    ~Application();
    void run();
};