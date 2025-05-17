#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

class Sphere {
public:
    Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18);
    ~Sphere();

    void draw() const;

    // Getters and setters for transformation
    void setPosition(const glm::vec3& pos) { position = pos; }
    void setScale(const glm::vec3& s) { scale = s; }
    void setColor(const glm::vec3& c) { color = c; }

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getScale() const { return scale; }
    const glm::vec3& getColor() const { return color; }

    glm::mat4 getModelMatrix() const;

private:
    void buildVertices();
    void setupBuffers();

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<int> indices;

    unsigned int VAO, VBO, EBO;
    float radius;
    int sectorCount;
    int stackCount;

    glm::vec3 position{0.0f};
    glm::vec3 scale{1.0f};
    glm::vec3 color{1.0f, 0.5f, 0.2f};
};
