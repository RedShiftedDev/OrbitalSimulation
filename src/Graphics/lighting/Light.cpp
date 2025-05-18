#include "Graphics/lighting/Light.h"

Light::Light(const std::string& name, const glm::vec3& position, 
             const glm::vec3& color, float intensity)
    : name(name), position(position), color(color), intensity(intensity) {}
