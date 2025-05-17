#pragma once
#include "shader.h"
#include "sphere.h"
#include <glm/glm.hpp>
#include <memory>

struct SceneSettings {
    glm::vec3 cameraPosition{0.0f, 0.0f, 5.0f};
    glm::vec3 cameraTarget{0.0f, 0.0f, 0.0f};
    float fieldOfView = 45.0f;

    // Light properties
    glm::vec3 lightPosition{5.0f, 5.0f, 5.0f};
    glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
    float ambientStrength = 0.1f;
    float diffuseStrength = 0.7f;
    float specularStrength = 0.5f;
    float shininess = 32.0f;

    // Background
    glm::vec3 backgroundColor{0.1f, 0.1f, 0.15f};
};

class Renderer {
public:
    Renderer();
    void init();
    void render(const Sphere& sphere);

    void updateProjection();
    SceneSettings& getSettings() { return settings; }
    const SceneSettings& getSettings() const { return settings; }

private:
    std::unique_ptr<Shader> sphereShader;
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projectionMatrix{1.0f};
    SceneSettings settings;
    float aspectRatio = 1280.0f / 720.0f;
};
