#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "game/game.hpp"
#include "game/states/loading_state.hpp"

Game::Game()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    initGLFW(800, 600);

    initGlad();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    imGuiManager = std::make_unique<ImGuiManager>(window, 800, 600);

    stateStack.push(std::make_unique<LoadingState>(*this));
}

Game::~Game()
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

void Game::run()
{
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        update(deltaTime);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::initGLFW(int windowWidth, int windowHeight)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, "gamestate", NULL, NULL);
    if (!window)
        throw std::runtime_error("Failed to create window");

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int windowWidth, int windowHeight)
                                   {
        if (Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window)))
            game->resize(windowWidth, windowHeight); });
}

void Game::initGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");
}

void Game::update(float deltaTime)
{
    stateStack.update(deltaTime);

    if (stateStack.isEmpty())
        glfwSetWindowShouldClose(window, true);
}

void Game::render()
{
    glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    imGuiManager->newFrame();

    stateStack.render();

    imGuiManager->renderFrame();
}

void Game::resize(int width, int height)
{
    imGuiManager->resize(width, height);
}

std::unique_ptr<GameState> Game::makeOptionsState()
{
    auto optionsState = std::make_unique<OptionsState>(*this);
    optionsState->onFullscreenToggled.connect(
        [this](bool fullscreen)
        {
            setFullscreen(fullscreen);
        });
    return std::move(optionsState);
}

void Game::setFullscreen(bool fullscreen)
{
    if (fullscreen)
    {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(
            window,
            monitor,
            0, 0,
            mode->width, mode->height,
            mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(
            window,
            nullptr,
            100, 100,
            800, 600,
            0);
    }
}

StateStack &Game::getStateStack()
{
    return stateStack;
}