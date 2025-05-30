#include "Renderer.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer(GLFWwindow* win) : window(win) {
    compileShaders();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void Renderer::compileShaders() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        layout (location = 2) in vec3 aNormal;
        out vec3 ourColor;
        out vec3 Normal;
        out vec3 FragPos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            ourColor = aColor;
            Normal = mat3(transpose(inverse(model))) * aNormal;
            FragPos = vec3(model * vec4(aPos, 1.0));
        }
    )";
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 ourColor;
        in vec3 Normal;
        in vec3 FragPos;
        out vec4 FragColor;
        uniform vec3 lightPos = vec3(1.0, 1.0, 2.0);
        uniform vec3 viewPos;
        uniform int fillMode; // 0 = Solid, 1 = Wireframe, 2 = Dotted
        uniform bool useRainbow;
        uniform float time;
        void main() {
            if (fillMode == 2) {
                ivec2 stipple = ivec2(gl_FragCoord.xy) % 8;
                if (stipple.x >= 2 || stipple.y >= 2) discard;
            }
            vec3 color = ourColor;
            if (useRainbow) {
                float t = time + gl_FragCoord.x * 0.02;
                float band = floor(mod(t * 0.5, 6.0));
                if (band < 1.0) color = vec3(1.0, 0.0, 0.0);
                else if (band < 2.0) color = vec3(1.0, 0.5, 0.0);
                else if (band < 3.0) color = vec3(1.0, 1.0, 0.0);
                else if (band < 4.0) color = vec3(0.0, 1.0, 0.0);
                else if (band < 5.0) color = vec3(0.0, 0.0, 1.0);
                else color = vec3(1.0, 0.0, 1.0);
            }
            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * color;
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * color;
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * vec3(1.0);
            vec3 result = (ambient + diffuse + specular);
            FragColor = vec4(result, 1.0);
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
}

void Renderer::setupBuffers(const Shape& shape) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, shape.getVertices().size() * sizeof(Vertex), shape.getVertices().data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.getIndices().size() * sizeof(unsigned int), shape.getIndices().data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::render(const Shape& shape) {
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    switch (backgroundMode) {
        case BackgroundMode::White:
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            break;
        case BackgroundMode::Dark:
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            break;
        case BackgroundMode::Gray:
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            break;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)display_w / (float)display_h, 0.1f, 100.0f);

    // Render Oxyz coordinate system as a background layer
    if (coordSystemMode) {
        glDepthFunc(GL_LEQUAL); // Ensure Oxyz is visible
        coordinateSystem.render(display_w, display_h, view, projection);
        glDepthFunc(GL_LESS);
    }

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 0.0f, 3.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) * glm::mat4_cast(getOxyzRotation()) * shape.getModelMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    int fillModeInt = static_cast<int>(shape.getFillMode());
    glUniform1i(glGetUniformLocation(shaderProgram, "fillMode"), fillModeInt);
    glUniform1i(glGetUniformLocation(shaderProgram, "useRainbow"), useRainbowEffect ? 1 : 0);
    glUniform1f(glGetUniformLocation(shaderProgram, "time"), currentTime);

    glBindVertexArray(VAO);
    if (shape.getFillMode() == FillMode::Wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_LINES, shape.getIndices().size(), GL_UNSIGNED_INT, 0);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, shape.getIndices().size(), GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);

    // Check for OpenGL errors
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << "\n";
    }
}