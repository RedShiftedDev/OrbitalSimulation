#include <glad/glad.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <iostream>
#include "GUI/gui.h"
#include "GUI/Scene.h"
#include "Graphics/renderer.h"
#include "Graphics/bodies/sphere.h"
#include "Graphics/bodies/cubeSphere.h"
#include "core/fps_counter.h"
#include "core/window.h"

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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(window.getSDLWindow(), window.getGLContext());
    ImGui_ImplOpenGL3_Init("#version 410");

    // Initialize the renderer and create a scene
    Renderer renderer(window);

    // Create and add a sphere to a scene
    auto sphere = std::make_shared<Sphere>(1.0F, 36, 18);
    sphere->setPosition(glm::vec3(-2.0F, 0.0F, 0.0F));  // Position sphere to the left
    sphere->setColor(glm::vec3(1.0F, 0.5F, 0.2F));      // Orange color
    gui::getScene().addObject(sphere, "Sphere");

    // Create and add a cube sphere to a scene
    auto cubeSphere = std::make_shared<CubeSphere>(1.0F, 24);
    cubeSphere->setPosition(glm::vec3(2.0F, 0.0F, 0.0F));  // Position a cube sphere to the right
    cubeSphere->setColor(glm::vec3(0.2F, 0.6F, 1.0F));     // Blue color
    gui::getScene().addObject(cubeSphere, "CubeSphere");

    // Set initial settings
    auto &settings = renderer.getSettings();
    settings.fieldOfView = 45.0F;
    settings.cameraPosition = glm::vec3(0.0F, 0.0F, 5.0F);
    settings.cameraTarget = glm::vec3(0.0F, 0.0F, 0.0F);
    renderer.updateProjection();

    // Set the initial background color
    glClearColor(settings.backgroundColor.r, settings.backgroundColor.g, settings.backgroundColor.b,
                 1.0F);
    glEnable(GL_DEPTH_TEST);

    renderer.init();
    FpsCounter fpsCounter;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                running = false;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Update FPS counter
        fpsCounter.update();

        // Render GUI
        gui::RenderGui(fpsCounter, sphere, cubeSphere, renderer);

        // Render scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render main scene objects
        const gui::Scene& scene = gui::getScene();
        renderer.render(scene);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return -1;
  }
}
