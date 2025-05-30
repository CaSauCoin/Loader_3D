#include "Globular.hpp"
#include <glm/gtc/constants.hpp>

void Globular::setupVertices() {
    vertices.clear();
    indices.clear();

    float radius = size * 0.5f;
    const int segments = 16;
    const int rings = 16;
    const float pi = glm::pi<float>();
    const float two_pi = 2.0f * pi;
    glm::vec3 center(1.0f, 1.0f, 1.0f); // Center at (1, 1, 1)

    for (int i = 0; i <= rings; ++i) {
        float theta = i * pi / rings;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int j = 0; j <= segments; ++j) {
            float phi = j * two_pi / segments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            Vertex v;
            v.position = center + glm::vec3(
                radius * sinTheta * cosPhi,
                radius * sinTheta * sinPhi,
                radius * cosTheta
            );
            v.normal = glm::normalize(v.position - center);
            v.color = color;

            vertices.push_back(v);
        }
    }

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
    } else {
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