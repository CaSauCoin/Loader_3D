#include "Controller.hpp"
#include <glm/gtc/quaternion.hpp>

void Controller::handleMouseInput() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    glm::vec2 currentMousePos(x, y);

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        if (!ImGui::GetIO().WantCaptureMouse) {
            rotating = true;
            lastMousePos = currentMousePos;
        }
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        rotating = false;
    }
    if (rotating) {
        glm::vec2 delta = currentMousePos - lastMousePos;
        lastMousePos = currentMousePos;
        float sensitivity = 0.005f;
        glm::quat rotX = glm::angleAxis(-delta.y * sensitivity, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat rotY = glm::angleAxis(-delta.x * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
        cube.applyRotation(rotY * rotX);
    }
}

void Controller::renderImGui() {
    ImGui::Begin("Cube Controls");
    ImGui::SliderFloat("Size", &cube.getSize(), 0.1f, 2.0f);
    bool filled = cube.isFilled();
    ImGui::Checkbox("Filled", &filled);
    if (filled != cube.isFilled()) {
        cube.setFilled(filled); // Use setter
    }
    ImGui::Separator();
    ImGui::Text("Face Colors");
    ImGui::ColorEdit3("Front", &cube.getColors()[0].x);
    ImGui::ColorEdit3("Back", &cube.getColors()[1].x);
    ImGui::ColorEdit3("Right", &cube.getColors()[2].x);
    ImGui::ColorEdit3("Left", &cube.getColors()[3].x);
    ImGui::ColorEdit3("Top", &cube.getColors()[4].x);
    ImGui::ColorEdit3("Bottom", &cube.getColors()[5].x);
    ImGui::Separator();
    if (ImGui::Button("Reset Rotation")) {
        cube.resetRotation();
    }
    ImGui::End();
}