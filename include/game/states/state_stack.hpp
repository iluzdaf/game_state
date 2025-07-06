#pragma once

#include <memory>
#include <vector>
#include "game/states/game_state.hpp"

class StateStack
{
public:
    void push(std::unique_ptr<GameState> state);
    void pop();
    void replace(std::unique_ptr<GameState> state);
    void update(float deltaTime);
    void render();
    bool isEmpty() const;
    size_t size() const;
    GameState &top() const;

private:
    std::vector<std::unique_ptr<GameState>> stack;
};