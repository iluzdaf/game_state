#pragma once

class Game;

class GameState
{
public:
    GameState() = default;
    explicit GameState(Game &game) : game(&game) {}
    virtual ~GameState() = default;
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onPause() {}
    virtual void onResume() {}
    virtual void update(float dt) {}
    virtual void render() {}

protected:
    Game *game = nullptr;
};