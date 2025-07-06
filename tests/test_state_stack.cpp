#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "game/game.hpp"
#include "game/states/state_stack.hpp"

class TestGame : public Game
{
public:
    TestGame()
    {
        stateStack.pop();
    }
};

class DummyState : public GameState
{
public:
    bool *enteredFlag = nullptr, *exitedFlag = nullptr,
         *renderedFlag = nullptr, *updatedFlag = nullptr,
         *pausedFlag = nullptr, *resumedFlag = nullptr;

    int *onEnterCallCount = nullptr, *onExitCallCount = nullptr,
        *renderCallCount = nullptr, *updateCallCount = nullptr,
        *pauseCallCount = nullptr, *resumeCallCount = nullptr;

    DummyState(
        Game &game,
        bool *enteredFlag = nullptr, bool *exitedFlag = nullptr,
        bool *renderedFlag = nullptr, bool *updatedFlag = nullptr,
        bool *pausedFlag = nullptr, bool *resumedFlag = nullptr,
        int *onEnterCallCount = nullptr, int *onExitCallCount = nullptr,
        int *renderCallCount = nullptr, int *updateCallCount = nullptr,
        int *pauseCallCount = nullptr, int *resumeCallCount = nullptr)
        : GameState(game),
          enteredFlag(enteredFlag), exitedFlag(exitedFlag),
          renderedFlag(renderedFlag), updatedFlag(updatedFlag),
          pausedFlag(pausedFlag), resumedFlag(resumedFlag),
          onEnterCallCount(onEnterCallCount), onExitCallCount(onExitCallCount),
          renderCallCount(renderCallCount), updateCallCount(updateCallCount),
          pauseCallCount(pauseCallCount), resumeCallCount(resumeCallCount)
    {
    }

    void onEnter() override
    {
        if (onEnterCallCount)
            (*onEnterCallCount)++;
        if (enteredFlag)
            *enteredFlag = true;
    }

    void onExit() override
    {
        if (onExitCallCount)
            (*onExitCallCount)++;
        if (exitedFlag)
            *exitedFlag = true;
    }

    void onPause() override
    {
        if (pauseCallCount)
            (*pauseCallCount)++;
        if (pausedFlag)
            *pausedFlag = true;
    }

    void onResume() override
    {
        if (resumeCallCount)
            (*resumeCallCount)++;
        if (resumedFlag)
            *resumedFlag = true;
    }

    void render() override
    {
        if (renderCallCount)
            (*renderCallCount)++;
        if (renderedFlag)
            *renderedFlag = true;
    }

    void update(float) override
    {
        if (updateCallCount)
            (*updateCallCount)++;
        if (updatedFlag)
            *updatedFlag = true;
    }
};

TEST_CASE("StateStack push", "[StateStack]")
{
    StateStack stack;
    stack.push(std::make_unique<GameState>());
    REQUIRE(!stack.isEmpty());
    REQUIRE(stack.size() == 1);
    stack.push(std::make_unique<GameState>());
    REQUIRE(!stack.isEmpty());
    REQUIRE(stack.size() == 2);
    REQUIRE_THROWS_WITH(stack.push(nullptr), "StateStack: push received nullptr GameState");
}

TEST_CASE("StateStack pop", "[StateStack]")
{
    StateStack stack;
    stack.push(std::make_unique<GameState>());
    stack.pop();
    REQUIRE(stack.isEmpty());
    REQUIRE_NOTHROW(stack.pop());
}

TEST_CASE("StateStack replace", "[StateStack]")
{
    StateStack stack;
    stack.push(std::make_unique<GameState>());
    stack.replace(std::make_unique<GameState>());
    REQUIRE(stack.size() == 1);
    REQUIRE_THROWS_WITH(stack.replace(nullptr), "StateStack: replace received nullptr GameState");
}

TEST_CASE("StateStack calls onEnter once when pushing a state", "[StateStack]")
{
    TestGame game;
    bool enteredFlag = false;
    int onEnterCallCount = 0;
    auto state = std::make_unique<DummyState>(
        game,
        &enteredFlag, nullptr, nullptr, nullptr, nullptr, nullptr,
        &onEnterCallCount);
    game.getStateStack().push(std::move(state));
    REQUIRE(enteredFlag);
    REQUIRE(onEnterCallCount == 1);
}

TEST_CASE("StateStack calls onPause once on top state when pushing a state", "[StateStack]")
{
    TestGame game;
    bool pausedFlag = false;
    int pauseCallCount = 0;
    auto state = std::make_unique<DummyState>(
        game,
        nullptr, nullptr, nullptr, nullptr, &pausedFlag, nullptr,
        nullptr, nullptr, nullptr, nullptr, &pauseCallCount);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(state));
    stack.push(std::make_unique<GameState>());
    REQUIRE(pausedFlag);
    REQUIRE(pauseCallCount == 1);
}

TEST_CASE("StateStack calls onExit once when popping a state", "[StateStack]")
{
    TestGame game;
    bool exitedFlag = false;
    int onExitCallCount = 0;
    auto state = std::make_unique<DummyState>(
        game,
        nullptr, &exitedFlag, nullptr, nullptr, nullptr, nullptr,
        nullptr, &onExitCallCount);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(state));
    stack.pop();
    REQUIRE(exitedFlag);
    REQUIRE(onExitCallCount == 1);
}

TEST_CASE("StateStack calls onResume once on top state when popping a state", "[StateStack]")
{
    TestGame game;
    bool resumedFlag = false;
    int resumeCallCount = 0;
    auto state = std::make_unique<DummyState>(
        game,
        nullptr, nullptr, nullptr, nullptr, nullptr, &resumedFlag,
        nullptr, nullptr, nullptr, nullptr, nullptr, &resumeCallCount);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(state));
    stack.push(std::make_unique<GameState>());
    stack.pop();
    REQUIRE(resumedFlag);
    REQUIRE(resumeCallCount == 1);
}

TEST_CASE("StateStack calls onExit on top state when replacing a state", "[StateStack]")
{
    TestGame game;
    bool exitedFlag = false;
    int onExitCallCount = 0;
    auto state = std::make_unique<DummyState>(
        game,
        nullptr, &exitedFlag, nullptr, nullptr, nullptr, nullptr,
        nullptr, &onExitCallCount);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(state));
    stack.replace(std::make_unique<GameState>());
    REQUIRE(exitedFlag);
    REQUIRE(onExitCallCount == 1);
}

TEST_CASE("StateStack calls onEnter on new state when replacing a state", "[StateStack]")
{
    TestGame game;
    bool enteredFlag = false;
    int onEnterCallCount = 0;
    auto state = std::make_unique<DummyState>(
        game,
        &enteredFlag, nullptr, nullptr, nullptr, nullptr, nullptr,
        &onEnterCallCount);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(state));
    stack.replace(std::make_unique<GameState>());
    REQUIRE(enteredFlag);
    REQUIRE(onEnterCallCount == 1);
}

class ReplaceRequestingState : public DummyState
{
public:
    bool *replacedEnteredFlag = nullptr, *replacedExitedFlag = nullptr,
         *replacedRenderedFlag = nullptr, *replacedUpdatedFlag = nullptr,
         *replacedPausedFlag = nullptr, *replacedResumedFlag = nullptr;
    int *replacedOnEnterCallCount = nullptr, *replacedOnExitCallCount = nullptr,
        *replacedRenderCallCount = nullptr, *replacedUpdateCallCount = nullptr,
        *replacedPauseCallCount = nullptr, *replacedResumeCallCount = nullptr;

    ReplaceRequestingState(
        Game &game,
        bool *enteredFlag = nullptr, bool *exitedFlag = nullptr,
        bool *renderedFlag = nullptr, bool *updatedFlag = nullptr,
        bool *pausedFlag = nullptr, bool *resumedFlag = nullptr,
        int *onEnterCallCount = nullptr, int *onExitCallCount = nullptr,
        int *renderCallCount = nullptr, int *updateCallCount = nullptr,
        int *pauseCallCount = nullptr, int *resumeCallCount = nullptr,
        bool *replacedEnteredFlag = nullptr, bool *replacedExitedFlag = nullptr,
        bool *replacedRenderedFlag = nullptr, bool *replacedUpdatedFlag = nullptr,
        bool *replacedPausedFlag = nullptr, bool *replacedResumedFlag = nullptr,
        int *replacedOnEnterCallCount = nullptr, int *replacedOnExitCallCount = nullptr,
        int *replacedRenderCallCount = nullptr, int *replacedUpdateCallCount = nullptr,
        int *replacedPauseCallCount = nullptr, int *replacedResumeCallCount = nullptr)
        : DummyState(
              game,
              enteredFlag, exitedFlag,
              renderedFlag, updatedFlag,
              pausedFlag, resumedFlag,
              onEnterCallCount, onExitCallCount,
              renderCallCount, updateCallCount,
              pauseCallCount, resumeCallCount),
          replacedEnteredFlag(replacedEnteredFlag), replacedExitedFlag(replacedExitedFlag),
          replacedRenderedFlag(replacedRenderedFlag), replacedUpdatedFlag(replacedUpdatedFlag),
          replacedPausedFlag(replacedPausedFlag), replacedResumedFlag(replacedResumedFlag),
          replacedOnEnterCallCount(replacedOnEnterCallCount), replacedOnExitCallCount(replacedOnExitCallCount),
          replacedRenderCallCount(replacedRenderCallCount), replacedUpdateCallCount(replacedUpdateCallCount),
          replacedPauseCallCount(replacedPauseCallCount), replacedResumeCallCount(replacedResumeCallCount)
    {
    }

    void update(float deltaTime) override
    {
        DummyState::update(deltaTime);

        game->getStateStack().replace(
            std::make_unique<DummyState>(
                *game,
                replacedEnteredFlag, replacedExitedFlag,
                replacedRenderedFlag, replacedUpdatedFlag,
                replacedPausedFlag, replacedResumedFlag,
                replacedOnEnterCallCount, replacedOnExitCallCount,
                replacedRenderCallCount, replacedUpdateCallCount,
                replacedPauseCallCount, replacedResumeCallCount));
    }
};

TEST_CASE("StateStack replace transition pops old state and pushes new state", "[StateStack]")
{
    TestGame game;
    bool firstExited = false,
         secondEntered = false;
    auto firstState = std::make_unique<ReplaceRequestingState>(
        game,
        nullptr, &firstExited, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        &secondEntered);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(firstState));
    stack.update(0.01f);
    REQUIRE(stack.size() == 1);
    REQUIRE(firstExited);
    REQUIRE(secondEntered);
}

class PopRequestingState : public DummyState
{
public:
    PopRequestingState(
        Game &game,
        bool *enteredFlag = nullptr, bool *exitedFlag = nullptr,
        bool *renderedFlag = nullptr, bool *updatedFlag = nullptr,
        bool *pausedFlag = nullptr, bool *resumedFlag = nullptr,
        int *onEnterCallCount = nullptr, int *onExitCallCount = nullptr,
        int *renderCallCount = nullptr, int *updateCallCount = nullptr,
        int *pauseCallCount = nullptr, int *resumeCallCount = nullptr)
        : DummyState(
              game,
              enteredFlag, exitedFlag,
              renderedFlag, updatedFlag,
              pausedFlag, resumedFlag,
              onEnterCallCount, onExitCallCount,
              renderCallCount, updateCallCount,
              pauseCallCount, resumeCallCount)
    {
    }

    void update(float deltaTime) override
    {
        DummyState::update(deltaTime);

        game->getStateStack().pop();
    }
};

TEST_CASE("StateStack pop transition pops the current state", "[StateStack]")
{
    TestGame game;
    bool exitedFlag = false;
    auto state = std::make_unique<PopRequestingState>(
        game, nullptr, &exitedFlag);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(state));
    stack.update(0.01f);
    REQUIRE(stack.isEmpty());
    REQUIRE(exitedFlag);
}

TEST_CASE("StateStack pop transition resumes the previous state", "[StateStack]")
{
    TestGame game;
    bool resumedFlag = false;
    int resumeCallCount = 0;
    auto firstState = std::make_unique<DummyState>(
        game,
        nullptr, nullptr, nullptr, nullptr, nullptr, &resumedFlag,
        nullptr, nullptr, nullptr, nullptr, nullptr, &resumeCallCount);
    auto secondState = std::make_unique<PopRequestingState>(game);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(firstState));
    stack.push(std::move(secondState));
    stack.update(0.01f);
    REQUIRE(resumedFlag);
    REQUIRE(resumeCallCount == 1);
}

class PushRequestingState : public DummyState
{
public:
    bool *pushedEnteredFlag = nullptr, *pushedExitedFlag = nullptr,
         *pushedRenderedFlag = nullptr, *pushedUpdatedFlag = nullptr,
         *pushedPausedFlag = nullptr, *pushedResumedFlag = nullptr;
    int *pushedOnEnterCallCount = nullptr, *pushedOnExitCallCount = nullptr,
        *pushedRenderCallCount = nullptr, *pushedUpdateCallCount = nullptr,
        *pushedPauseCallCount = nullptr, *pushedResumeCallCount = nullptr;

    PushRequestingState(
        Game &game,
        bool *enteredFlag = nullptr, bool *exitedFlag = nullptr,
        bool *renderedFlag = nullptr, bool *updatedFlag = nullptr,
        bool *pausedFlag = nullptr, bool *resumedFlag = nullptr,
        int *onEnterCallCount = nullptr, int *onExitCallCount = nullptr,
        int *renderCallCount = nullptr, int *updateCallCount = nullptr,
        int *pauseCallCount = nullptr, int *resumeCallCount = nullptr,
        bool *pushedEnteredFlag = nullptr, bool *pushedExitedFlag = nullptr,
        bool *pushedRenderedFlag = nullptr, bool *pushedUpdatedFlag = nullptr,
        bool *pushedPausedFlag = nullptr, bool *pushedResumedFlag = nullptr,
        int *pushedOnEnterCallCount = nullptr, int *pushedOnExitCallCount = nullptr,
        int *pushedRenderCallCount = nullptr, int *pushedUpdateCallCount = nullptr,
        int *pushedPauseCallCount = nullptr, int *pushedResumeCallCount = nullptr)
        : DummyState(
              game,
              enteredFlag, exitedFlag,
              renderedFlag, updatedFlag,
              pausedFlag, resumedFlag,
              onEnterCallCount, onExitCallCount,
              renderCallCount, updateCallCount,
              pauseCallCount, resumeCallCount),
          pushedEnteredFlag(pushedEnteredFlag), pushedExitedFlag(pushedExitedFlag),
          pushedRenderedFlag(pushedRenderedFlag), pushedUpdatedFlag(pushedUpdatedFlag),
          pushedPausedFlag(pushedPausedFlag), pushedResumedFlag(pushedResumedFlag),
          pushedOnEnterCallCount(pushedOnEnterCallCount), pushedOnExitCallCount(pushedOnExitCallCount),
          pushedRenderCallCount(pushedRenderCallCount), pushedUpdateCallCount(pushedUpdateCallCount),
          pushedPauseCallCount(pushedPauseCallCount), pushedResumeCallCount(pushedResumeCallCount)
    {
    }

    void update(float deltaTime) override
    {
        DummyState::update(deltaTime);

        game->getStateStack().push(std::make_unique<DummyState>(
            *game,
            pushedEnteredFlag, pushedExitedFlag,
            pushedRenderedFlag, pushedUpdatedFlag,
            pushedPausedFlag, pushedResumedFlag,
            pushedOnEnterCallCount, pushedOnExitCallCount,
            pushedRenderCallCount, pushedUpdateCallCount,
            pushedPauseCallCount, pushedResumeCallCount));
    }
};

TEST_CASE("StateStack push transition pushes a new state", "[StateStack]")
{
    TestGame game;
    bool firstEntered = false, firstExited = false, firstPaused = false,
         secondEntered = false;
    auto firstState = std::make_unique<PushRequestingState>(
        game,
        &firstEntered, &firstExited, nullptr, nullptr, &firstPaused, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        &secondEntered);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(firstState));
    stack.update(0.01f);
    REQUIRE(stack.size() == 2);
    REQUIRE(firstEntered);
    REQUIRE_FALSE(firstExited);
    REQUIRE(firstPaused);
    REQUIRE(secondEntered);
}

TEST_CASE("StateStack renders all states", "[StateStack]")
{
    TestGame game;
    bool renderedFlag1 = false;
    bool renderedFlag2 = false;
    auto state1 = std::make_unique<DummyState>(
        game,
        nullptr, nullptr, &renderedFlag1);
    auto state2 = std::make_unique<DummyState>(
        game,
        nullptr, nullptr, &renderedFlag2);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(state1));
    stack.push(std::move(state2));
    stack.render();
    REQUIRE(renderedFlag1);
    REQUIRE(renderedFlag2);
}

TEST_CASE("StateStack updates the top state", "[StateStack]")
{
    TestGame game;
    bool updatedFlag1 = false;
    bool updatedFlag2 = false;
    auto state1 = std::make_unique<DummyState>(
        game,
        nullptr, nullptr, nullptr, &updatedFlag1);
    auto state2 = std::make_unique<DummyState>(
        game,
        nullptr, nullptr, nullptr, &updatedFlag2);
    StateStack &stack = game.getStateStack();
    stack.push(std::move(state1));
    stack.push(std::move(state2));
    stack.update(0.01f);
    REQUIRE(updatedFlag2);
    REQUIRE_FALSE(updatedFlag1);
}