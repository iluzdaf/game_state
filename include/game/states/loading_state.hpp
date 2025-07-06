#pragma once
#include "game/states/game_state.hpp"
#include "game/states/splash_state.hpp"

class LoadingState : public GameState
{
public:
    LoadingState(Game &game)
        : GameState(game)
    {
        pickRandomQuote();
    }

    void update(float deltaTime) override
    {
        quoteChangeTimer += deltaTime;
        if (quoteChangeTimer >= quoteChangeDuration)
        {
            pickRandomQuote();
            quoteChangeTimer = 0.0f;
        }

        timer += deltaTime;
        if (timer >= duration)
            game->getStateStack().replace(std::make_unique<SplashState>(*game, 3.0f));
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

        ImGui::Begin("Loading", nullptr, window_flags);

        std::string text = "Loading ...";
        ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
        float text_x = (viewport->Size.x - text_size.x) * 0.5f;
        float text_y = (viewport->Size.y - text_size.y) * 0.4f;

        ImGui::SetCursorPos(ImVec2(text_x, text_y));
        ImGui::Text("%s", text.c_str());

        if (!currentQuote.empty())
        {
            ImVec2 quote_size = ImGui::CalcTextSize(currentQuote.c_str());
            float quote_x = (viewport->Size.x - quote_size.x) * 0.5f;
            float quote_y = text_y + text_size.y + 20.0f;

            ImGui::SetCursorPos(ImVec2(quote_x, quote_y));
            ImGui::Text("%s", currentQuote.c_str());
        }

        ImGui::End();
    }

private:
    float duration = 4.0f,
          timer = 0.0f,
          quoteChangeTimer = 0.0f,
          quoteChangeDuration = 2.0f;
    std::string currentQuote;
    std::vector<std::string> quotes = {
        "Finding the number of grains of sand on the beach.",
        "Digging a hole to the other side of the world.",
        "Counting how many crabs run sideways.",
        "Combing the beach for lost treasure.",
        "Checking if the tide is coming in... or out.",
        "Polishing seashells for maximum shine.",
        "Raking zen patterns into the sand.",
        "Tracing footsteps before the waves wash them away.",
        "Calculating how many coconuts a palm tree can hold.",
        "Wondering why seagulls scream so much."};

    void pickRandomQuote()
    {
        if (!quotes.empty())
        {
            int index = rand() % quotes.size();
            currentQuote = quotes[index];
        }
    }
};