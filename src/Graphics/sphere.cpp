#include "sphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Sphere::Sphere(float r, int sectors, int stacks)
    : radius(r), sectorCount(sectors), stackCount(stacks) {
    buildVertices();
    setupBuffers();
}

Sphere::~Sphere() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Sphere::buildVertices() {
    float x, y, z, xy;
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            // vertex position
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal
            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);
        }
    }

    // indices
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Sphere::setupBuffers() {
    std::vector<float> interleavedVertices;
    for (size_t i = 0; i < vertices.size() / 3; ++i) {
        interleavedVertices.push_back(vertices[i * 3]);     // x
        interleavedVertices.push_back(vertices[i * 3 + 1]); // y
        interleavedVertices.push_back(vertices[i * 3 + 2]); // z
        interleavedVertices.push_back(normals[i * 3]);      // nx
        interleavedVertices.push_back(normals[i * 3 + 1]);  // ny
        interleavedVertices.push_back(normals[i * 3 + 2]);  // nz
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, interleavedVertices.size() * sizeof(float),
                interleavedVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int),
                indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Sphere::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

glm::mat4 Sphere::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    return model;
}
