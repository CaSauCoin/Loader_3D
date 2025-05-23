#include "Cube.hpp"

void Cube::setupVertices() {
    float s = size * 0.5f;
    glm::vec3 positions[] = {
        {-s, -s, s}, {s, -s, s}, {s, s, s}, {-s, s, s},
        {-s, -s, -s}, {s, -s, -s}, {s, s, -s}, {-s, s, -s}
    };
    int faces[6][4] = {
        {0, 1, 2, 3}, {5, 4, 7, 6}, {1, 5, 6, 2},
        {4, 0, 3, 7}, {3, 2, 6, 7}, {4, 5, 1, 0}
    };
    glm::vec3 normals[6] = {
        {0, 0, 1}, {0, 0, -1}, {1, 0, 0},
        {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}
    };

    vertices.clear();
    indices.clear();

    if (fillMode == FillMode::Wireframe) {
        for (int face = 0; face < 6; ++face) {
            int baseIndex = vertices.size();
            for (int i = 0; i < 4; ++i) {
                Vertex v;
                v.position = positions[faces[face][i]];
                v.color = colors[face];
                v.normal = normals[face];
                vertices.push_back(v);
            }
            for (int i = 0; i < 4; ++i) {
                indices.push_back(baseIndex + i);
                indices.push_back(baseIndex + (i + 1) % 4);
            }
        }
    } else { // Solid or Dotted (both use triangles)
        for (int face = 0; face < 6; ++face) {
            int baseIndex = vertices.size();
            for (int i = 0; i < 4; ++i) {
                Vertex v;
                v.position = positions[faces[face][i]];
                v.color = colors[face];
                v.normal = normals[face];
                vertices.push_back(v);
            }
            indices.push_back(baseIndex);
            indices.push_back(baseIndex + 1);
            indices.push_back(baseIndex + 2);
            indices.push_back(baseIndex);
            indices.push_back(baseIndex + 2);
            indices.push_back(baseIndex + 3);
        }
    }
}

void Cube::resetRotation() {
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}