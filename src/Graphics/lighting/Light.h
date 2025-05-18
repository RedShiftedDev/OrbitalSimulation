#pragma once

#include <glm/glm.hpp>
#include <string>

class Light {
public:
    Light(const std::string& name = "Light", 
          const glm::vec3& position = glm::vec3(0.0f, 5.0f, 5.0f),
          const glm::vec3& color = glm::vec3(1.0f),
          float intensity = 1.0f);

    // Getters
    const std::string& getName() const { return name; }
    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getColor() const { return color; }
    float getIntensity() const { return intensity; }
    float getAmbientStrength() const { return ambientStrength; }
    float getDiffuseStrength() const { return diffuseStrength; }
    float getSpecularStrength() const { return specularStrength; }
    float getShininess() const { return shininess; }

    // Setters
    void setPosition(const glm::vec3& pos) { position = pos; }
    void setColor(const glm::vec3& col) { color = col; }
    void setIntensity(float i) { intensity = i; }
    void setAmbientStrength(float strength) { ambientStrength = strength; }
    void setDiffuseStrength(float strength) { diffuseStrength = strength; }
    void setSpecularStrength(float strength) { specularStrength = strength; }
    void setShininess(float shine) { shininess = shine; }

private:
    std::string name;
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float ambientStrength = 0.1f;
    float diffuseStrength = 0.7f;
    float specularStrength = 0.5f;
    float shininess = 32.0f;
};
