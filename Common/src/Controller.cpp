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

    if (autoRotate) {
        float angle = static_cast<float>(glfwGetTime()) * rotateSpeed;
        glm::quat rotY = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
        shape.applyRotation(rotY);
    }
}

void Controller::renderImGui() {
    ImGui::Begin("Shape Controls");

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    ImVec2 pos = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();

    float maxWidth = static_cast<float>(display_w);
    float maxHeight = static_cast<float>(display_h);
    if (size.x > maxWidth) size.x = maxWidth;
    if (size.y > maxHeight) size.y = maxHeight;
    ImGui::SetWindowSize(ImVec2(size.x, size.y));

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ImVec2 newPos = pos;
        if (pos.x < snapThreshold && pos.x >= 0) {
            newPos.x = 0;
        }
        else if (pos.x + size.x > display_w - snapThreshold && pos.x + size.x <= display_w) {
            newPos.x = display_w - size.x;
        }
        if (pos.y < snapThreshold && pos.y >= 0) {
            newPos.y = 0;
        }
        else if (pos.y + size.y > display_h - snapThreshold && pos.y + size.y <= display_h) {
            newPos.y = display_h - size.y;
        }
        newPos.x = glm::clamp(newPos.x, 0.0f, maxWidth - size.x);
        newPos.y = glm::clamp(newPos.y, 0.0f, maxHeight - size.y);
        
        ImGui::SetWindowPos(newPos);
    }

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
    const char* shapeLabel = (shapeType == 0) ? "Switch to Pyramid" : (shapeType == 1) ? "Switch to Globular" : "Switch to Cube";
    if (ImGui::Button(shapeLabel)) {
        std::cout << "Button pressed: Switching to " << ((shapeType == 0) ? "Pyramid" : (shapeType == 1) ? "Globular" : "Cube") << "\n";
        toggleShape = true;
        shapeType = (shapeType + 1) % 3; // Cycle: Cube -> Pyramid -> Globular
    }
    ImGui::Separator();
    ImGui::Text("Face Colors");
    if (!renderer.getRainbowEffect()) {
        int colorCount = (shapeType == 1) ? 4 : 6;
        if (shapeType == 0) { // Cube
            ImGui::ColorEdit3("Front", &shape.getColors()[0].x);
            ImGui::ColorEdit3("Back", &shape.getColors()[1].x);
            ImGui::ColorEdit3("Right", &shape.getColors()[2].x);
            ImGui::ColorEdit3("Left", &shape.getColors()[3].x);
            ImGui::ColorEdit3("Top", &shape.getColors()[4].x);
            ImGui::ColorEdit3("Bottom", &shape.getColors()[5].x);
        } else if (shapeType == 1) { // Pyramid
            ImGui::ColorEdit3("Base Face 1", &shape.getColors()[0].x);
            ImGui::ColorEdit3("Base Face 2", &shape.getColors()[1].x);
            ImGui::ColorEdit3("Side Face 1", &shape.getColors()[2].x);
            ImGui::ColorEdit3("Side Face 2", &shape.getColors()[3].x);
        } else { // Globular
            ImGui::ColorEdit3("Sphere Color", &shape.getColors()[0].x);
        }
    }

    ImGui::Separator();
    const char* rotateLabel = autoRotate ? "Stop Auto Rotate Y" : "Start Auto Rotate Y";
    if (ImGui::Button(rotateLabel)) {
        setAutoRotate(!autoRotate);
    }

    ImGui::Separator();
    ImGui::SliderFloat("Rotation Speed", &rotateSpeed, 0.00f, 0.015f, "%.4f rad/s");

    ImGui::Separator();
    if (ImGui::Button("Reset Rotation")) {
        shape.resetRotation();
    }

    ImGui::End();
}