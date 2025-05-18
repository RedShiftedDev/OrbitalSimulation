#version 410 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

uniform Light lights[10];  // Array of lights
uniform int numLights;    // Number of active lights
uniform vec3 viewPos;
uniform vec3 objectColor;

void main() {
    vec3 result = vec3(0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Calculate contribution from each light
    for(int i = 0; i < numLights; i++) {
        // ambient
        vec3 ambient = lights[i].ambientStrength * lights[i].color;

        // diffuse
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lights[i].diffuseStrength * diff * lights[i].color;

        // specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), lights[i].shininess);
        vec3 specular = lights[i].specularStrength * spec * lights[i].color;

        // combine and apply light intensity
        result += (ambient + diffuse + specular) * lights[i].intensity;
    }

    // Apply final color
    result *= objectColor;
    FragColor = vec4(result, 1.0);
}
