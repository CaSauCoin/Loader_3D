#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Vertex.hpp"
#include <iostream>

enum class FillMode { Solid, Wireframe, Dotted };

class Shape {
public:
    virtual ~Shape() = default;
    virtual void setupVertices() = 0;
    virtual void resetRotation() = 0;
    virtual float& getSize() = 0;
    virtual FillMode getFillMode() const = 0;
    virtual void setFillMode(FillMode mode) = 0;
    virtual glm::vec3* getColors() = 0;
    virtual const std::vector<Vertex>& getVertices() const = 0;
    virtual const std::vector<unsigned int>& getIndices() const = 0;
    virtual glm::mat4 getModelMatrix() const = 0;
    virtual void applyRotation(const glm::quat& rot) = 0;
};