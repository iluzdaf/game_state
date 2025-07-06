#pragma once
#include "game/states/game_state.hpp"
#include "game/states/options_state.hpp"

class PlayState : public GameState
{
public:
    PlayState(Game &game)
        : GameState(game)
    {
    }

    void onPause() override
    {
        paused = true;
    }

    void onResume() override
    {
        paused = false;
    }

    void update(float dt) override
    {
        if (transition)
        {
            game->getStateStack().push(game->makeOptionsState());
            transition = false;
        }

        if (addScore)
        {
            score += 100;
            addScore = false;
        }
    }

    void render() override
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();

        ImVec2 window_size(200, 200);
        ImVec2 window_pos(
            viewport->Pos.x + (viewport->Size.x - window_size.x) * 0.5f,
            viewport->Pos.y + (viewport->Size.y - window_size.y) * 0.5f);

        ImGui::SetNextWindowPos(window_pos);
        ImGui::SetNextWindowSize(window_size);

        ImGui::Begin(
            "Play", nullptr,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImGui::Text("One button adventure!");

        ImGui::Text("Score: %d", score);

        ImGui::BeginDisabled(paused);

        if (ImGui::Button("Press Me!"))
            addScore = true;

        if (ImGui::Button("Options"))
            transition = true;

        ImGui::EndDisabled();

        ImGui::End();
    }

private:
    int score = 0;
    bool transition = false,
         addScore = false,
         paused = false;
};