#pragma once
#include <iostream>
#include <vector>

#include "Player.h"

class Game;

class GameState {
public:
    virtual void Init(Game* game) {};
    virtual void Cleanup() {};

    virtual void Pause() {};
    virtual void Resume() {};

    virtual void HandleEvent() {};
    virtual void Update(float dt) {};
    virtual void Render() {};
};

class GameStateManager {
public:
    void Init(Game* game);
    void Destroy();

    void ChangeState(GameState* state);
    void PushState(GameState* state);
    void PopState();

    GameState* GetState();
    GameState* GetPrevState();

    void HandleEvent();
    void Update(float dt);
    void Render();
private:
    std::vector<GameState*> states;
    Game* m_game;
};

class Screen_Play : public GameState {
public:
    void Init(Game* game);

    void HandleEvent();
    void Update(float dt);
    void Render();
private:
    Game* game;

    Player player;

    bool mouseDown[6];
};
static Screen_Play* GS_Screen_Play = new Screen_Play;