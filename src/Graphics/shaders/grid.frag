#version 410 core
out vec4 FragColor;

uniform vec3 gridColor;
uniform float fadeDistance;

void main() {
    float dist = length(gl_FragCoord.xy / gl_FragCoord.w);
    float alpha = 1.0 - (dist / fadeDistance);
    FragColor = vec4(gridColor, alpha * 0.5);
}
