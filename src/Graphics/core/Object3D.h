#pragma once
#include <glm/glm.hpp>
#include "ObjectVisitor.h"
#include "RenderVisitor.h"
#include "SceneObject.h"

class Renderer;
class RenderVisitor;

class Object3D : public SceneObject {
public:
    ~Object3D() override = default;

    // SceneObject interface implementation
    void accept(ObjectVisitor& visitor) override {
        if (auto* renderVisitor = dynamic_cast<RenderVisitor*>(&visitor)) {
            accept(*renderVisitor);
        }
    }

    // Object3D specific interface
    virtual void setPosition(const glm::vec3& pos) = 0;
    virtual glm::vec3 getPosition() const = 0;

    virtual void setScale(const glm::vec3& scale) = 0;
    virtual glm::vec3 getScale() const = 0;

    virtual void setColor(const glm::vec3& color) = 0;
    virtual glm::vec3 getColor() const = 0;

    // Draw method can be called with nullptr when drawing is handled by visitor
    virtual void draw(Renderer* renderer) = 0;
    virtual void accept(RenderVisitor& visitor) = 0;
};

