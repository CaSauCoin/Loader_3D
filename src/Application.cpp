#include "Application.hpp"
#include <GL/glew.h>
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
    shape = new Cube();
    renderer = new Renderer(window);
    controller = new Controller(window, *shape, *renderer);
    shape->setupVertices();
    renderer->setupBuffers(*shape);
    std::cout << "Initialized Cube with " << shape->getVertices().size() << " vertices, " << shape->getIndices().size() << " indices\n";
    for (int i = 0; i < prevColorCount; ++i) {
        prevColors[i] = shape->getColors()[i];
    }
}

Application::~Application() {
    delete shape;
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

        if (controller) {
            controller->handleMouseInput();
            controller->renderImGui();
        } else {
            std::cerr << "Controller is null!\n";
        }

        renderer->updateTime(static_cast<float>(glfwGetTime()));

        if (controller && controller->shouldToggleShape()) {
            std::cout << "Processing toggle: isCube = " << controller->isCubeShape() << "\n";
            delete shape;
            shape = controller->isCubeShape() ? static_cast<Shape*>(new Cube()) : static_cast<Shape*>(new Pyramid());
            if (!shape) {
                std::cerr << "Failed to create new shape!\n";
                exit(-1);
            }
            prevIsCube = controller->isCubeShape();
            prevColorCount = prevIsCube ? 6 : 4;
            shape->setupVertices();
            renderer->setupBuffers(*shape);
            std::cout << "Switched to " << (prevIsCube ? "Cube" : "Pyramid") << " with "
                      << shape->getVertices().size() << " vertices, "
                      << shape->getIndices().size() << " indices\n";
            prevSize = shape->getSize();
            prevFillMode = shape->getFillMode();
            for (int i = 0; i < prevColorCount; ++i) {
                prevColors[i] = shape->getColors()[i];
            }
            delete controller;
            controller = new Controller(window, *shape, *renderer);
            controller->setCubeShape(prevIsCube);
            if (!controller) {
                std::cerr << "Failed to create new controller!\n";
                exit(-1);
            }
            controller->clearToggle();
        }

        bool needsUpdate = (shape->getSize() != prevSize || shape->getFillMode() != prevFillMode);
        int colorCount = prevIsCube ? 6 : 4;
        for (int i = 0; i < colorCount; ++i) {
            if (shape->getColors()[i] != prevColors[i]) {
                needsUpdate = true;
                prevColors[i] = shape->getColors()[i];
            }
        }

        if (needsUpdate) {
            shape->setupVertices();
            renderer->setupBuffers(*shape);
            prevSize = shape->getSize();
            prevFillMode = shape->getFillMode();
        }

        if (shape) {
            renderer->render(*shape);
        } else {
            std::cerr << "Shape is null!\n";
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}