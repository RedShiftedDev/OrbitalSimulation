#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer() { init(); }

void Renderer::init() {
  sphereShader = std::make_unique<Shader>(
      "/Users/redshifted/code/C++/OrbitalSimulation/src/Graphics/shaders/sphere.vert",
      "/Users/redshifted/code/C++/OrbitalSimulation/src/Graphics/shaders/sphere.frag");
  updateProjection();
}

void Renderer::updateProjection() {
  projectionMatrix =
      glm::perspective(glm::radians(settings.fieldOfView), aspectRatio, 0.1f, 100.0f);
  viewMatrix =
      glm::lookAt(settings.cameraPosition, settings.cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Renderer::render(const Sphere &sphere) {
  sphereShader->use();

  // Update matrices
  sphereShader->setMat4("projection", projectionMatrix);
  sphereShader->setMat4("view", viewMatrix);
  sphereShader->setMat4("model", sphere.getModelMatrix());

  // Set lighting parameters
  sphereShader->setVec3("lightPos", settings.lightPosition);
  sphereShader->setVec3("lightColor", settings.lightColor);
  sphereShader->setVec3("viewPos", settings.cameraPosition);
  sphereShader->setFloat("ambientStrength", settings.ambientStrength);
  sphereShader->setFloat("diffuseStrength", settings.diffuseStrength);
  sphereShader->setFloat("specularStrength", settings.specularStrength);
  sphereShader->setFloat("shininess", settings.shininess);

  // Set object color
  sphereShader->setVec3("objectColor", sphere.getColor());

  sphere.draw();
}
