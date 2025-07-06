#pragma once
#include <signals.hpp>
#include "game/states/game_state.hpp"


class OptionsState : public GameState
{
public:
    OptionsState(Game &game)
        : GameState(game)
    {
    }

    void update(float dt) override
    {
        if (!open)
            game->getStateStack().pop();

        if (fullscreen != wasFullscreen)
        {
            wasFullscreen = fullscreen;
            onFullscreenToggled(fullscreen);
        }
    }

    void render() override
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();

        ImVec2 window_size(200, 200);
        ImVec2 window_pos(
            viewport->Pos.x + (viewport->Size.x - window_size.x) * 0.5f + 200,
            viewport->Pos.y + (viewport->Size.y - window_size.y) * 0.5f);

        ImGui::SetNextWindowPos(window_pos);
        ImGui::SetNextWindowSize(window_size);

        ImGui::Begin(
            "Options", &open,
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImGui::Checkbox("Fullscreen", &fullscreen);

        ImGui::End();
    }

    fteng::signal<void(bool)> onFullscreenToggled;

private:
    bool fullscreen = false,
         wasFullscreen = false,
         open = true;
};