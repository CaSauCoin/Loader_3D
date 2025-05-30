#include "Cube.hpp"
#include <glm/glm.hpp>

void Cube::setupVertices() {
    vertices.clear();
    indices.clear();

    float halfSize = size * 0.5f;
    glm::vec3 center(1.0f, 1.0f, 1.0f); // Center at (1, 1, 1)

    // Define 8 vertices of the cube, centered at (1, 1, 1)
    glm::vec3 v[] = {
        center + glm::vec3(-halfSize, -halfSize, -halfSize), // 0
        center + glm::vec3( halfSize, -halfSize, -halfSize), // 1
        center + glm::vec3( halfSize,  halfSize, -halfSize), // 2
        center + glm::vec3(-halfSize,  halfSize, -halfSize), // 3
        center + glm::vec3(-halfSize, -halfSize,  halfSize), // 4
        center + glm::vec3( halfSize, -halfSize,  halfSize), // 5
        center + glm::vec3( halfSize,  halfSize,  halfSize), // 6
        center + glm::vec3(-halfSize,  halfSize,  halfSize)  // 7
    };

    // Define normals
    glm::vec3 normals[] = {
        glm::vec3( 0.0f,  0.0f, -1.0f), // Front
        glm::vec3( 0.0f,  0.0f,  1.0f), // Back
        glm::vec3( 1.0f,  0.0f,  0.0f), // Right
        glm::vec3(-1.0f,  0.0f,  0.0f), // Left
        glm::vec3( 0.0f,  1.0f,  0.0f), // Top
        glm::vec3( 0.0f, -1.0f,  0.0f)  // Bottom
    };

    // Define faces with vertices and colors
    vertices = {
        // Front face
        { v[0], colors[0], normals[0] }, { v[1], colors[0], normals[0] },
        { v[2], colors[0], normals[0] }, { v[3], colors[0], normals[0] },
        // Back face
        { v[5], colors[1], normals[1] }, { v[4], colors[1], normals[1] },
        { v[7], colors[1], normals[1] }, { v[6], colors[1], normals[1] },
        // Right face
        { v[1], colors[2], normals[2] }, { v[5], colors[2], normals[2] },
        { v[6], colors[2], normals[2] }, { v[2], colors[2], normals[2] },
        // Left face
        { v[4], colors[3], normals[3] }, { v[0], colors[3], normals[3] },
        { v[3], colors[3], normals[3] }, { v[7], colors[3], normals[3] },
        // Top face
        { v[3], colors[4], normals[4] }, { v[2], colors[4], normals[4] },
        { v[6], colors[4], normals[4] }, { v[7], colors[4], normals[4] },
        // Bottom face
        { v[0], colors[5], normals[5] }, { v[4], colors[5], normals[5] },
        { v[5], colors[5], normals[5] }, { v[1], colors[5], normals[5] }
    };

    if (fillMode == FillMode::Wireframe) {
        // Wireframe indices (lines)
        indices = {
            0, 1, 1, 2, 2, 3, 3, 0, // Front
            4, 5, 5, 6, 6, 7, 7, 4, // Back
            0, 4, 1, 5, 2, 6, 3, 7  // Connecting edges
        };
    } else {
        // Solid or Dotted indices (triangles)
        indices = {
            0, 1, 2, 0, 2, 3, // Front
            4, 5, 6, 4, 6, 7, // Back
            8, 9, 10, 8, 10, 11, // Right
            12, 13, 14, 12, 14, 15, // Left
            16, 17, 18, 16, 18, 19, // Top
            20, 21, 22, 20, 22, 23 // Bottom
        };
    }
}

void Cube::resetRotation() {
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}