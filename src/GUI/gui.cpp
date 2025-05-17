#include "GUI/gui.h"
#include <imgui.h>
#include "core/fps_counter.h"
#include "Graphics/sphere.h"
#include "Graphics/renderer.h"

namespace gui {

void RenderGui(FpsCounter &fpsCounter, Sphere &sphere, Renderer &renderer) {
    ImGui::Begin("Performance");
    ImGui::Text("FPS: %.1f", fpsCounter.getFps());
    ImGui::End();

    ImGui::Begin("Scene Controls");
    bool sceneUpdated = false;
    auto& settings = renderer.getSettings();

    // Camera Controls
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        sceneUpdated |= ImGui::DragFloat3("Position", &settings.cameraPosition[0], 0.1f);
        sceneUpdated |= ImGui::DragFloat3("Target", &settings.cameraTarget[0], 0.1f);
        sceneUpdated |= ImGui::SliderFloat("Field of View", &settings.fieldOfView, 10.0f, 120.0f);
    }

    // Lighting Controls
    if (ImGui::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_DefaultOpen)) {
        sceneUpdated |= ImGui::DragFloat3("Light Position", &settings.lightPosition[0], 0.1f);
        sceneUpdated |= ImGui::ColorEdit3("Light Color", &settings.lightColor[0]);
        sceneUpdated |= ImGui::SliderFloat("Ambient Strength", &settings.ambientStrength, 0.0f, 1.0f);
        sceneUpdated |= ImGui::SliderFloat("Diffuse Strength", &settings.diffuseStrength, 0.0f, 1.0f);
        sceneUpdated |= ImGui::SliderFloat("Specular Strength", &settings.specularStrength, 0.0f, 1.0f);
        sceneUpdated |= ImGui::SliderFloat("Shininess", &settings.shininess, 1.0f, 256.0f);
    }

    // Background Color
    if (ImGui::CollapsingHeader("Background", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::ColorEdit3("Background Color", &settings.backgroundColor[0])) {
            glClearColor(settings.backgroundColor.r, settings.backgroundColor.g, settings.backgroundColor.b, 1.0f);
        }
    }

    if (sceneUpdated) {
        renderer.updateProjection();
    }
    ImGui::End();

    ImGui::Begin("Sphere Controls");
    // Position control
    glm::vec3 position = sphere.getPosition();
    if (ImGui::DragFloat3("Position", &position[0], 0.1f)) {
        sphere.setPosition(position);
    }

    // Scale control
    glm::vec3 scale = sphere.getScale();
    if (ImGui::DragFloat3("Scale", &scale[0], 0.1f, 0.1f, 10.0f)) {
        sphere.setScale(scale);
    }

    // Color control
    glm::vec3 color = sphere.getColor();
    if (ImGui::ColorEdit3("Color", &color[0])) {
        sphere.setColor(color);
    }
    ImGui::End();
}

} // namespace gui
