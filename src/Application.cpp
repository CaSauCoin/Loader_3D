#include <GL/glew.h>
#include "Application.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

void Application::initGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1280, 720, "ImGui Rubik's Cube Block", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window!" << std::endl;
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

void Application::initGLEW() {
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        glfwTerminate();
        exit(-1);
    }
}

void Application::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

Application::Application() {
    initGLFW();
    initGLEW();
    initImGui();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    renderer = new Renderer(window);
    controller = new Controller(window, cube);
    cube.setupVertices();
    renderer->setupBuffers(cube);
    for (int i = 0; i < 6; ++i) {
        prevColors[i] = cube.getColors()[i];
    }
}

Application::~Application() {
    delete renderer;
    delete controller;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        controller->handleMouseInput();
        controller->renderImGui();

        bool needsUpdate = (cube.getSize() != prevSize || cube.isFilled() != prevFilled);
        for (int i = 0; i < 6; ++i) {
            if (cube.getColors()[i] != prevColors[i]) {
                needsUpdate = true;
                prevColors[i] = cube.getColors()[i];
            }
        }

        if (needsUpdate) {
            cube.setupVertices();
            renderer->setupBuffers(cube);
            prevSize = cube.getSize();
            prevFilled = cube.isFilled();
        }

        renderer->render(cube);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}