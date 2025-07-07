#pragma once
#include <memory>
#include <rendering/texture2d.hpp>
#include "game/states/game_state.hpp"
#include "game/states/play_state.hpp"

class SplashState : public GameState
{
public:
    SplashState(Game &game, float duration)
        : GameState(game),
          duration(duration)
    {
    }

    void onEnter() override
    {
        splashTexture = std::make_unique<Texture2D>("../../assets/textures/man_on_a_beach_logo.jpg");
    }

    void onExit() override
    {
        splashTexture.reset();
    }

    void update(float deltaTime) override
    {
        timer += deltaTime;
        if (timer >= duration)
            game->getStateStack().replace(std::make_unique<PlayState>(*game));
    }

    void render() override
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);

        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus;

        ImGui::Begin("Splash", nullptr, window_flags);

        ImVec2 image_size(400, 400);

        ImVec2 image_pos(
            viewport->Pos.x + (viewport->Size.x - image_size.x) * 0.5f,
            viewport->Pos.y + (viewport->Size.y - image_size.y) * 0.5f);

        ImGui::GetWindowDrawList()->AddImage(
            (ImTextureID)(intptr_t)splashTexture->getTextureID(),
            image_pos,
            ImVec2(image_pos.x + image_size.x,
                   image_pos.y + image_size.y));

        const char *text = "Man on a beach presents";
        ImVec2 text_size = ImGui::CalcTextSize(text);

        float text_x = (viewport->Size.x - text_size.x) * 0.5f;
        float text_y = viewport->Size.y - text_size.y - 50;

        ImGui::SetCursorPos(ImVec2(text_x, text_y));
        ImGui::Text("%s", text);

        ImGui::End();
    }

private:
    float duration,
        timer = 0.0f;
    std::unique_ptr<Texture2D> splashTexture;
};