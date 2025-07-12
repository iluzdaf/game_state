---
marp: true
theme: gaia
class: invert
paginate: true
---

# DX1212 - Game Play Progamming

---

## Welcome to the Game State Practical

You'll learn how to implement a flexible game state system using modern C++, design patterns and test-driven development principles.

---

## Why Do We Use a `GameState` System?

Why not just use a big `switch` statement to manage game states?

```cpp
switch (gameState)
{
    case PLAY:
        // run game logic
        break;
    case OPTIONS:
        // render options menu
        break;
}
```

---

However, this approach quickly becomes difficult to manage, because:

1. Handling overlays becomes messy
2. Resource management is harder
3. Encapsulation improves maintainability
4. It supports flexible transitions

<!-- _notes: 
1. Handling overlays becomes messy
  For example, when an options menu appears over the game:
    - you want to keep rendering the game behind it
    - but pause the game’s updates and input
    - managing this logic with switch statements becomes complex and error-prone

2. Resource management is harder
  - Certain states need specific assets (textures, sounds, data).
  - A state system ensures that:
    - resources load only when needed
    - and can be cleaned up when the state exits

3. Encapsulation improves maintainability
  - Each game state becomes its own class.
  - You avoid giant blocks of if or switch logic.
  - It’s easier to isolate bugs and extend the game later.

4. It supports flexible transitions
  For example:
  - replacing one state entirely (like loading → splash)
  - overlaying a new state (like options) on top of an existing state
  - All these transitions become much simpler with a `StateStack`.
-->
---

## Project Architecture Overview

This project consists of:

- `StateStack`: manages the stack of active game states
- `GameState`: base class for all states
  - `SplashState`: displays a splash screen
  - `LoadingState`: simulates a loading screen with rotating quotes
  - `PlayState`: gameplay screen with score tracking
  - `OptionsState`: overlay UI to toggle game options

<!-- _notes: 
All states inherit from `GameState`. 
The `StateStack` handles transitions.
-->

---

```cpp
class GameState
{
public:
    GameState() = default;
    virtual ~GameState() = default;
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void onPause() {}
    virtual void onResume() {}
    virtual void update(float dt) {}
    virtual void render() {}
};
```

---

```cpp
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
```

---

Here's the intended flow of the game:

```markdown
┌─────────┐
│ Loading │
└─────────┘
      │
      ▼
┌────────┐
│ Splash │
└────────┘
      │
      ▼
┌────────┐
│  Play  │
└────────┘
    ▲  │
    │  ▼
┌─────────┐
│ Options │ 
└─────────┘
```

<!-- _notes: 
Options is overlayed on top of Play
-->

---

## Practical Exercise Instructions

Your goal is to modify the StateStack so that it behaves like a proper stack of states.

- `StateStack` currently only supports changing to a single state
- `StateStack` already handles calling `render()` and `update()` for all states
  - Only the **top state** is updated each frame
  - **All states are rendered** each frame
- The concrete `GameStates` are already implemented

<!-- _notes: 
- When you push a new state, it replaces the current state instead of layering on top.
- There’s no support for popping states, overlays, or proper transitions.

- This ensures that lower states (e.g. Play) don’t receive logic updates if a state like Options is active on top.
- This allows visual overlays—for example, `OptionsState` can render on top while still showing the `PlayState` underneath.
-->

---

<div style="display: flex; gap: 10px;">
  <img src="https://media.giphy.com/media/FweEi5KvATPgeBRxTu/giphy.gif" width="550"/>
  <img src="https://media.giphy.com/media/geEujqfCVFwmCVItus/giphy.gif" width="550"/>
</div>

---

Run gamestate_tests.

- Fail

  ```markdown
  ./game_state/tests/test_state_stack.cpp:334: FAILED:
    REQUIRE_THROWS_WITH( stack.update(0.01f), "StateStack: push transition requested but state factory returned nullptr" )
  because no exception was thrown where one was expected:

  ===============================================================================
  test cases: 15 | 3 passed | 12 failed
  assertions: 21 | 9 passed | 12 failed
  ```

- Pass

  ```markdown
  Randomness seeded to: 4068396096
  ===============================================================================
  All tests passed (35 assertions in 15 test cases)
  ```

<!-- _notes: 
- Read through the failing tests.
- The tests are already written to express the correct behaviour of the `StateStack`.
-->

---

Update your `StateStack` class to make the tests pass:

- `push()` - Add a new state on top of the stack
- `pop()` - Remove the current top state revealing the state beneath
- `replace()` - Replace the current state with a new one
- Proper calling of lifecycle methods:
  - `onEnter()` — called when a state becomes active
  - `onExit()` — called when a state is removed from the stack
  - `onPause()` — called on the top state when a new state is pushed
  - `onResume()` — called when a state underneath is uncovered

---

## ❓ Questions?

I’d love to hear how the class went or how I can improve.

Feel free to email me at:  
<muhammad_fadzuli_mohamad_said@myaccount.nyp.edu.sg>

📂 All course materials are available at:  
[www.github.com/iluzdaf/game_state](https://www.github.com/iluzdaf/game_state)
