# 🎮 Game State Practical

Welcome to the **Game State Practical**!

You'll learn how to implement a flexible game state system using modern C++, design patterns and test-driven development principles.

## Why Do We Use a `GameState` System?

You might wonder: why not just use a big `switch` statement to manage game states?

Technically, you can write code like this:

```cpp
switch (gameState)
{
    case SPLASH:
        // render splash screen
        break;
    case PLAY:
        // run game logic
        break;
    case OPTIONS:
        // render options menu
        break;
}
```

However, this approach quickly becomes difficult to manage, because:

1. Handling overlays becomes messy. For example, when an options menu appears over the game:
    - you want to keep rendering the game behind it
    - but pause the game’s updates and input
    - managing this logic with switch statements becomes complex and error-prone

2. Resource management is harder.
    - Certain states need specific assets (textures, sounds, data).
    - A state system ensures that:
      - resources load only when needed
      - and can be cleaned up when the state exits

3. Encapsulation improves maintainability.
    - Each game state becomes its own class.
    - You avoid giant blocks of if or switch logic.
    - It’s easier to isolate bugs and extend the game later.

4. It supports flexible transitions. For example:
    - replacing one state entirely (like loading → splash)
    - overlaying a new state (like options) on top of an existing state
    - All these transitions become much simpler with a `StateStack`.

This is why most modern games use a `StateStack` architecture rather than large switch statements. It keeps your game organized, flexible and maintainable as it grows.

## Learning Objectives

By the end of this practical, you should be able to:

- ✅ Understand the purpose of `GameState` and a `StateStack`
- ✅ Handle overlays (like options menus that pause game updates beneath)

## Project Architecture Overview

This project consists of:

- `StateStack`: manages the stack of active game states
- `GameState`: base class for all individual states
- `SplashState`: displays a splash screen
- `LoadingState`: simulates a loading screen with rotating quotes
- `PlayState`: gameplay screen with score tracking
- `OptionsState`: overlay UI to toggle game options

All states inherit from `GameState`. The `StateStack` handles transitions. Here's the intended flow of the game:

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
│ Options │ (Overlay on top of Play)
└─────────┘
```

## Setup Instructions

1. Install dependencies if needed:
    - CMake — for building the project
    - git-lfs — for downloading large assets stored in Git LFS

    For macOS and Linux, use your package manager of choice.

    For Windows, make sure CMake and Git for Windows are installed in Visual Studio via:

    ```markdown
    Tools and Features -> Individual Components
    ```

2. Clone the repository:

    For macOS and Linux,

    ```bash
    git clone https://github.com/iluzdaf/game_state.git
    git submodule update --init --recursive
    ```

    For Windows, use the Visual Studio UI or your favourite git client.

3. Build the project:

    For macOS and Linux,

    ```bash
    mkdir build
    cd build

    # Debug build

    mkdir debug
    cd debug
    cmake ../.. -DCMAKE_BUILD_TYPE=Debug
    cmake --build .
    cd ..

    # Release build

    mkdir release
    cd release
    cmake ../.. -DCMAKE_BUILD_TYPE=Release
    cmake --build .
    ```

    For Windows,
    - once you open the repository folder using Visual Studio, it will automatically detect the CMake settings and setup the project folder accordingly.
    - You can choose **Debug** or **Release** in the dropdown near the Run button.

4. Run tests:

    For macOS and Linux,

    ```bash
    ./gamestate_tests
    ```

    For Windows, choose the project in the Visual Studio **Startup Project** selector near the run button and run it.

5. Run the game executable:

    For macOS and Linux,

    ```bash
    ./gamestate 
    ```

    For Windows, choose the project in the Visual Studio Project selector near the run button and run it.

## Practical Exercise Instructions

In this exercise, you’ll be implementing a flexible state management system by extending a minimal StateStack class.

### Your Starting Point

- The provided code contains a stripped-down `StateStack`, which currently only supports pushing a single state.
  - When you push a new state, it replaces the current state instead of layering on top.
  - There’s no support for popping states, overlays, or proper transitions.
- `StateStack` already handles calling `render()` and `update()` for all states.
  - Only the **top state** is updated each frame. This ensures that lower states (e.g. Play) don’t receive logic updates if a state like Options is active on top.
  - **All states are rendered** each frame. This allows visual overlays—for example, `OptionsState` can render on top while still showing the `PlayState` underneath.
- The concrete `GameStates` are already implemented.

![Starting Point](https://media.giphy.com/media/FweEi5KvATPgeBRxTu/giphy.gif)

### What You Need to Do

Your goal is to modify the StateStack so that it behaves like a proper stack of states.

![Goal](https://media.giphy.com/media/geEujqfCVFwmCVItus/giphy.gif)

Follow these steps:

#### Step 1 — Review the Tests

- Run the tests.

```markdown
./game_state/tests/test_state_stack.cpp:334: FAILED:
  REQUIRE_THROWS_WITH( stack.update(0.01f), "StateStack: push transition requested but state factory returned nullptr" )
because no exception was thrown where one was expected:

===============================================================================
test cases: 15 | 3 passed | 12 failed
assertions: 21 | 9 passed | 12 failed
```

- Read through the failing tests.
- The tests are already written to express the correct behaviour of the `StateStack`.

#### Step 2 — Implement Missing Functionality

Update your `StateStack` class to make the tests pass.

You’ll need to implement:

- `push()` - Should add a new state on top of the stack without discarding the previous state.
- `pop()` - Should remove the current top state and reveal the state beneath.
- `replace()` - Should replace the current state with a new one.
- Proper calling of lifecycle methods:
  - `onEnter()` — called when a state becomes active.
  - `onExit()` — called when a state is removed from the stack.
  - `onPause()` — called on the previous top state when a new state is pushed on top.
  - `onResume()` — called when a state underneath is uncovered by popping the top state.

#### Step 3 — Rerun Your Tests

- After each change, rerun your tests.
- Continue until all tests pass.

```markdown
Randomness seeded to: 4068396096
===============================================================================
All tests passed (35 assertions in 15 test cases)
```

### End Goal

By the end of this exercise, you’ll have a fully working `StateStack` system that:

- allows overlays like Options menus
- handles resource management and transitions properly
- is fully tested by the provided test suite

Happy coding! 🚀

### Bonus challenges

Try these if you finish early!

- Try implementing `StateStack::clear()`
- Try writing a test for render order
- Think about how you could serialize GameState for save games or persistent sessions
- Research other methods of managing game flow (Scene Graphs, Entity-Component System)

## FAQ

### Why Test-Driven Development (TDD)?

We use **Test-Driven Development (TDD)** because it helps you become a better programmer—not just someone who writes code that *runs*, but code that’s **correct, reliable, and easy to change.**

TDD helps you:

- **Think first.** You decide what your code should do before writing it.
- **Catch bugs early.** It’s much easier (and cheaper) to fix mistakes right away.
- **Code with confidence.** You know your changes won’t accidentally break something else.
- **Avoid surprises.** Tests act as a safety net for your future self.

In this project, the tests show you exactly how `StateStack` should behave. Your job is to:

- read the tests
- understand what’s expected
- write just enough code to make them pass

TDD isn’t just about writing tests—it’s about writing **better code, faster.**

It might feel slower at first, but it pays off by saving you hours of debugging later—and it’s an awesome feeling when all your tests turn green!

### Unique pointers vs raw pointers

We use `std::unique_ptr` throughout the project because:

- it automatically deletes objects when they’re no longer needed
- it helps prevent memory leaks
- it makes ownership clear. Only one part of the code owns each object

While raw pointers might seem faster, the difference is tiny in modern C++. We prefer safe, clean code first and we can always revisit it for optimization if needed.

### What are move semantics?

Move semantics let you transfer ownership of resources (like memory) from one object to another without copying.

Instead of copying all the data, it just “moves the pointer.”

```cpp
std::unique_ptr<GameState> state1 = std::make_unique<GameState>();
std::unique_ptr<GameState> state2 = std::move(state1);
```

- Now `state2` owns the object.
- `state1` becomes `nullptr`.

Why use it?

- avoids unnecessary copies
- faster and more efficient
- essential for types like `std::unique_ptr`, which can’t be copied

### What is forward declaration?

A forward declaration tells the compiler that a class or function exists, without showing its details yet.

```cpp
#pragma once

struct GLFWwindow;

class Game
{
public:
    Game();
    .
    .
    .

    GLFWwindow *window;
};
```

You should use it to:

- Avoid big includes like external libraries (e.g. GLFW).
- Speed up compile times.
- Reduce dependencies between files.

### What Are Signals?

- A signal is a way for one part of your code to broadcast that something has happened without needing to know who cares about it.
- They’re like a publisher-subscriber system.

Think of them like Unity events:

```c#
public class Player : MonoBehaviour
{
    public UnityEvent OnPlayerDied;

    void Die()
    {
        OnPlayerDied?.Invoke();
    }
}

public class GameManager : MonoBehaviour
{
    public Player player;

    void Start()
    {
        // Subscribe to the player's signal (event)
        player.OnPlayerDied.AddListener(OnPlayerDiedHandler);
    }

    void OnPlayerDiedHandler()
    {
        Debug.Log("Player has died! Game Over!");
        // e.g. show Game Over UI, stop gameplay, etc.
    }
}
```

- `OnPlayerDied` is a signal.
- Other scripts subscribe to it and react when it fires.

In this project,

- We use a third-party signals library.
- `OptionsState` uses a signal to tell the game whether fullscreen should be turned on or off.
- This keeps UI code separate from the game’s low-level window management.

```cpp
// declaration
fteng::signal<void(bool)> onFullscreenToggled;

// firing the event
onFullscreenToggled(fullscreen);

// listening to the event
auto optionsState = std::make_unique<OptionsState>();
optionsState->onFullscreenToggled.connect(
    [](bool fullscreen)
    {
        setFullscreen(fullscreen);
    });
```

### Why use exception handling?

```cpp
// In main.cpp
try
{
    Game game;
    game.run();
}
catch (...)
{
    std::cerr << "Unknown exception caught" << std::endl;
    return -1;
}

// Example in state_stack.cpp
void StateStack::replace(std::unique_ptr<GameState> state)
{
    if (!state)
        throw std::runtime_error("StateStack: replace received nullptr GameState");
    .
    .
    .
```

- Don’t let errors quietly get lost in logs.
- Fail loudly if something happens that should never happen.
- This makes bugs easier to find and prevents hidden crashes later.

## Credits

- Man on a Beach logo created with Canva. [https://www.canva.com](https://www.canva.com)

## 📝 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

Feel free to use, modify, and distribute in your own projects!

## ❓ Questions or Feedback

Feel free to reach out if you have questions or suggestions:

📧 **Email:** <muhammad_fadzuli_mohamad_said@myaccount.nyp.edu.sg>

Happy coding! 🚀

---
