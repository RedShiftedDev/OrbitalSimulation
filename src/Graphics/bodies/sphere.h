#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Graphics/core/Object3D.h"
#include "Graphics/core/ObjectVisitor.h"
#include "Graphics/renderer.h"

class Sphere : public Object3D {
public:
  Sphere(float radius = 1.0f, int sectors = 36, int stacks = 18);
  ~Sphere() override = default;

  // Implement Object3D interface
  void setPosition(const glm::vec3 &pos) override { position = pos; }
  glm::vec3 getPosition() const override { return position; }

  void setScale(const glm::vec3 &s) override { scale = s; }
  glm::vec3 getScale() const override { return scale; }

  void setColor(const glm::vec3 &c) override { color = c; }
  glm::vec3 getColor() const override { return color; }

  void draw(Renderer *renderer) override; // Changed from Renderer& to Renderer*
  void accept(ObjectVisitor &visitor) override { visitor.visit(*this); }
  void accept(RenderVisitor &visitor) override { visitor.visit(this); }

  // Implement SceneObject interface
  const std::string &getType() const override {
    static const std::string type = "Sphere";
    return type;
  }

  // Sphere-specific methods
  void setResolution(int sectors, int stacks);
  int getSectorCount() const { return sectors; }
  int getStackCount() const { return stacks; }
  float getRadius() const { return radius; }
  void setRadius(const float radius) {
    this->radius = radius;
    build();
    setupBuffers();
  }

private:
  void build();
  void buildVertices();
  void buildIndices();
  void setupBuffers();

  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> interleavedVertices;
  std::vector<unsigned int> indices;
  unsigned int VAO{}, VBO{}, EBO{};

  float radius;
  int sectors;
  int stacks;
  glm::vec3 position{0.0F};
  glm::vec3 scale{1.0F};
  glm::vec3 color{1.0F};
};
