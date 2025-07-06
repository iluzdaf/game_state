#pragma once

#include <memory>
#include "game/states/state_stack.hpp"
#include "rendering/ui/imgui_manager.hpp"

struct GLFWwindow;

class Game
{
public:
    Game();
    ~Game();
    void run();
    std::unique_ptr<GameState> makeOptionsState();
    void setFullscreen(bool fullscreen);
    StateStack &getStateStack();

protected:
    void initGLFW(int windowWidth, int windowHeight);
    void initGlad();
    void update(float deltaTime);
    void render();
    void resize(int width, int height);

    GLFWwindow *window;
    StateStack stateStack;
    std::unique_ptr<ImGuiManager> imGuiManager;
};