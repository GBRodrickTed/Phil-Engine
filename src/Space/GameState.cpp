#include "Space/GameState.h"
#include "Space/Game.h"

void GameStateManager::Init(Game* game) {
	m_game = game;
}
void GameStateManager::Destroy() {
	while (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}
}

void GameStateManager::ChangeState(GameState* state) {
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}
	states.push_back(state);
	states.back()->Init(m_game);
}
void GameStateManager::PushState(GameState* state) {
	if (!states.empty()) {
		states.back()->Pause();
	}
	states.push_back(state);
	states.back()->Init(m_game);
}
void GameStateManager::PopState() {
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	if (!states.empty()) {
		states.back()->Resume();
	}
}

GameState* GameStateManager::GetState()
{
	if (states.size() <= 0) {
		return 0;
	}
	return states[states.size() - 1];
}
GameState* GameStateManager::GetPrevState()
{
	if (states.size() <= 1) {
		return 0;
	}
	return states[states.size() - 2];
}

void GameStateManager::HandleEvent() {
	states.back()->HandleEvent();
}
void GameStateManager::Update(float dt) {
	states.back()->Update(dt);
}
void GameStateManager::Render() {
	states.back()->Render();
}

void Screen_Play::Init(Game* game) {
	this->game = game;
}

void Screen_Play::HandleEvent() {
	SDL_Event event;

	player.dir = vec2(0);
	const unsigned char* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
	{
		player.dir.y = -1;
	}

	if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
	{
		player.dir.y = 1;
	}

	if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
	{
		player.dir.x = -1;
	}

	if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
	{
		player.dir.x = 1;
	}
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			game->running = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				mouseDown[SDL_BUTTON_LEFT] = true;
				break;
			case SDL_BUTTON_RIGHT:
				mouseDown[SDL_BUTTON_RIGHT] = true;
				break;
			case SDL_BUTTON_MIDDLE:
				mouseDown[SDL_BUTTON_MIDDLE] = true;
				break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				mouseDown[SDL_BUTTON_LEFT] = false;
				break;
			case SDL_BUTTON_RIGHT:
				mouseDown[SDL_BUTTON_RIGHT] = false;
				break;
			case SDL_BUTTON_MIDDLE:
				mouseDown[SDL_BUTTON_MIDDLE] = false;
				break;
			}
			break;
		}
	}

}
void Screen_Play::Update(float dt) {
	game->renderer->camera.SetSize(vec2(game->window->GetW(), game->window->GetH()) * 5.0f);
	ivec2 mouse;
	vec2 gMouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);

	gMouse = game->renderer->camera.TransMouse(mouse);

	vec2 force = vec2(100);

	//player.vel *= pow(0.75f, dt * 1);

	if (mouseDown[SDL_BUTTON_LEFT]) {
		vec2 delta = player.pos - gMouse;

		player.vel.x += dt * cosf((atan2f(delta.y, delta.x)) + M_PI) * (force.x / player.mass);
		player.vel.y += dt * sinf((atan2f(delta.y, delta.x)) + M_PI) * (force.y / player.mass);
	}

	if (mouseDown[SDL_BUTTON_RIGHT]) {
		player.pos = gMouse;
	}

	player.vel += player.dir * (force / player.mass) * dt;
	player.pos += player.vel * dt;
}
void Screen_Play::Render() {
	Phil::Rect rect = { {100, 100}, {100, 100} };
	Phil::Texture tex("res/gfx/pixel_phil.png", GL_RGBA, GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP);
	game->renderer->Clear();

	game->renderer->AddRect(&tex, { player.pos - (player.size)/2.0f, player.size });

	game->renderer->camera.SetSize(vec2(game->window->GetW(), game->window->GetH()) * 1.0f);
	game->renderer->AddLine(vec2(100), vec2(100.f + (player.vel)/10.f));

	game->renderer->Present();
	SDL_GL_SwapWindow(game->window->GetWindow());
}
