#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shape.hpp"

enum class BackgroundMode { White, Dark, Gray };

class Renderer {
private:
    GLuint shaderProgram, VAO, VBO, EBO;
    GLFWwindow* window;
    BackgroundMode backgroundMode = BackgroundMode::White;
    bool useRainbowEffect = false;
    float currentTime = 0.0f;

    void compileShaders();

public:
    Renderer(GLFWwindow* win);
    ~Renderer();
    void setupBuffers(const Shape& shape);
    void render(const Shape& shape);
    BackgroundMode getBackgroundMode() const { return backgroundMode; }
    void setBackgroundMode(BackgroundMode mode) { backgroundMode = mode; }
    bool getRainbowEffect() const { return useRainbowEffect; }
    void setRainbowEffect(bool value) { useRainbowEffect = value; }
    void updateTime(float time) { currentTime = time; }
};