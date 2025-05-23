#pragma once
#include "Shape.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

class Pyramid : public Shape {
private:
    float size = 1.0f;
    FillMode fillMode = FillMode::Solid;
    glm::vec3 colors[4] = {
        {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}
    };
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

public:
    void setupVertices() override;
    void resetRotation() override;
    float& getSize() override { return size; }
    FillMode getFillMode() const override { return fillMode; }
    void setFillMode(FillMode mode) override { fillMode = mode; }
    glm::vec3* getColors() override { return colors; }
    const std::vector<Vertex>& getVertices() const override { return vertices; }
    const std::vector<unsigned int>& getIndices() const override { return indices; }
    glm::mat4 getModelMatrix() const override { return glm::mat4_cast(rotation); }
    void applyRotation(const glm::quat& rot) override { rotation = glm::normalize(rot * rotation); }
};