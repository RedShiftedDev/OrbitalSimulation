#pragma once

#include <chrono>

class FpsCounter {
public:
  void update();

  [[nodiscard]] float getFps() const;
  [[nodiscard]] float getDeltaTime() const;

private:
  std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();
  float deltaTime = 0.0f;
  float fps = 0.0f;
  int frameCount = 0;
  float elapsedTime = 0.0f;
};
