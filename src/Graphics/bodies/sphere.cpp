#include "sphere.h"
#include <glad/glad.h> // Fixed include path
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/core/RenderVisitor.h"

Sphere::Sphere(float r, int sectors, int stacks)
    : radius(r), sectors(sectors), stacks(stacks) {
    build();
    setupBuffers();
}

void Sphere::setResolution(int newSectors, int newStacks) {
    sectors = newSectors;
    stacks = newStacks;
    build();
    setupBuffers();
}

void Sphere::build() {
    vertices.clear();
    normals.clear();
    indices.clear();
    buildVertices();
    buildIndices();
}

void Sphere::buildVertices() {
    float sectorStep = 2 * M_PI / sectors;
    float stackStep = M_PI / stacks;

    // Generate vertices and normals
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2 - i * stackStep;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * sectorStep;

            // Vertex position
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Normalized vertex normal
            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);
        }
    }
}

void Sphere::buildIndices() {
    // Generate indices
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Sphere::setupBuffers() {
    // Interleave vertex attributes
    interleavedVertices.clear();
    std::size_t vertexCount = vertices.size() / 3;
    for (std::size_t i = 0; i < vertexCount; ++i) {
        interleavedVertices.push_back(vertices[i * 3]);     // x
        interleavedVertices.push_back(vertices[i * 3 + 1]); // y
        interleavedVertices.push_back(vertices[i * 3 + 2]); // z
        interleavedVertices.push_back(normals[i * 3]);      // nx
        interleavedVertices.push_back(normals[i * 3 + 1]);  // ny
        interleavedVertices.push_back(normals[i * 3 + 2]);  // nz
    }

    // Create and setup buffers
    if (VAO == 0) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    }

    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, interleavedVertices.size() * sizeof(float),
                 interleavedVertices.data(), GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::draw(Renderer* renderer) {
    // Basic drawing without any transformation - those are handled by the visitor
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// void Sphere::accept(RenderVisitor& visitor) {
//     visitor.visit(this);
// }

