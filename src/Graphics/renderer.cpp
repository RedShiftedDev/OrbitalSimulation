#include "renderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "GUI/gui.h"
#include "Graphics/core/RenderVisitor.h"

#include "GUI/Scene.h"

Renderer::Renderer(Window &window) : window(window) {
  init();

  // Add default light with better initial position and properties
  auto defaultLight = std::make_shared<Light>("Main Light");
  defaultLight->setPosition(glm::vec3(5.0f, 5.0f, 5.0f));
  defaultLight->setColor(glm::vec3(1.0f));
  defaultLight->setIntensity(1.0f);
  defaultLight->setAmbientStrength(0.1f);
  defaultLight->setDiffuseStrength(0.8f);
  defaultLight->setSpecularStrength(0.5f);
  lights.push_back(defaultLight);
}

Renderer::~Renderer() {
  if (gridVAO != 0) {
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
  }
}

void Renderer::init() {
  // Create shaders
  const auto sphereShader = std::make_shared<Shader>(
      "/Users/redshifted/code/OrbitalSimulation/src/Graphics/shaders/sphere.vert",
      "/Users/redshifted/code/OrbitalSimulation/src/Graphics/shaders/sphere.frag");
  const auto cubeSphereShader = std::make_shared<Shader>(
      "/Users/redshifted/code/OrbitalSimulation/src/Graphics/shaders/cubeSphere.vert",
      "/Users/redshifted/code/OrbitalSimulation/src/Graphics/shaders/cubeSphere.frag");

  // Create RenderVisitor and register shaders
  updateProjection();
  renderVisitor = std::make_unique<RenderVisitor>(viewMatrix, projectionMatrix);
  registerShader("Sphere", sphereShader);
  registerShader("CubeSphere", cubeSphereShader);

  // Initialize grid
  gridShader = std::make_unique<Shader>(
      "/Users/redshifted/code/OrbitalSimulation/src/Graphics/shaders/grid.vert",
      "/Users/redshifted/code/OrbitalSimulation/src/Graphics/shaders/grid.frag");
  initGrid();
}

void Renderer::initGrid() {
  // Delete existing grid buffers if they exist
  if (gridVAO != 0) {
    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
  }

  std::vector<float> vertices;
  float halfSize = settings.gridSize * 0.5f;
  float step = settings.gridSize / static_cast<float>(settings.gridDivisions);

  // Create grid lines along X axis
  for (int i = 0; i <= settings.gridDivisions; ++i) {
    float x = -halfSize + (i * step);
    vertices.push_back(x);
    vertices.push_back(0.0F);
    vertices.push_back(-halfSize);

    vertices.push_back(x);
    vertices.push_back(0.0F);
    vertices.push_back(halfSize);
  }

  // Create grid lines along Z axis
  for (int i = 0; i <= settings.gridDivisions; ++i) {
    float z = -halfSize + i * step;
    vertices.push_back(-halfSize);
    vertices.push_back(0.0f);
    vertices.push_back(z);

    vertices.push_back(halfSize);
    vertices.push_back(0.0f);
    vertices.push_back(z);
  }

  glGenVertexArrays(1, &gridVAO);
  glGenBuffers(1, &gridVBO);

  glBindVertexArray(gridVAO);
  glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Renderer::updateProjection() {
  float aspectRatio = static_cast<float>(window.getWidth()) / window.getHeight();
  projectionMatrix =
      glm::perspective(glm::radians(settings.fieldOfView), aspectRatio, 0.1f, 100.0f);
  viewMatrix =
      glm::lookAt(settings.cameraPosition, settings.cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

  if (renderVisitor) {
    renderVisitor->updateMatrices(viewMatrix, projectionMatrix);
  }
}

void Renderer::registerShader(const std::string &objectType,
                              const std::shared_ptr<Shader> &shader) const {
  if (renderVisitor) {
    renderVisitor->registerShader(objectType, shader);
  }
}

void Renderer::renderGrid() const {
  if (!settings.showGrid) {
    return;
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  gridShader->use();
  gridShader->setMat4("projection", projectionMatrix);
  gridShader->setMat4("view", viewMatrix);
  gridShader->setMat4("model", glm::mat4(1.0f));
  gridShader->setVec3("gridColor", settings.gridColor);
  gridShader->setFloat("fadeDistance", settings.gridFadeDistance);
  gridShader->setFloat("gridSize", settings.gridSize);
  gridShader->setInt("gridDivisions", settings.gridDivisions);

  glBindVertexArray(gridVAO);
  glDrawArrays(GL_LINES, 0, (settings.gridDivisions + 1) * 4);
  glBindVertexArray(0);

  glDisable(GL_BLEND);
}

void Renderer::render(const gui::Scene &scene) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  updateProjection();
  renderGrid();

  // Pass the lights to the RenderVisitor before rendering
  renderVisitor->setLights(lights);

  // Render all objects in the scene
  renderScene(scene);
}

void Renderer::renderScene(const gui::Scene &scene) const {
  // Render each object in the scene using the visitor pattern
  for (const auto &obj : scene.getObjects()) {
    if (obj.object) {                     // Check if the Object3D pointer is valid
      obj.object->accept(*renderVisitor); // Access the Object3D through the object member
    }
  }
}

void Renderer::setWireframe(const bool enable) {
  wireframeMode = enable;
  glPolygonMode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
}
