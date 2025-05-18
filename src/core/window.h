#pragma once

#include <SDL3/SDL.h>

class Window {
public:
  Window(const char *title, int width, int height);
  ~Window();

  void swapBuffers() const;
  bool shouldClose() const;

  void pollEvents();
  SDL_Window *getSDLWindow() const;
  SDL_GLContext getGLContext() const;

  int getWidth() const { return width; }
  int getHeight() const { return height; }

private:
  SDL_Window *window = nullptr;
  SDL_GLContext glContext = nullptr;
  bool isRunning = true;
  int width = 0;
  int height = 0;
};
