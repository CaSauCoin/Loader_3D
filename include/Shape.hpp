#pragma once
#include "Vertex.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

class Shape {
public:
    virtual ~Shape() = default;
    virtual void setupVertices() = 0;
    virtual void resetRotation() = 0;
    virtual float& getSize() = 0;
    virtual bool isFilled() const = 0;
    virtual void setFilled(bool value) = 0;
    virtual glm::vec3* getColors() = 0;
    virtual const std::vector<Vertex>& getVertices() const = 0;
    virtual const std::vector<unsigned int>& getIndices() const = 0;
    virtual glm::mat4 getModelMatrix() const = 0;
    virtual void applyRotation(const glm::quat& rot) = 0;
};