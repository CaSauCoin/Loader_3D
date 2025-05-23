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
        shape.applyRotation(rotY * rotX);
    }
}

void Controller::renderImGui() {
    ImGui::Begin("Shape Controls");
    ImGui::SliderFloat("Size", &shape.getSize(), 0.1f, 2.0f);
    const char* fillItems[] = { "Solid", "Wireframe", "Dotted" };
    int currentFill = static_cast<int>(shape.getFillMode());
    if (ImGui::Combo("Fill Mode", &currentFill, fillItems, 3)) {
        shape.setFillMode(static_cast<FillMode>(currentFill));
    }
    ImGui::Separator();
    bool rainbow = renderer.getRainbowEffect();
    if (ImGui::Checkbox("Rainbow Effect", &rainbow)) {
        renderer.setRainbowEffect(rainbow);
    }
    ImGui::Separator();
    const char* bgItems[] = { "White", "Dark", "Gray" };
    int currentBg = static_cast<int>(renderer.getBackgroundMode());
    if (ImGui::Combo("Background", &currentBg, bgItems, 3)) {
        renderer.setBackgroundMode(static_cast<BackgroundMode>(currentBg));
    }
    ImGui::Separator();
    if (ImGui::Button(isCube ? "Switch to Pyramid" : "Switch to Cube")) {
        std::cout << "Button pressed: Switching to " << (isCube ? "Pyramid" : "Cube") << "\n";
        toggleShape = true;
        isCube = !isCube;
    }
    ImGui::Separator();
    ImGui::Text("Face Colors");
    if (!renderer.getRainbowEffect()) {
        if (isCube) {
            ImGui::ColorEdit3("Front", &shape.getColors()[0].x);
            ImGui::ColorEdit3("Back", &shape.getColors()[1].x);
            ImGui::ColorEdit3("Right", &shape.getColors()[2].x);
            ImGui::ColorEdit3("Left", &shape.getColors()[3].x);
            ImGui::ColorEdit3("Top", &shape.getColors()[4].x);
            ImGui::ColorEdit3("Bottom", &shape.getColors()[5].x);
        } else {
            ImGui::ColorEdit3("Base Face 1", &shape.getColors()[0].x);
            ImGui::ColorEdit3("Base Face 2", &shape.getColors()[1].x);
            ImGui::ColorEdit3("Side Face 1", &shape.getColors()[2].x);
            ImGui::ColorEdit3("Side Face 2", &shape.getColors()[3].x);
        }
    }
    ImGui::Separator();
    if (ImGui::Button("Reset Rotation")) {
        shape.resetRotation();
    }
    ImGui::End();
}