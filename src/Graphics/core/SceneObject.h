#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>

class ObjectVisitor;

class SceneObject {
public:
    virtual ~SceneObject() = default;
    virtual void accept(ObjectVisitor& visitor) = 0;

    void setPosition(const glm::vec3& pos) { position = pos; }
    void setRotation(const glm::vec3& rot) { rotation = rot; }
    void setScale(const glm::vec3& scl) { scale = scl; }

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getRotation() const { return rotation; }
    const glm::vec3& getScale() const { return scale; }

    virtual const std::string& getType() const = 0;

protected:
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 scale{1.0f};
};
