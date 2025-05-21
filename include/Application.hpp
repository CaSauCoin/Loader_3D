#pragma once
#include "Cube.hpp"
#include "Renderer.hpp"
#include "Controller.hpp"
#include <GLFW/glfw3.h>


class Application {
private:
    GLFWwindow* window;
    Cube cube;
    Renderer* renderer;
    Controller* controller;
    float prevSize = 1.0f;
    bool prevFilled = true;
    glm::vec3 prevColors[6];
    
    void initGLFW();
    void initGLEW();
    void initImGui();

public:
    Application();
    ~Application();
    void run();
};