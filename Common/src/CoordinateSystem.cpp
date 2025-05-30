#include "CoordinateSystem.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <limits>

CoordinateSystem::CoordinateSystem() {
    compileShaders();
    setup();
}

CoordinateSystem::~CoordinateSystem() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void CoordinateSystem::compileShaders() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        out vec3 ourColor;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            ourColor = aColor;
        }
    )";
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 ourColor;
        out vec4 FragColor;
        uniform int selectedAxis; // -1: none, 0: X, 1: Y, 2: Z
        void main() {
            vec3 color = ourColor;
            if (selectedAxis == 0 && ourColor.r > 0.5) color *= 1.5; // Highlight X
            if (selectedAxis == 1 && ourColor.g > 0.5) color *= 1.5; // Highlight Y
            if (selectedAxis == 2 && ourColor.b > 0.5) color *= 1.5; // Highlight Z
            FragColor = vec4(color, 1.0);
        }
    )";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Oxyz shader link error: " << infoLog << "\n";
    }
}

void CoordinateSystem::setup() {
    vertices.clear();
    float axisLength = 2.0f;
    float arrowLength = 0.01f;
    float arrowRadius = 0.1f;
    int segments = 4;
    glm::vec3 offset(0.0f, 0.0f, 0.0f);

    // X-axis (red): Ox
    vertices.push_back({ offset, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) });
    vertices.push_back({ offset + glm::vec3(axisLength - arrowLength, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) });
    for (int i = 0; i <= segments; ++i) {
        float theta = i * 2.0f * glm::pi<float>() / segments;
        glm::vec3 basePos(axisLength - arrowLength, arrowRadius * cos(theta), arrowRadius * sin(theta));
        glm::vec3 tipPos(axisLength, 0.0f, 0.0f);
        vertices.push_back({ offset + basePos, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) });
        vertices.push_back({ offset + tipPos, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f) });
    }

    // Y-axis (green): Oy
    vertices.push_back({ offset, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f) });
    vertices.push_back({ offset + glm::vec3(0.0f, axisLength - arrowLength, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f) });
    for (int i = 0; i <= segments; ++i) {
        float theta = i * 2.0f * glm::pi<float>() / segments;
        glm::vec3 basePos(arrowRadius * cos(theta), axisLength - arrowLength, arrowRadius * sin(theta));
        glm::vec3 tipPos(0.0f, axisLength, 0.0f);
        vertices.push_back({ offset + basePos, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f) });
        vertices.push_back({ offset + tipPos, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f) });
    }

    // Z-axis (blue): Oz
    vertices.push_back({ offset, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f) });
    vertices.push_back({ offset + glm::vec3(0.0f, 0.0f, axisLength - arrowLength), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f) });
    for (int i = 0; i <= segments; ++i) {
        float theta = i * 2.0f * glm::pi<float>() / segments;
        glm::vec3 basePos(arrowRadius * cos(theta), arrowRadius * sin(theta), axisLength - arrowLength);
        glm::vec3 tipPos(0.0f, 0.0f, axisLength);
        vertices.push_back({ offset + basePos, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f) });
        vertices.push_back({ offset + tipPos, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f) });
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::cout << "Oxyz setup with " << vertices.size() << " vertices\n";
}

void CoordinateSystem::render(int display_w, int display_h, const glm::mat4& view, const glm::mat4& projection) {
    // std::cout << "Rendering Oxyz coordinate system\n";
    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4_cast(rotation);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1i(glGetUniformLocation(shaderProgram, "selectedAxis"), selectedAxis);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, vertices.size());
    glBindVertexArray(0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "Oxyz OpenGL error: " << err << "\n";
    }
}

bool intersectRaySegment(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& segStart, const glm::vec3& segEnd, float& t, float threshold = 0.05f) {
    glm::vec3 segDir = segEnd - segStart;
    glm::vec3 rayToStart = segStart - rayOrigin;

    float a = glm::dot(rayDir, rayDir);
    float b = glm::dot(rayDir, segDir);
    float c = glm::dot(segDir, segDir);
    float d = glm::dot(rayDir, rayToStart);
    float e = glm::dot(segDir, rayToStart);

    float denom = a * c - b * b;
    if (std::abs(denom) < 1e-6f) return false;

    float tRay = (b * e - c * d) / denom;
    float tSeg = (a * e - b * d) / denom;

    if (tRay < 0.0f || tSeg < 0.0f || tSeg > 1.0f) return false;

    glm::vec3 pointOnRay = rayOrigin + tRay * rayDir;
    glm::vec3 pointOnSeg = segStart + tSeg * segDir;
    float distance = glm::length(pointOnRay - pointOnSeg);

    if (distance < threshold) {
        t = tRay;
        return true;
    }
    return false;
}

bool CoordinateSystem::selectAxis(const glm::vec3& rayOrigin, const glm::vec3& rayDir, float& t) {
    float closestT = std::numeric_limits<float>::max();
    int closestAxis = -1;

    glm::vec3 axes[] = {
        glm::vec3(1.0f, 0.0f, 0.0f), // X
        glm::vec3(0.0f, 1.0f, 0.0f), // Y
        glm::vec3(0.0f, 0.0f, 1.0f)  // Z
    };
    glm::vec3 origin(0.0f, 0.0f, -1.0f);
    float axisLength = 2.0f;

    for (int i = 0; i < 3; ++i) {
        glm::vec3 axisEnd = origin + axes[i] * axisLength;
        glm::vec3 rotatedStart = glm::mat3(rotation) * origin + origin;
        glm::vec3 rotatedEnd = glm::mat3(rotation) * (axes[i] * axisLength) + origin;
        float tHit;
        if (intersectRaySegment(rayOrigin, rayDir, rotatedStart, rotatedEnd, tHit)) {
            if (tHit < closestT) {
                closestT = tHit;
                closestAxis = i;
            }
        }
    }

    if (closestAxis != -1) {
        t = closestT;
        selectedAxis = closestAxis;
        return true;
    }
    selectedAxis = -1;
    return false;
}