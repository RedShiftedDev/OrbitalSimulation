#include <glad/glad.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <imgui.h>
#include <iostream>
#include "core/fps_counter.h"
#include "GUI/gui.h"
#include "core/window.h"
#include "Graphics/renderer.h"
#include "Graphics/sphere.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {
  try {
    Window window("Orbital Simulation", 1280, 720);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)) == 0) {
      std::cerr << "Failed to initialize GLAD\n";
      return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(window.getSDLWindow(), window.getGLContext());
    ImGui_ImplOpenGL3_Init("#version 410");

    // Initialize renderer and create sphere
    Renderer renderer;
    Sphere sphere(1.0f, 36, 18); // radius = 1.0, 36 sectors, 18 stacks

    // Set initial settings
    auto& settings = renderer.getSettings();
    settings.fieldOfView = 45.0f;
    settings.cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
    settings.cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    renderer.updateProjection();

    // Set initial background color
    glClearColor(settings.backgroundColor.r, settings.backgroundColor.g, settings.backgroundColor.b, 1.0f);
    glEnable(GL_DEPTH_TEST);

    FpsCounter fpsCounter;

    while (!window.shouldClose()) {
      window.pollEvents();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL3_NewFrame();
      ImGui::NewFrame();

      gui::RenderGui(fpsCounter, sphere, renderer);

      ImGui::Render();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Render the sphere
      renderer.render(sphere);

      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      window.swapBuffers();

      fpsCounter.update();
    }

  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return -1;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  return 0;
}
