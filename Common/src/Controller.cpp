#include "Controller.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <iostream>

void Controller::handleMouseInput() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    glm::vec2 currentMousePos(x, y);

    // Debug key states
    bool leftCtrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    bool rightCtrl = glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
    bool ctrlPressed = leftCtrl || rightCtrl;
    // debug ctrl pressed
    // if (ctrlPressed) {
        // std::cout << "Ctrl pressed: Left=" << leftCtrl << ", Right=" << rightCtrl << "\n";
    // }
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::GetIO().WantCaptureMouse) {
        if (coordSystemMode && ctrlPressed) {
            // std::cout << "Starting Oxyz rotation\n";
            rotatingOxyz = true;
            lastMousePos = currentMousePos;
            // std::cout << "CTRL Pressed " << ctrlPressed << std::endl;
        } else if (coordSystemMode) {
            // Axis selection
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            float normX = (2.0f * x) / display_w - 1.0f;
            float normY = 1.0f - (2.0f * y) / display_h;

            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)display_w / display_h, 0.1f, 100.0f);
            glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 invVP = glm::inverse(projection * view);
            glm::vec4 nearPoint(normX, normY, -1.0f, 1.0f);
            glm::vec4 farPoint(normX, normY, 1.0f, 1.0f);
            glm::vec4 nearWorld = invVP * nearPoint;
            glm::vec4 farWorld = invVP * farPoint;
            nearWorld /= nearWorld.w;
            farWorld /= farWorld.w;

            glm::vec3 rayOrigin = glm::vec3(nearWorld);
            glm::vec3 rayDir = glm::normalize(glm::vec3(farWorld - nearWorld));

            float t;
            if (renderer.selectAxis(rayOrigin, rayDir, t)) {
                setSelectedAxis(renderer.getSelectedAxis());
                // std::cout << "Selected axis: " << selectedAxis << "\n";
                return;
            } else {
                setSelectedAxis(-1);
            }
        }

        rotating = true;
        lastMousePos = currentMousePos;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        rotating = false;
    }
    if (rotatingOxyz && coordSystemMode && ctrlPressed) {
        glm::vec2 delta = currentMousePos - lastMousePos;
        lastMousePos = currentMousePos;
        float sensitivity = 0.005f;
        glm::quat rotX = glm::angleAxis(-delta.y * sensitivity, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::quat rotY = glm::angleAxis(-delta.x * sensitivity, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat rot = rotY * rotX;
        renderer.applyOxyzRotation(rot);
        shape.applyRotation(rot);
        // std::cout << "Rotating Oxyz and block, delta: (" << delta.x << ", " << delta.y << ")\n";
    } else if (rotating) {
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
    if (!coordSystemMode){
        if (ImGui::Combo("Fill Mode", &currentFill, fillItems, 3)) {
            prevFillMode = static_cast<FillMode>(currentFill);
            shape.setFillMode(prevFillMode);
        }
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
        // std::cout << "Button pressed: Switching to " << ((shapeType == 0) ? "Pyramid" : (shapeType == 1) ? "Globular" : "Cube") << "\n";
        toggleShape = true;
        shapeType = (shapeType + 1) % 3; // Cycle: Cube -> Pyramid -> Globular
    }
    ImGui::Separator();
    ImGui::Text("Face Colors");
    if (!renderer.getRainbowEffect()) {
        int colorCount = (shapeType == 1) ? 4 : (shapeType == 2) ? 1 : 6;
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
    const char* coordLabel = coordSystemMode ? "Disable Coord System" : "Enable Coord System";
    if (ImGui::Button(coordLabel)) {
        coordSystemMode = !coordSystemMode;
        renderer.setCoordSystemMode(coordSystemMode);
        if (coordSystemMode) {
            prevFillMode = shape.getFillMode(); // Store current fill mode
            shape.setFillMode(FillMode::Wireframe); // Set to wireframe
        } else {
            shape.setFillMode(prevFillMode); // Restore previous fill mode
        }
    }
    ImGui::Separator();
    if (ImGui::Button("Reset Rotation")) {
        shape.resetRotation();
        renderer.resetOxyzRotation();
    }
    
    ImGui::End();
}