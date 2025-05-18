#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Graphics/core/Object3D.h"
#include "Graphics/lighting/Light.h"  // Add Light header include
#include "core/fps_counter.h"

// Forward declarations
namespace gui {
    class Scene;
}
class Sphere;
class CubeSphere;
class Renderer;
class Light;

namespace gui {

struct SceneObject {
    std::string name;
    std::shared_ptr<Object3D> object;
    bool selected = false;
};

// Update function signature to use shared_ptr
void RenderGui(const FpsCounter& fpsCounter, std::shared_ptr<Sphere>& sphere, 
               std::shared_ptr<CubeSphere>& cubeSphere, Renderer& renderer);
Scene& getScene();

// Update function declarations to accept const references
void MainMenuBar(const FpsCounter& fpsCounter);
void PerformanceWindow(const FpsCounter& fpsCounter);

// Function to render lighting controls
void RenderLightingControls(std::vector<std::shared_ptr<Light>>& lights);

} // namespace gui
