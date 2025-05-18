#include "RenderVisitor.h"
#include "Object3D.h"
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/bodies/sphere.h"
#include "Graphics/bodies/cubeSphere.h"


void RenderVisitor::visit(Object3D* object) {
    if (!object) return;

    // Get the appropriate shader based on object type
    std::string shaderType = "Sphere";  // Default
    if (dynamic_cast<CubeSphere*>(object)) {
        shaderType = "CubeSphere";
    }

    auto shader = shaders[shaderType];
    if (!shader) return;

    // Set up transformation matrices
    glm::mat4 model(1.0f);
    model = glm::translate(model, object->getPosition());
    model = glm::scale(model, object->getScale());

    shader->use();
    shader->setMat4("projection", projectionMatrix);
    shader->setMat4("view", viewMatrix);
    shader->setMat4("model", model);
    shader->setVec3("objectColor", object->getColor());

    // Set view position for specular lighting
    glm::mat4 invView = glm::inverse(viewMatrix);
    glm::vec3 viewPos = glm::vec3(invView[3]); // Extract camera position from inverse view matrix
    shader->setVec3("viewPos", viewPos);

    // Update lighting information for each light
    shader->setInt("numLights", static_cast<int>(lights.size()));
    for (size_t i = 0; i < lights.size(); ++i) {
        const auto& light = lights[i];
        std::string index = std::to_string(i);

        shader->setVec3("lights[" + index + "].position", light->getPosition());
        shader->setVec3("lights[" + index + "].color", light->getColor());
        shader->setFloat("lights[" + index + "].intensity", light->getIntensity());
        shader->setFloat("lights[" + index + "].ambientStrength", light->getAmbientStrength());
        shader->setFloat("lights[" + index + "].diffuseStrength", light->getDiffuseStrength());
        shader->setFloat("lights[" + index + "].specularStrength", light->getSpecularStrength());
        shader->setFloat("lights[" + index + "].shininess", light->getShininess());
    }

    // The object will handle its own drawing after shader setup
    object->draw(nullptr);
}

