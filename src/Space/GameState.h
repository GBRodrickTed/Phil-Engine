#pragma once
#include <iostream>
#include <vector>

#include "Player.h"
#include "Object.h"
#include "Phil/Utility.h"

class Game;
class Time;

class GameState {
public:
    virtual void Init(Game* game) {};
    virtual void Cleanup() {};

    virtual void Pause() {};
    virtual void Resume() {};

    virtual void HandleEvent() {};
    virtual void Update(Time* time) {};
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
    void Update(Time* time);
    void Render();
private:
    std::vector<GameState*> states;
    Game* m_game;
};

class Screen_Play : public GameState {
public:
    void Init(Game* game);
    void Cleanup();

    void HandleEvent();
    void Update(Time* time);
    void Render();
private:
    Game* game;

    Player player;

    Object objs[1000];
    int objNum;

    float elast = 0.75;
    float response_coef = 0.75;
    float fric = 1;
    float coef = 2.0f;

    float force;
    float pointForce = 50000;
    bool boarder = true;
    float gameTime;
    float scrScale;
    int chosen = -1;
    bool isGravity = false;
    bool isForceMouse = false;
    float gStrength = 1;
    vec2 gravity = { 0.f, 1000.f };
    float energy;
    float momentum;

    bool mouseDown[6];
};
static Screen_Play* GS_Screen_Play = new Screen_Play;