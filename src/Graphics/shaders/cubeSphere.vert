#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Calculate fragment position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform normal to world space while handling non-uniform scaling
    // The transpose(inverse(mat3(model))) corrects normal transformation under non-uniform scaling
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    
    // Transform vertex position to clip space
    gl_Position = projection * view * vec4(FragPos, 1.0);
}