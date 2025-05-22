#include "Pyramid.hpp"

void Pyramid::setupVertices() {
    float s = size * 0.5f;
    glm::vec3 positions[] = {
        {0.0f, s, 0.0f},      // Apex (0)
        {-s, -s, s},          // Base vertex 1 (1)
        {s, -s, s},           // Base vertex 2 (2)
        {0.0f, -s, -s}        // Base vertex 3 (3)
    };
    int faces[4][3] = {
        {1, 2, 3}, // Base
        {0, 1, 2}, // Side 1
        {0, 2, 3}, // Side 2
        {0, 3, 1}  // Side 3
    };
    glm::vec3 normals[4] = {
        {0.0f, -1.0f, 0.0f},  // Base
        {0.0f, 0.4472f, 0.8944f}, // Side 1
        {0.7071f, 0.4472f, -0.5477f}, // Side 2
        {-0.7071f, 0.4472f, -0.5477f} // Side 3
    };

    vertices.clear();
    indices.clear();

    if (filled) {
        for (int face = 0; face < 4; ++face) {
            int baseIndex = vertices.size();
            for (int i = 0; i < 3; ++i) {
                Vertex v;
                v.position = positions[faces[face][i]];
                v.color = colors[face];
                v.normal = normals[face];
                vertices.push_back(v);
            }
            indices.push_back(baseIndex);
            indices.push_back(baseIndex + 1);
            indices.push_back(baseIndex + 2);
        }
    } else {
        for (int face = 0; face < 4; ++face) {
            int baseIndex = vertices.size();
            for (int i = 0; i < 3; ++i) {
                Vertex v;
                v.position = positions[faces[face][i]];
                v.color = colors[face];
                v.normal = normals[face];
                vertices.push_back(v);
            }
            for (int i = 0; i < 3; ++i) {
                indices.push_back(baseIndex + i);
                indices.push_back(baseIndex + (i + 1) % 3);
            }
        }
    }
}

void Pyramid::resetRotation() {
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}