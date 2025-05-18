#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "Graphics/shader.h"
#include "Graphics/lighting/Light.h"

class Object3D;

class RenderVisitor {
public:
    RenderVisitor(const glm::mat4& view, const glm::mat4& projection)
        : viewMatrix(view), projectionMatrix(projection) {}

    void updateMatrices(const glm::mat4& view, const glm::mat4& projection) {
        viewMatrix = view;
        projectionMatrix = projection;
    }

    void registerShader(const std::string& objectType, const std::shared_ptr<Shader>& shader) {
        shaders[objectType] = shader;
    }

    void setLights(const std::vector<std::shared_ptr<Light>>& newLights) {
        lights = newLights;
    }

    void visit(Object3D* object);

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    std::map<std::string, std::shared_ptr<Shader>> shaders;
    std::vector<std::shared_ptr<Light>> lights;
};
