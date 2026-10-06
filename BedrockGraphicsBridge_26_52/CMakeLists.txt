cmake_minimum_required(VERSION 3.25)
project(BedrockGraphicsBridge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.2b
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG v1.3.4
)
FetchContent_MakeAvailable(minhook)

add_library(imgui_core STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp
)

target_include_directories(imgui_core PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_core PUBLIC d3d12 dxgi)

add_library(BedrockGraphicsBridge SHARED
    src/dllmain.cpp
    src/state.cpp
    src/state.h
    src/material_bridge.cpp
    src/material_bridge.h
    src/overlay.cpp
    src/overlay.h
    src/render_hook.cpp
    src/render_hook.h
)

target_include_directories(BedrockGraphicsBridge PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${minhook_SOURCE_DIR}/include
    src
)

target_link_libraries(BedrockGraphicsBridge PRIVATE
    imgui_core
    minhook
    d3d12
    dxgi
    user32
    gdi32
    psapi
)

target_compile_definitions(BedrockGraphicsBridge PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    BGB_BUILD=1
)

set_target_properties(BedrockGraphicsBridge PROPERTIES
    OUTPUT_NAME "BedrockGraphicsBridge"
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
