#include "GUI/gui.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>
#include <string>
#include <vector>
#include "Graphics/bodies/cubeSphere.h"
#include "Graphics/bodies/sphere.h"
#include "Graphics/core/RenderVisitor.h"
#include "Graphics/renderer.h"
#include "Scene.h"
#include "core/fps_counter.h"

namespace gui {

// Constants for styling
constexpr ImVec4 HEADER_COLOR = ImVec4(0.2F, 0.4F, 0.8F, 1.0F);
constexpr ImVec4 ACTIVE_TAB_COLOR = ImVec4(0.2F, 0.4F, 0.8F, 0.7F);
constexpr ImVec4 INACTIVE_TAB_COLOR = ImVec4(0.2F, 0.4F, 0.8F, 0.3F);
constexpr float WINDOW_PADDING = 10.0F;
constexpr float WINDOW_ROUNDING = 5.0F;
constexpr float FRAME_ROUNDING = 4.0F;
constexpr float ITEM_SPACING = 8.0F;

// GUI state variables
struct GuiState {
  int currentTab = 0;
  bool showPerformance = true;
  bool showControlPanel = true;
  float mainPanelWidth = 350.0F;
  bool showHelpTooltips = true;
  std::vector<std::string> presets;
  int selectedPreset = -1;
  glm::vec3 lastSavedCameraPos = glm::vec3(0.0F);
  glm::vec3 lastSavedCameraTarget = glm::vec3(0.0F);
  float splitRatio = 0.5F;
};

// Initialize static GUI state
static GuiState guiState;

namespace {
// Static instance for the Scene singleton
Scene instance;

void CreateNewObject(const std::string &name, const std::string &type, const glm::vec3 &position,
                     const glm::vec3 &color, float size, int resolution) {
  std::shared_ptr<Object3D> newObject;

  if (type == "Sphere") {
    newObject = std::make_shared<Sphere>(size, resolution);
    newObject->setPosition(position);
    newObject->setColor(color);
  } else if (type == "CubeSphere") {
    newObject = std::make_shared<CubeSphere>(size, resolution);
    newObject->setPosition(position);
    newObject->setColor(color);
  }

  if (newObject) {
    instance.addObject(newObject, name);
  }
}
} // namespace

Scene &getScene() { return instance; }

// Helper functions
void SetupStyle() {
  ImGuiStyle &style = ImGui::GetStyle();
  style.WindowPadding = ImVec2(WINDOW_PADDING, WINDOW_PADDING);
  style.WindowRounding = WINDOW_ROUNDING;
  style.FrameRounding = FRAME_ROUNDING;
  style.ItemSpacing = ImVec2(ITEM_SPACING, ITEM_SPACING);
  style.Colors[ImGuiCol_Header] = HEADER_COLOR;
  style.Colors[ImGuiCol_HeaderHovered] =
      ImVec4(HEADER_COLOR.x + 0.1F, HEADER_COLOR.y + 0.1F, HEADER_COLOR.z + 0.1F, HEADER_COLOR.w);
  style.Colors[ImGuiCol_HeaderActive] =
      ImVec4(HEADER_COLOR.x + 0.2F, HEADER_COLOR.y + 0.2F, HEADER_COLOR.z + 0.2F, HEADER_COLOR.w);
  style.Colors[ImGuiCol_TitleBgActive] = HEADER_COLOR;
}

void HelpMarker(const char *desc) {
  if (!guiState.showHelpTooltips) {
    return;
  }

  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0F);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

bool TabButton(const char *label, bool selected) {
  ImGuiStyle &style = ImGui::GetStyle();
  // Fix operator precedence with parentheses and use uppercase F suffix
  float width = ImGui::CalcTextSize(label).x + (style.FramePadding.x * 2.0F);
  float height = ImGui::GetFrameHeight();

  ImGui::PushStyleColor(ImGuiCol_Button, selected ? ACTIVE_TAB_COLOR : INACTIVE_TAB_COLOR);
  bool pressed = ImGui::Button(label, ImVec2(width, height));
  ImGui::PopStyleColor();

  return pressed;
}

bool Vec3Control(const char *label, glm::vec3 &values, float resetValue = 0.0F, float speed = 0.1F,
                 float min = 0.0F, float max = 0.0F, const char *format = "%.2f") {
  bool changed = false;

  ImGui::PushID(label);
  ImGui::Columns(2);
  ImGui::SetColumnWidth(0, ImGui::CalcTextSize(label).x + 20.0F);
  ImGui::Text("%s", label);
  ImGui::NextColumn();

  ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8F, 0.1F, 0.15F, 1.0F));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9F, 0.2F, 0.2F, 1.0F));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8F, 0.1F, 0.15F, 1.0F));
  if (ImGui::Button("X", ImVec2(20, 0))) {
    values.x = resetValue;
    changed = true;
  }
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  changed |= ImGui::DragFloat("##X", &values.x, speed, min, max, format);
  ImGui::PopItemWidth();
  ImGui::SameLine();

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2F, 0.7F, 0.2F, 1.0F));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3F, 0.8F, 0.3F, 1.0F));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2F, 0.7F, 0.2F, 1.0F));
  if (ImGui::Button("Y", ImVec2(20, 0))) {
    values.y = resetValue;
    changed = true;
  }
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  changed |= ImGui::DragFloat("##Y", &values.y, speed, min, max, format);
  ImGui::PopItemWidth();
  ImGui::SameLine();

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1F, 0.25F, 0.8F, 1.0F));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2F, 0.35F, 0.9F, 1.0F));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1F, 0.25F, 0.8F, 1.0F));
  if (ImGui::Button("Z", ImVec2(20, 0))) {
    values.z = resetValue;
    changed = true;
  }
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  changed |= ImGui::DragFloat("##Z", &values.z, speed, min, max, format);
  ImGui::PopItemWidth();

  ImGui::Columns(1);
  ImGui::PopID();

  // // Link scale values if needed
  // if (guiState.linkScaleValues && std::string(label) == "Scale") {
  //   if (values.x != values.y || values.x != values.z) {
  //     values.y = values.x;
  //     values.z = values.x;
  //     changed = true;
  //   }
  // }

  return changed;
}

bool ColorControl(const char *label, glm::vec3 &color) {
  ImGui::PushID(label);
  ImGui::Columns(2);
  ImGui::SetColumnWidth(0, ImGui::CalcTextSize(label).x + 20.0F);
  ImGui::Text("%s", label);
  ImGui::NextColumn();

  const bool changed = ImGui::ColorEdit3(
      "##color", &color[0], ImGuiColorEditFlags_Float | ImGuiColorEditFlags_PickerHueWheel);

  ImGui::Columns(1);
  ImGui::PopID();

  return changed;
}

void SaveCameraPreset(const char *name, const glm::vec3 &position, const glm::vec3 &target) {
  // In a real implementation, you would save to a file or configuration
  guiState.presets.push_back(name);
  guiState.lastSavedCameraPos = position;
  guiState.lastSavedCameraTarget = target;
}

void PresetSelector(const char *label, int &selected, const std::vector<std::string> &items) {
  if (ImGui::BeginCombo(label, selected >= 0 && selected < items.size() ? items[selected].c_str()
                                                                        : "Select Preset")) {
    for (int i = 0; i < items.size(); i++) {
      bool isSelected = (selected == i);
      if (ImGui::Selectable(items[i].c_str(), isSelected)) {
        selected = i;
      }
      if (isSelected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }
}

void MainMenuBar(const FpsCounter &fpsCounter) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New Scene", "Ctrl+N")) {
      }
      if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {
      }
      if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
      }
      if (ImGui::MenuItem("Export...", "Ctrl+E")) {
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit", "Alt+F4")) {
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
      }
      if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Preferences")) {
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("View")) {
      ImGui::MenuItem("Performance", nullptr, &guiState.showPerformance);
      ImGui::MenuItem("Control Panel", nullptr, &guiState.showControlPanel);
      ImGui::MenuItem("Show Help Tooltips", nullptr, &guiState.showHelpTooltips);
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help")) {
      if (ImGui::MenuItem("Documentation")) {
      }
      if (ImGui::MenuItem("About")) {
      }
      ImGui::EndMenu();
    }

    // Right-aligned FPS counter
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 120);
    ImGui::Text("FPS: %.1f", fpsCounter.getFps());

    ImGui::EndMainMenuBar();
  }
}

void PerformanceWindow(const FpsCounter &fpsCounter) {
  if (!guiState.showPerformance) {
    return;
  }

  ImGui::Begin("Performance Metrics", &guiState.showPerformance);

  ImGui::Text("FPS: %.1f", fpsCounter.getFps());
  ImGui::SameLine();
  HelpMarker("Frames per second - higher is better");

  ImGui::Text("Frame Time: %.2f ms", 1000.0F / fpsCounter.getFps());

  // Simple graph for FPS
  static float values[90] = {};
  static int values_offset = 0;
  static float refresh_time = 0.0F;

  float current_time = ImGui::GetTime();
  if (current_time - refresh_time >= 0.1F) { // Update every 100 ms
    values[values_offset] = fpsCounter.getFps();
    values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
    refresh_time = current_time;
  }

  float average = 0.0F;
  for (int n = 0; n < IM_ARRAYSIZE(values); n++) {
    average += values[n];
  }
  average /= static_cast<float>(IM_ARRAYSIZE(values));

  char overlay[32];
  snprintf(overlay, sizeof(overlay), "Avg %.1f FPS", average);
  ImGui::PlotLines("##FPS", values, IM_ARRAYSIZE(values), values_offset, overlay, 0.0F, 120.0F,
                   ImVec2(0, 80.0F));

  ImGui::End();
}

void RenderSceneControls(Renderer &renderer, bool &sceneUpdated) {
  auto &settings = renderer.getSettings();

  // Camera Controls
  if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
    sceneUpdated |= Vec3Control("Position", settings.cameraPosition, 0.0F, 0.1F);
    sceneUpdated |= Vec3Control("Target", settings.cameraTarget, 0.0F, 0.1F);

    sceneUpdated |= ImGui::SliderFloat("Field of View", &settings.fieldOfView, 10.0F, 120.0F);
    ImGui::SameLine();
    HelpMarker("Controls camera zoom level");

    // Camera Presets
    ImGui::Separator();
    ImGui::Text("Camera Presets");
    ImGui::SameLine();
    HelpMarker("Save and load camera positions");

    static char presetName[64] = "Default View";
    ImGui::InputText("Name", presetName, IM_ARRAYSIZE(presetName));

    if (ImGui::Button("Save Current")) {
      SaveCameraPreset(presetName, settings.cameraPosition, settings.cameraTarget);
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset Camera")) {
      settings.cameraPosition = glm::vec3(0.0F, 0.0F, 5.0F);
      settings.cameraTarget = glm::vec3(0.0F, 0.0F, 0.0F);
      sceneUpdated = true;
    }

    if (!guiState.presets.empty()) {
      PresetSelector("Load Preset", guiState.selectedPreset, guiState.presets);

      if (guiState.selectedPreset >= 0 && ImGui::Button("Apply Selected")) {
        settings.cameraPosition = guiState.lastSavedCameraPos;
        settings.cameraTarget = guiState.lastSavedCameraTarget;
        sceneUpdated = true;
      }
    }
  }

  // Scene Options
  if (ImGui::CollapsingHeader("Scene Options", ImGuiTreeNodeFlags_DefaultOpen)) {
    bool wireframeMode = renderer.isWireframe();
    if (ImGui::Checkbox("Wireframe Mode", &wireframeMode)) {
      renderer.setWireframe(wireframeMode);
    }

    // Grid Controls
    if (ImGui::TreeNode("Grid Settings")) {
      bool gridUpdated = false;

      gridUpdated |= ImGui::Checkbox("Show Grid", &settings.showGrid);

      if (settings.showGrid) {
        // Basic Grid Settings
        if (ImGui::CollapsingHeader("Basic Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
          gridUpdated |= ImGui::SliderFloat("Grid Size", &settings.gridSize, 5.0f, 100.0f, "%.1f");
          ImGui::SameLine();
          HelpMarker("Controls the total size of the grid");

          gridUpdated |= ImGui::SliderInt("Grid Divisions", &settings.gridDivisions, 5, 100);
          ImGui::SameLine();
          HelpMarker("Controls how many lines make up the grid");

          gridUpdated |=
              ImGui::SliderFloat("Grid Opacity", &settings.gridOpacity, 0.0f, 1.0f, "%.2f");
          ImGui::SameLine();
          HelpMarker("Controls overall grid transparency");
        }

        // Visual Settings
        if (ImGui::CollapsingHeader("Visual Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
          gridUpdated |= ColorControl("Minor Grid Color", settings.gridColor);
          gridUpdated |= ColorControl("Major Grid Color", settings.majorGridColor);

          gridUpdated |=
              ImGui::SliderFloat("Minor Line Width", &settings.minorLineWidth, 0.5f, 3.0f, "%.1f");
          gridUpdated |=
              ImGui::SliderFloat("Major Line Width", &settings.majorLineWidth, 1.0f, 5.0f, "%.1f");

          gridUpdated |= ImGui::SliderFloat("Major Grid Spacing", &settings.majorGridSpacing, 2.0f,
                                            10.0f, "%.0f");
          ImGui::SameLine();
          HelpMarker("Number of minor grid cells between major grid lines");
        }

        // Axis Settings
        if (ImGui::CollapsingHeader("Axis Lines", ImGuiTreeNodeFlags_DefaultOpen)) {
          gridUpdated |= ImGui::Checkbox("Show Axis Lines", &settings.showAxisLines);
          if (settings.showAxisLines) {
            gridUpdated |= ColorControl("X-Axis Color", settings.xAxisColor);
            gridUpdated |= ColorControl("Z-Axis Color", settings.zAxisColor);
          }
        }

        // Fade Settings
        if (ImGui::CollapsingHeader("Fade Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
          gridUpdated |= ImGui::SliderFloat("Fade Distance", &settings.gridFadeDistance, 10.0f,
                                            200.0f, "%.1f");
          ImGui::SameLine();
          HelpMarker("Distance at which the grid fades out");
        }

        // Grid Presets
        if (ImGui::TreeNode("Grid Presets")) {
          if (ImGui::Button("Engineering")) {
            settings.gridColor = glm::vec3(0.2f, 0.2f, 0.2f);
            settings.majorGridColor = glm::vec3(0.4f, 0.4f, 0.4f);
            settings.majorGridSpacing = 5.0f;
            settings.gridOpacity = 0.7f;
            settings.showAxisLines = true;
            gridUpdated = true;
          }
          ImGui::SameLine();
          if (ImGui::Button("Blueprint")) {
            settings.gridColor = glm::vec3(0.1f, 0.3f, 0.6f);
            settings.majorGridColor = glm::vec3(0.2f, 0.4f, 0.8f);
            settings.majorGridSpacing = 4.0f;
            settings.gridOpacity = 0.5f;
            settings.showAxisLines = true;
            gridUpdated = true;
          }
          ImGui::SameLine();
          if (ImGui::Button("Minimal")) {
            settings.gridColor = glm::vec3(0.15f);
            settings.majorGridColor = glm::vec3(0.3f);
            settings.majorGridSpacing = 2.0f;
            settings.gridOpacity = 0.3f;
            settings.showAxisLines = false;
            gridUpdated = true;
          }
          ImGui::TreePop();
        }
      }

      if (gridUpdated) {
        renderer.initGrid();
      }

      ImGui::TreePop();
    }
  }
}

void RenderLightingControls(std::vector<std::shared_ptr<Light>> &lights) {
  if (ImGui::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_DefaultOpen)) {
    // Add a Light button
    if (ImGui::Button("Add Light")) {
      lights.push_back(std::make_shared<Light>("Light " + std::to_string(lights.size() + 1)));
    }

    // Light list
    for (size_t i = 0; i < lights.size(); i++) {
      auto &light = lights[i];
      if (ImGui::TreeNode((light->getName() + "##" + std::to_string(i)).c_str())) {
        // Position control
        glm::vec3 position = light->getPosition();
        if (Vec3Control("Position", position)) {
          light->setPosition(position);
        }

        // Color control
        glm::vec3 color = light->getColor();
        if (ColorControl("Color", color)) {
          light->setColor(color);
        }

        // Intensity slider
        float intensity = light->getIntensity();
        if (ImGui::SliderFloat("Intensity", &intensity, 0.0f, 2.0f)) {
          light->setIntensity(intensity);
        }

        // Light properties
        float ambient = light->getAmbientStrength();
        if (ImGui::SliderFloat("Ambient", &ambient, 0.0f, 1.0f)) {
          light->setAmbientStrength(ambient);
        }

        float diffuse = light->getDiffuseStrength();
        if (ImGui::SliderFloat("Diffuse", &diffuse, 0.0f, 1.0f)) {
          light->setDiffuseStrength(diffuse);
        }

        float specular = light->getSpecularStrength();
        if (ImGui::SliderFloat("Specular", &specular, 0.0f, 1.0f)) {
          light->setSpecularStrength(specular);
        }

        float shininess = light->getShininess();
        if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 256.0f)) {
          light->setShininess(shininess);
        }

        // Delete button
        if (ImGui::Button("Delete") && lights.size() > 1) {
          lights.erase(lights.begin() + i);
          ImGui::TreePop();
          break;
        }

        ImGui::TreePop();
      }
    }

    // Lighting presets
    if (ImGui::TreeNode("Lighting Presets")) {
      if (ImGui::Button("Daylight")) {
        lights[0]->setColor(glm::vec3(1.0f, 0.95f, 0.8f));
        lights[0]->setIntensity(1.0f);
        lights[0]->setAmbientStrength(0.3f);
        lights[0]->setDiffuseStrength(0.7f);
        lights[0]->setSpecularStrength(0.3f);
      }
      ImGui::SameLine();
      if (ImGui::Button("Night")) {
        lights[0]->setColor(glm::vec3(0.1f, 0.1f, 0.3f));
        lights[0]->setIntensity(0.5f);
        lights[0]->setAmbientStrength(0.2f);
        lights[0]->setDiffuseStrength(0.3f);
        lights[0]->setSpecularStrength(0.5f);
      }
      ImGui::SameLine();
      if (ImGui::Button("Studio")) {
        lights[0]->setColor(glm::vec3(1.0f));
        lights[0]->setIntensity(1.0f);
        lights[0]->setAmbientStrength(0.3f);
        lights[0]->setDiffuseStrength(0.9f);
        lights[0]->setSpecularStrength(0.8f);
      }
      ImGui::TreePop();
    }
  }
}

void RenderObjectControls(Sphere &sphere, CubeSphere &cubeSphere) {
  if (ImGui::BeginTabBar("ObjectTabs")) {
    if (ImGui::BeginTabItem("Sphere")) {
      ImGui::Spacing();

      // Position control
      glm::vec3 position = sphere.getPosition();
      if (Vec3Control("Position", position)) {
        sphere.setPosition(position);
      }

      float radius = sphere.getRadius();
      if (ImGui::SliderFloat("Radius", &radius, 0.1F, 5.0F)) {
        sphere.setRadius(radius);
      }

      glm::vec3 scale = sphere.getScale();
      if (Vec3Control("Scale", scale, 1.0F, 0.1F, 0.1F, 10.0F)) {
        sphere.setScale(scale);
      }

      // Color control
      glm::vec3 color = sphere.getColor();
      if (ColorControl("Color", color)) {
        sphere.setColor(color);
      }

      // Additional sphere properties
      ImGui::Separator();
      ImGui::Text("Sphere Properties");

      // Resolution controls
      int sectors = sphere.getSectorCount();
      int stacks = sphere.getStackCount();
      bool resolutionChanged = false;

      resolutionChanged |= ImGui::SliderInt("Longitude Segments", &sectors, 4, 72);
      ImGui::SameLine();
      HelpMarker("Controls the number of vertical divisions (like longitude lines)");

      resolutionChanged |= ImGui::SliderInt("Latitude Segments", &stacks, 4, 36);
      ImGui::SameLine();
      HelpMarker("Controls the number of horizontal divisions (like latitude lines)");

      if (resolutionChanged) {
        sphere.setResolution(sectors, stacks);
      }

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("CubeSphere")) {
      ImGui::Spacing();

      // Position control
      if (glm::vec3 position = cubeSphere.getPosition(); Vec3Control("Position", position)) {
        cubeSphere.setPosition(position);
      }

      if (glm::vec3 scale = cubeSphere.getScale();
          Vec3Control("Scale", scale, 1.0F, 0.1F, 0.1F, 10.0F)) {
        cubeSphere.setScale(scale);
      }

      // Color control
      glm::vec3 color = cubeSphere.getColor();
      if (ColorControl("Color", color)) {
        cubeSphere.setColor(color);
      }

      // Additional CubeSphere properties
      ImGui::Separator();
      ImGui::Text("CubeSphere Properties");

      int resolution = cubeSphere.getResolution();
      if (ImGui::SliderInt("Subdivisions", &resolution, 2, 32)) {
        cubeSphere.setResolution(resolution);
      }
      ImGui::SameLine();
      HelpMarker("Controls mesh detail level. Higher values create smoother surfaces.");

      float size = cubeSphere.getSize();
      if (ImGui::SliderFloat("Size", &size, 0.1F, 2.0F)) {
        cubeSphere.setSize(size);
      }
      ImGui::SameLine();
      HelpMarker("Adjusts the overall size of the cube sphere.");

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Add Object")) {
      ImGui::Spacing();
      ImGui::Text("Create a new object:");

      static int newObjectType = 0;
      static char objectName[64] = "New Object";
      static float objectSize = 1.0F;
      static int objectResolution = 16;
      static glm::vec3 objectColor(1.0F);
      static glm::vec3 objectPosition(0.0F);

      const char *objectTypes[] = {"Sphere", "Cube Sphere"};
      ImGui::Combo("Object Type", &newObjectType, objectTypes, IM_ARRAYSIZE(objectTypes));

      ImGui::InputText("Name", objectName, IM_ARRAYSIZE(objectName));

      Vec3Control("Position", objectPosition, 0.0F, 0.1F, -10.0F, 10.0F);
      ImGui::SliderFloat("Size", &objectSize, 0.1F, 5.0F);

      if (newObjectType == 0) { // Sphere
        ImGui::SliderInt("Resolution", &objectResolution, 4, 72);
        HelpMarker("Controls the number of segments in the sphere mesh");
      } else { // Cube Sphere
        ImGui::SliderInt("Subdivisions", &objectResolution, 2, 32);
        HelpMarker("Controls the subdivision level of the cube sphere");
      }

      ImGui::ColorEdit3("Color", &objectColor[0]);

      if (ImGui::Button("Create Object", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
        CreateNewObject(objectName, newObjectType == 0 ? "Sphere" : "CubeSphere", objectPosition,
                        objectColor, objectSize, objectResolution);

        // Reset form
        strcpy(objectName, "New Object");
        objectSize = 1.0F;
        objectResolution = 16;
        objectColor = glm::vec3(1.0F);
        objectPosition = glm::vec3(0.0F);
      }

      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }
}

void RenderObjectList() {
  ImGui::Begin("Scene Objects");

  auto &objects = getScene().getObjects();

  // Object List
  ImGui::BeginChild("ObjectList", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
  for (size_t i = 0; i < objects.size(); i++) {
    auto &obj = objects[i];
    char label[128];
    snprintf(label, sizeof(label), "%s##%zu", obj.name.c_str(), i);

    if (ImGui::Selectable(label, obj.selected)) {
      // Deselect all other objects
      for (auto &other : objects) {
        other.selected = false;
      }
      obj.selected = true;
    }

    if (ImGui::BeginPopupContextItem()) {
      if (ImGui::MenuItem("Delete")) {
        getScene().removeObject(i);
      }
      if (ImGui::MenuItem("Rename")) {
        // TODO: Implement rename functionality
      }
      ImGui::EndPopup();
    }
  }
  ImGui::EndChild();

  ImGui::End();
}

void RenderGui(const FpsCounter &fpsCounter, const std::shared_ptr<Sphere> &sphere,
               const std::shared_ptr<CubeSphere> &cubeSphere, Renderer &renderer) {
  static bool sceneUpdated = false;

  // Setup ImGui style
  SetupStyle();

  // Render main menu bar
  MainMenuBar(fpsCounter);

  // Performance window
  PerformanceWindow(fpsCounter);

  // Object List window
  RenderObjectList();

  // Main control panel
  if (guiState.showControlPanel) {
    ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(guiState.mainPanelWidth, 600), ImGuiCond_FirstUseEver);

    ImGui::Begin("Control Panel", &guiState.showControlPanel);

    if (ImGui::Button(guiState.currentTab == 0 ? "Scene Settings##active" : "Scene Settings")) {
      guiState.currentTab = 0;
    }
    ImGui::SameLine();
    if (ImGui::Button(guiState.currentTab == 1 ? "Object Properties##active"
                                               : "Object Properties")) {
      guiState.currentTab = 1;
    }

    ImGui::Separator();

    if (guiState.currentTab == 0) {
      RenderSceneControls(renderer, sceneUpdated);
      RenderLightingControls(renderer.getLights());
    } else {
      RenderObjectControls(*sphere, *cubeSphere);
    }

    ImGui::End();
  }
}

} // namespace gui
