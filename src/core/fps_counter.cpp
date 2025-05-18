#include "fps_counter.h"


void FpsCounter::update() {
  using namespace std::chrono;
  const auto currentTime = high_resolution_clock::now();
  deltaTime = duration_cast<milliseconds>(currentTime - lastTime).count() / 1000.0f;
  lastTime = currentTime;

  frameCount++;
  elapsedTime += deltaTime;

  if (elapsedTime >= 1.0f) {
    fps = frameCount / elapsedTime;
    frameCount = 0;
    elapsedTime = 0.0f;
  }
}

float FpsCounter::getFps() const {
  return fps;
}

float FpsCounter::getDeltaTime() const {
  return deltaTime;
}
