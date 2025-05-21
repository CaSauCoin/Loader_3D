#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include "Vertex.hpp"

class Cube {
private:
    float size = 1.0f;
    bool filled = true;
    glm::vec3 colors[6] = {
        {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}
    };
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

public:
    void setupVertices();
    void resetRotation();
    float& getSize() { return size; }
    bool isFilled() const { return filled; }
    void setFilled(bool value) { filled = value; }
    glm::vec3* getColors() { return colors; }
    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }
    glm::mat4 getModelMatrix() const { return glm::mat4_cast(rotation); }
    void applyRotation(const glm::quat& rot) { rotation = glm::normalize(rot * rotation); }
};