#include "cubeSphere.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

CubeSphere::CubeSphere(float s, int res) : size(s), resolution(res) {
  rebuild();
  setupBuffers();
}

void CubeSphere::setResolution(int res) {
  resolution = res;
  rebuild();
  setupBuffers();
}

glm::vec3 CubeSphere::spherify(const glm::vec3 &point) const {
  float x2 = point.x * point.x;
  float y2 = point.y * point.y;
  float z2 = point.z * point.z;

  float x = point.x * sqrt(1.0f - (y2 + z2) / 2.0f + (y2 * z2) / 3.0f);
  float y = point.y * sqrt(1.0f - (x2 + z2) / 2.0f + (x2 * z2) / 3.0f);
  float z = point.z * sqrt(1.0f - (x2 + y2) / 2.0f + (x2 * y2) / 3.0f);

  return glm::vec3(x, y, z) * size;
}

void CubeSphere::rebuild() {
  vertices.clear();
  normals.clear();
  indices.clear();
  buildVertices();
  buildIndices();
}

void CubeSphere::buildVertices() {
  float step = 2.0f / resolution;

  // Generate vertices for each face
  for (int face = 0; face < 6; ++face) {
    glm::vec3 normal;
    float fixed;

    switch (face) {
    case 0:
      normal = glm::vec3(1, 0, 0);
      fixed = 1;
      break; // +X
    case 1:
      normal = glm::vec3(-1, 0, 0);
      fixed = -1;
      break; // -X
    case 2:
      normal = glm::vec3(0, 1, 0);
      fixed = 1;
      break; // +Y
    case 3:
      normal = glm::vec3(0, -1, 0);
      fixed = -1;
      break; // -Y
    case 4:
      normal = glm::vec3(0, 0, 1);
      fixed = 1;
      break; // +Z
    case 5:
      normal = glm::vec3(0, 0, -1);
      fixed = -1;
      break; // -Z
    default:
      continue;
    }

    for (int i = 0; i <= resolution; ++i) {
      float y = -1.0f + i * step;
      for (int j = 0; j <= resolution; ++j) {
        float x = -1.0f + j * step;

        glm::vec3 point;
        if (face < 2) { // X faces
          point = glm::vec3(fixed, x, y);
        } else if (face < 4) { // Y faces
          point = glm::vec3(x, fixed, y);
        } else { // Z faces
          point = glm::vec3(x, y, fixed);
        }

        glm::vec3 spherePoint = spherify(point);
        vertices.push_back(spherePoint.x);
        vertices.push_back(spherePoint.y);
        vertices.push_back(spherePoint.z);

        // Calculate normal as the normalized position
        glm::vec3 normalizedPos = glm::normalize(spherePoint);
        normals.push_back(normalizedPos.x);
        normals.push_back(normalizedPos.y);
        normals.push_back(normalizedPos.z);
      }
    }
  }
}

void CubeSphere::buildIndices() {
  int verticesPerRow = resolution + 1;
  int facesOffset = verticesPerRow * verticesPerRow;

  for (int face = 0; face < 6; ++face) {
    int offset = face * facesOffset;

    for (int i = 0; i < resolution; ++i) {
      for (int j = 0; j < resolution; ++j) {
        int index = offset + i * verticesPerRow + j;

        indices.push_back(index);
        indices.push_back(index + verticesPerRow);
        indices.push_back(index + 1);

        indices.push_back(index + 1);
        indices.push_back(index + verticesPerRow);
        indices.push_back(index + verticesPerRow + 1);
      }
    }
  }
}

void CubeSphere::setupBuffers() {
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
               GL_STATIC_DRAW);

  // Vertex attributes
  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // Normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void CubeSphere::draw(Renderer *renderer) {
  // Basic drawing without any transformation - those are handled by the visitor
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
