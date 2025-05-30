#pragma once
#include "Shape.hpp"
#include "CoordinateSystem.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum class BackgroundMode { White, Dark, Gray };

class Renderer {
private:
    GLuint shaderProgram, VAO, VBO, EBO;
    GLFWwindow* window;
    BackgroundMode backgroundMode = BackgroundMode::White;
    bool useRainbowEffect = false;
    float currentTime = 0.0f;
    CoordinateSystem coordinateSystem;
    bool coordSystemMode = false;

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
    bool getCoordSystemMode() const { return coordSystemMode; }
    void setCoordSystemMode(bool value) { coordSystemMode = value; }
    int getSelectedAxis() const { return coordinateSystem.getSelectedAxis(); }
    bool selectAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDir, float& t) {
        bool hit = coordinateSystem.selectAxis(rayOrigin, rayDir, t);
        if (hit) coordinateSystem.setSelectedAxis(coordinateSystem.getSelectedAxis());
        return hit;
    }
    void applyOxyzRotation(const glm::quat& rot) { coordinateSystem.applyRotation(rot); }
    void resetOxyzRotation() { coordinateSystem.resetRotation(); }
    glm::quat getOxyzRotation() const { return coordinateSystem.getRotation(); }
};