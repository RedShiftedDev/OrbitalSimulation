#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include "core/window.h"
#include "Graphics/shader.h"

// Forward declarations
namespace gui {
    class Scene;
}
class RenderVisitor;
class Light;

class Renderer {
public:
    struct Settings {
        glm::vec3 cameraPosition{0.0f, 3.0f, 5.0f};
        glm::vec3 cameraTarget{0.0f, 0.0f, 0.0f};
        float fieldOfView = 45.0f;
        glm::vec3 lightPosition{5.0f, 5.0f, 5.0f};
        glm::vec3 lightColor{1.0f};
        float ambientStrength = 0.1f;
        float diffuseStrength = 0.7f;
        float specularStrength = 0.5f;
        float shininess = 32.0f;
        glm::vec3 backgroundColor{0.1f, 0.1f, 0.1f};
        bool showGrid = true;
        float gridSize = 20.0f;
        int gridDivisions = 20;
        glm::vec3 gridColor{0.5f, 0.5f, 0.5f};
        float gridFadeDistance = 100.0f;
    };

    explicit Renderer(Window& window);
    ~Renderer();  // Add destructor declaration

    void init();
    void render(const gui::Scene& scene);  // Updated signature to match implementation
    void renderScene(const gui::Scene& scene) const;  // Fixed namespace
    void renderGrid() const;
    void initGrid();
    void updateProjection();

    void registerShader(const std::string& objectType, const std::shared_ptr<Shader> &shader) const;
    Settings& getSettings() { return settings; }

    bool isWireframe() const { return wireframeMode; }
    void setWireframe(bool enable);

    // Add light management methods
    std::vector<std::shared_ptr<Light>>& getLights() { return lights; }
    const std::vector<std::shared_ptr<Light>>& getLights() const { return lights; }
    void addLight(const std::shared_ptr<Light>& light) { lights.push_back(light); }

private:
    Window& window;
    Settings settings;
    bool wireframeMode = false;
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;

    // Grid mesh data
    unsigned int gridVAO = 0;
    unsigned int gridVBO = 0;
    std::shared_ptr<Shader> gridShader;

    // View and projection matrices
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projectionMatrix{1.0f};
    std::unique_ptr<RenderVisitor> renderVisitor;

    std::vector<std::shared_ptr<Light>> lights;  // Add lights vector
};
