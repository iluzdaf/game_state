#include <stdexcept>
#include "game/states/state_stack.hpp"

void StateStack::push(std::unique_ptr<GameState> state)
{
    if (!state)
        throw std::runtime_error("StateStack: push received nullptr GameState");

    if (!stack.empty())
        top().onPause();

    stack.push_back(std::move(state));

    top().onEnter();
}

void StateStack::pop()
{
    if (!stack.empty())
    {
        top().onExit();

        stack.pop_back();

        if (!stack.empty())
            top().onResume();
    }
}

void StateStack::replace(std::unique_ptr<GameState> state)
{
    if (!state)
        throw std::runtime_error("StateStack: replace received nullptr GameState");

    pop();

    push(std::move(state));
}

void StateStack::update(float deltaTime)
{
    if (!stack.empty())
        top().update(deltaTime);
}

void StateStack::render()
{
    for (auto &state : stack)
        state->render();
}

bool StateStack::isEmpty() const
{
    return stack.empty();
}

size_t StateStack::size() const
{
    return stack.size();
}

GameState &StateStack::top() const
{
    if (stack.empty())
        throw std::runtime_error("StateStack: top called on empty stack");

    return *stack.back();
}