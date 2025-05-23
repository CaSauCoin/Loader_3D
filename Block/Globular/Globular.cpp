#include "Globular.hpp"
#include <glm/gtc/constants.hpp>

void Globular::setupVertices() {
    vertices.clear();
    indices.clear();

    float radius = size * 0.5f;
    const int segments = 64; // Number of segments for sphere approximation
    const int rings = 64;    // Number of rings for sphere approximation
    const float pi = glm::pi<float>();
    const float two_pi = 2.0f * pi;

    // Generate vertices
    for (int i = 0; i <= rings; ++i) {
        float theta = i * pi / rings;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int j = 0; j <= segments; ++j) {
            float phi = j * two_pi / segments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            Vertex v;
            v.position = glm::vec3(
                radius * sinTheta * cosPhi,
                radius * sinTheta * sinPhi,
                radius * cosTheta
            );
            v.normal = glm::normalize(v.position);
            v.color = color;
            vertices.push_back(v);
        }
    }

    // Generate indices
    if (fillMode == FillMode::Wireframe) {
        for (int i = 0; i < rings; ++i) {
            for (int j = 0; j < segments; ++j) {
                int base = i * (segments + 1) + j;
                indices.push_back(base);
                indices.push_back(base + 1);
                indices.push_back(base);
                indices.push_back(base + segments + 1);
            }
        }
    } else { // Solid or Dotted
        for (int i = 0; i < rings; ++i) {
            for (int j = 0; j < segments; ++j) {
                int base = i * (segments + 1) + j;
                indices.push_back(base);
                indices.push_back(base + 1);
                indices.push_back(base + segments + 2);
                indices.push_back(base);
                indices.push_back(base + segments + 2);
                indices.push_back(base + segments + 1);
            }
        }
    }
}

void Globular::resetRotation() {
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}