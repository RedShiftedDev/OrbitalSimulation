#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out float DistanceFromCamera;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    
    vec4 viewPos = view * worldPos;
    DistanceFromCamera = length(viewPos.xyz);
    
    gl_Position = projection * viewPos;
}
