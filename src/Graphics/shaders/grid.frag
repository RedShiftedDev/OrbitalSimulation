#version 410 core
out vec4 FragColor;

in vec3 FragPos;
in float DistanceFromCamera;

uniform vec3 gridColor;
uniform vec3 majorGridColor;
uniform float fadeDistance;
uniform float gridSize;
uniform int gridDivisions;
uniform float majorGridSpacing;
uniform float minorLineWidth;
uniform float majorLineWidth;
uniform float gridOpacity;
uniform bool showAxisLines;
uniform vec3 xAxisColor;
uniform vec3 zAxisColor;

float getGrid(vec2 coord, float size, float width) {
    vec2 grid = abs(fract(coord / size - 0.5) - 0.5) / fwidth(coord / size);
    return 1.0 - min(min(grid.x, grid.y), 1.0) * width;
}

void main() {
    float cellSize = gridSize / float(gridDivisions);
    vec2 coord = FragPos.xz;
    
    // Calculate fade based on distance
    float fade = 1.0 - (DistanceFromCamera / fadeDistance);
    fade = clamp(fade, 0.0, 1.0);
    
    // Minor grid
    float minorGrid = getGrid(coord, cellSize, minorLineWidth);
    
    // Major grid
    float majorGrid = getGrid(coord, cellSize * majorGridSpacing, majorLineWidth);
    
    // Combine grids
    vec3 color = mix(gridColor, majorGridColor, majorGrid);
    float alpha = max(minorGrid, majorGrid) * gridOpacity * fade;
    
    // Add axis lines
    if (showAxisLines) {
        float axisWidth = majorLineWidth * 2.0;
        float xAxis = 1.0 - smoothstep(0.0, fwidth(coord.y) * axisWidth, abs(coord.y));
        float zAxis = 1.0 - smoothstep(0.0, fwidth(coord.x) * axisWidth, abs(coord.x));
        
        if (xAxis > 0.0) {
            color = xAxisColor;
            alpha = max(alpha, xAxis * fade);
        }
        if (zAxis > 0.0) {
            color = zAxisColor;
            alpha = max(alpha, zAxis * fade);
        }
    }
    
    FragColor = vec4(color, alpha);
}
