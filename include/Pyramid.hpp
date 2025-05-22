#pragma once
#include "Shape.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

class Pyramid : public Shape {
private:
    float size = 1.0f;
    bool filled = true;
    glm::vec3 colors[4] = {
        {1.0f, 0.0f, 0.0f}, // Base face 1 - red
        {0.0f, 1.0f, 0.0f}, // Base face 2 - green
        {0.0f, 0.0f, 1.0f}, // Side face 1 - blue
        {1.0f, 1.0f, 0.0f}  // Side face 2 - yellow
    };
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

public:
    void setupVertices() override;
    void resetRotation() override;
    float& getSize() override { return size; }
    bool isFilled() const override { return filled; }
    void setFilled(bool value) override { filled = value; }
    glm::vec3* getColors() override { return colors; }
    const std::vector<Vertex>& getVertices() const override { return vertices; }
    const std::vector<unsigned int>& getIndices() const override { return indices; }
    glm::mat4 getModelMatrix() const override { return glm::mat4_cast(rotation); }
    void applyRotation(const glm::quat& rot) override { rotation = glm::normalize(rot * rotation); }
};