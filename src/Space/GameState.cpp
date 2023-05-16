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
void GameStateManager::Update(Time* time) {
	states.back()->Update(time);
}
void GameStateManager::Render() {
	states.back()->Render();
}

void Screen_Play::Init(Game* game) {
	this->game = game;
	objNum = 10;
	for (int i = 0; i < objNum; i++) {
		objs.push_back(new Object(
			rand_f(1, 2),
			vec2(
				rand_f(0, game->window->GetW()),
				rand_f(0, game->window->GetH())
			),
			vec2(0.f),
			vec2(0.f),
			rand_f(50, 100)
		));
	}
}

void Screen_Play::Cleanup() {
	for (int i = 0; i < objNum; i++) {
		delete objs[i];
	}
	objs.clear();
}

void Screen_Play::HandleEvent() {
	SDL_Event event;

	player.dir = vec2(0);
	const unsigned char* keystate = SDL_GetKeyboardState(NULL);

	/*if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
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
	}*/
	
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);
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
void Screen_Play::Update(Time* time) {
	game->renderer->camera.SetSize(vec2(game->window->GetW(), game->window->GetH()) * 5.0f);
	ivec2 mouse;
	vec2 gMouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);

	gMouse = game->renderer->camera.TransMouse(mouse);

	for (int i = 0; i < objNum; i++) {
		objs[i]->vel *= pow(0.95f, time->dt * 1);
	}

	//player.vel *= pow(0.95f, time->dt * 1);

	if (mouseDown[SDL_BUTTON_LEFT]) {
		vec2 delta = player.pos - gMouse;

		player.vel.x += time->dt * cosf((atan2f(delta.y, delta.x)) + M_PI) * (player.force / player.mass);
		player.vel.y += time->dt * sinf((atan2f(delta.y, delta.x)) + M_PI) * (player.force / player.mass);
	}

	if (mouseDown[SDL_BUTTON_RIGHT]) {
		player.pos = gMouse;
		player.vel = vec2(0);
	}

	if (mouseDown[SDL_BUTTON_MIDDLE]) {
		player.vel = vec2(0);
	}

	//player.vel.y += 9800 * time->dt;
	player.vel += player.dir * (player.force / player.mass) * time->dt;
	player.pos += player.vel * time->dt;

	float elast = 1.01;

	if ((player.pos.y + player.mass) >= game->window->GetH() * 5.0f) {
		player.pos.y = game->window->GetH() * 5.0f - player.mass-1;
		player.vel.y = -player.vel.y * pow(elast, 2);
	}
	if ((player.pos.y - player.mass) <= 0) {
		player.pos.y = player.mass + 1;
		player.vel.y = -player.vel.y * pow(elast, 2);
	}
	if ((player.pos.x + player.mass) >= game->window->GetW() * 5.0f) {
		player.pos.x = game->window->GetW() * 5.0f - player.mass - 1;
		player.vel.x = -player.vel.x * pow(elast, 2);
	}
	if ((player.pos.x - player.mass) <= 0) {
		player.pos.x = player.mass + 1;
		player.vel.x = -player.vel.x * pow(elast, 2);
	}

	float totalVel = sqrt(pow(player.vel.x, 2) + pow(player.vel.y, 2));
	momentum = totalVel * player.mass;
	energy = 0.5 * player.mass * pow(totalVel, 2);//pow(momentum, 2)/(2*player.mass);
}
void Screen_Play::Render() {
	Phil::Rect rect = { {100, 100}, {100, 100} };
	Phil::Texture tex("res/gfx/pixel_phil.png", GL_RGBA, GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP);
	game->renderer->Clear();

	game->renderer->camera.SetSize(vec2(game->window->GetW(), game->window->GetH()) * 5.0f);
	game->renderer->AddCircle({ player.pos, player.mass });

	game->renderer->camera.SetSize(vec2(game->window->GetW(), game->window->GetH()) * 1.0f);
	game->renderer->AddLine(vec2(100), vec2(100.f + (player.vel)/10.f));

	game->renderer->Present();

	{
		float newMass = player.mass;
		static int counter = 0;

		ImGui::Begin("Hello, world!");

		ImGui::Text("This is some useful text.");

		ImGui::InputFloat("Mass", &player.mass);
		ImGui::InputFloat("force", &player.force);
		if (player.mass < 0.01) {
			player.mass = 0.01;
		}
		if (player.mass != newMass) {
			float newVel = momentum / player.mass;
			player.vel = glm::normalize(player.vel) * newVel;
		}
		ImGui::Text("Momentum: %0.6f", momentum);
		ImGui::Text("Kinetic Energy: %0.6f", energy);
		ImGui::Text("Pos: %0.6f, %0.6f", player.pos.x, player.pos.y);
		//ImGui::Text("Norm Vel: (%0.6f, %0.6f)", glm::normalize(player.vel).x, glm::normalize(player.vel).y);
		ImGui::End();
	}

}
