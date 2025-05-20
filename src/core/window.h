#pragma once

#include <SDL3/SDL.h>

class Window {
public:
  Window(const Window &) = default;
  Window(Window &&) = delete;
  Window &operator=(const Window &) = default;
  Window &operator=(Window &&) = delete;
  Window(const char *title, int width, int height);
  ~Window();

  void swapBuffers() const;
  bool shouldClose() const;

  void pollEvents();
  SDL_Window *getSDLWindow() const;
  SDL_GLContext getGLContext() const;

  int getWidth() const { return width; }
  int getHeight() const { return height; }

  bool getState() const { return this->isRunning; }
  void setStateRunning() { this->isRunning = true; }
  void setState(bool state) { this->isRunning = state; }
  void close() { setState(false); }

private:
  SDL_Window *window = nullptr;
  SDL_GLContext glContext = nullptr;
  bool isRunning;
  int width = 0;
  int height = 0;
};
