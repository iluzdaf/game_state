cmake_minimum_required(VERSION 3.31)
project(GameState)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/Zc:preprocessor)
endif()

# GLAD
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# STB
add_library(stb STATIC external/stb/stb_image.cpp)
target_include_directories(stb PUBLIC external/stb)

# GLFW
add_subdirectory(external/glfw)

# GLM (Header-only)
include_directories(external/glm)

# TheWisp/signals (Header-only)
include_directories(external/signals)

# Catch2 (for tests only)
add_subdirectory(external/catch2)

# IMGUI
file(GLOB IMGUI_SRC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
    external/imgui
    external/imgui/backends
)
target_link_libraries(imgui PRIVATE glfw glad)

add_executable(gamestate
    src/main.cpp
    src/rendering/texture2D.cpp
    src/rendering/ui/imgui_manager.cpp
    src/game/game.cpp
    src/game/states/state_stack.cpp
)

target_include_directories(gamestate
    PRIVATE
    include
    external/glad/include
    external/glfw/include
)

target_link_libraries(gamestate
    PRIVATE
    glad
    glfw
    stb
    imgui
    ${CMAKE_DL_LIBS}
)

add_executable(gamestate_tests
    tests/test_state_stack.cpp
    src/rendering/texture2D.cpp
    src/rendering/ui/imgui_manager.cpp
    src/game/game.cpp
    src/game/states/state_stack.cpp
)

target_include_directories(gamestate_tests
    PRIVATE
    include
    external/glad/include
    external/glfw/include
    tests/
)

target_link_libraries(gamestate_tests
    PRIVATE
    Catch2::Catch2WithMain
    glad
    glfw
    stb
    imgui
    ${CMAKE_DL_LIBS}
)

enable_testing()
add_test(NAME AllTests COMMAND gamestate_tests)
