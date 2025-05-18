cmake_minimum_required(VERSION 4.00)
project(OrbitalSimulation LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 REQUIRED)

# Glad library setup
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# ImGui sources
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp
)

# ImGui backend sources for SDL3 + OpenGL3
set(IMGUI_BACKENDS_SOURCES
    external/imgui/backends/imgui_impl_sdl3.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)

# Executable
add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    ${IMGUI_SOURCES}
    ${IMGUI_BACKENDS_SOURCES}
    src/main.cpp
    src/core/window.cpp
    src/core/fps_counter.cpp
    src/Graphics/shader.cpp
    src/Graphics/renderer.cpp
    src/GUI/gui.cpp
    src/GUI/Scene.cpp
    src/Graphics/core/RenderVisitor.cpp
    src/Graphics/bodies/sphere.cpp
    src/Graphics/bodies/cubeSphere.cpp
    src/Graphics/lighting/Light.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/imgui/backends
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    glad
)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework OpenGL"
    )
endif()
