#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "Vertex.hpp"

class CoordinateSystem {
private:
    GLuint shaderProgram, VAO, VBO;
    std::vector<Vertex> vertices;
    int selectedAxis = -1; // -1: none, 0: X, 1: Y, 2: Z
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    void compileShaders();

public:
    CoordinateSystem();
    ~CoordinateSystem();
    void setup();
    void render(int display_w, int display_h, const glm::mat4& view, const glm::mat4& projection);
    int getSelectedAxis() const { return selectedAxis; }
    void setSelectedAxis(int axis) { selectedAxis = axis; }
    bool selectAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDir, float& t);
    glm::quat getRotation() const { return rotation; }
    void applyRotation(const glm::quat& rot) { rotation = glm::normalize(rot * rotation); }
    void resetRotation() { rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); }
};