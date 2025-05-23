#include "Pyramid.hpp"

void Pyramid::setupVertices() {
    float s = size * 0.5f;
    glm::vec3 positions[] = {
        {0.0f, s, 0.0f}, {-s, -s, s}, {s, -s, s}, {0.0f, -s, -s}
    };
    int faces[4][3] = {
        {1, 2, 3}, {0, 1, 2}, {0, 2, 3}, {0, 3, 1}
    };
    glm::vec3 normals[4] = {
        {0.0f, -1.0f, 0.0f},
        {0.0f, 0.4472f, 0.8944f},
        {0.7071f, 0.4472f, -0.5477f},
        {-0.7071f, 0.4472f, -0.5477f}
    };

    vertices.clear();
    indices.clear();

    if (fillMode == FillMode::Wireframe) {
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
    } else { // Solid or Dotted
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
    }
}

void Pyramid::resetRotation() {
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}