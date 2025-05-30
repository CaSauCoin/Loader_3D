#include "Pyramid.hpp"

void Pyramid::setupVertices() {
    vertices.clear();
    indices.clear();

    float halfSize = size * 0.5f;
    glm::vec3 center(1.0f, 1.0f, 1.0f); // Center at (1, 1, 1)

    // Define 5 vertices of the pyramid, centered at (1, 1, 1)
    glm::vec3 v[] = {
        center + glm::vec3(-halfSize, -halfSize, -halfSize), // 0: Base
        center + glm::vec3( halfSize, -halfSize, -halfSize), // 1: Base
        center + glm::vec3( halfSize, -halfSize,  halfSize), // 2: Base
        center + glm::vec3(-halfSize, -halfSize,  halfSize), // 3: Base
        center + glm::vec3( 0.0f,  halfSize,  0.0f)          // 4: Apex
    };

    // Define normals
    glm::vec3 normals[] = {
        glm::vec3( 0.0f, -1.0f,  0.0f), // Base
        glm::normalize(glm::vec3(-1.0f,  1.0f,  0.0f)), // Side 0-4-3
        glm::normalize(glm::vec3( 1.0f,  1.0f,  0.0f)), // Side 1-4-2
        glm::normalize(glm::vec3( 0.0f,  1.0f, -1.0f)), // Side 0-4-1
        glm::normalize(glm::vec3( 0.0f,  1.0f,  1.0f))  // Side 2-4-3
    };

    vertices = {
        // Base face
        { v[0], colors[0], normals[0] }, { v[1], colors[0], normals[0] },
        { v[2], colors[0], normals[0] }, { v[3], colors[0], normals[0] },
        // Side 0-4-3
        { v[0], colors[1], normals[1] }, { v[4], colors[1], normals[1] },
        { v[3], colors[1], normals[1] },
        // Side 1-4-2
        { v[1], colors[2], normals[2] }, { v[4], colors[2], normals[2] },
        { v[2], colors[2], normals[2] },
        // Side 0-4-1
        { v[0], colors[3], normals[3] }, { v[4], colors[3], normals[3] },
        { v[1], colors[3], normals[3] },
        // Side 2-4-3
        { v[2], colors[4], normals[4] }, { v[4], colors[4], normals[4] },
        { v[3], colors[4], normals[4] }
    };

    if (fillMode == FillMode::Wireframe) {
        indices = {
            0, 1, 1, 2, 2, 3, 3, 0, // Base
            4, 6, 6, 5, 5, 4, // Side 0-4-3
            7, 9, 9, 8, 8, 7, // Side 1-4-2
            10, 12, 12, 11, 11, 10, // Side 0-4-1
            13, 15, 15, 14, 14, 13 // Side 2-4-3
        };
    } else {
        indices = {
            0, 1, 2, 0, 2, 3, // Base
            4, 5, 6, // Side 0-4-3
            7, 8, 9, // Side 1-4-2
            10, 11, 12, // Side 0-4-1
            13, 14, 15 // Side 2-4-3
        };
    }
}

void Pyramid::resetRotation() {
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}