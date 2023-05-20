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
	objNum = 2;
	force = 50000;

	objs.push_back(new Object());
	objs.push_back(new Object());

	objs[0]->pos = vec2(300, 100);
	objs[0]->r = 100;
	objs[0]->mass = 100;
	objs[0]->setVelocity(vec2(0, 20));

	objs[1]->pos = vec2(300, 1000);
	objs[1]->r = 100;
	objs[1]->mass = 10000;
	objs[1]->setVelocity(vec2(0, 0));
	
	//for (int i = 0; i < objNum; i++) {
	//	/*vec2 init_pos = vec2(
	//		rand_f(0, game->window->GetW() * 5),
	//		rand_f(0, game->window->GetH() * 5)
	//	);
	//	float rad = rand_f(50, 100);
	//	objs.push_back(new Object(
	//		rad,
	//		init_pos,
	//		init_pos,
	//		vec2(0.f),
	//		rad
	//	));*/
	//	objs.push_back(new Object());
	//}
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
	using namespace std;
	game->renderer->camera.SetSize(vec2(game->window->GetW(), game->window->GetH()) * 5.0f);
	ivec2 mouse;
	vec2 gMouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);

	gMouse = game->renderer->camera.TransMouse(mouse);


	for (int i = 0; i < objNum; i++) {
		//objs[i]->vel *= pow(0.95f, time->dt * 1);

		if (mouseDown[SDL_BUTTON_LEFT]) {
			vec2 delta = objs[i]->pos - gMouse;
			objs[i]->acc.x += cosf((atan2f(delta.y, delta.x)) + M_PI) * (force / objs[i]->mass);
			objs[i]->acc.y += sinf((atan2f(delta.y, delta.x)) + M_PI) * (force / objs[i]->mass);
		}

		if (mouseDown[SDL_BUTTON_RIGHT]) {
			objs[i]->pos = gMouse;
			objs[i]->pos_prev = gMouse;
		}

		if (mouseDown[SDL_BUTTON_MIDDLE]) {
			objs[i]->pos_prev = objs[i]->pos;
		}
		//objs[i]->acc += gravity;

		float elast = 1;
		const float response_coef = 1.f;

		for (int j = i; j < objNum; j++) {
			if (objs[i] != objs[j]) {
				float dist = distance(objs[i]->pos, objs[j]->pos);
				float radSum = objs[i]->r + objs[j]->r;
				if (dist > 0.0001f && dist < radSum) {
					vec2 vrel = objs[i]->getVelocity() - objs[j]->getVelocity();

					vec2 normal = normalize(objs[i]->pos - objs[j]->pos);

					float impMag = -(1 + elast) * dot(vrel, normal) / ((1.0f / objs[i]->mass) + (1.0f / objs[j]->mass));
					
					vec2 jn = normal * impMag;

					cout << jn.x << endl;
					cout << jn.y << endl;

					objs[i]->addImpulse(jn);
					objs[j]->addImpulse(-jn);
				}
			}
		}
		cout << objs[i]->getVelocity().x << ", " << objs[i]->getVelocity().y << " " << i + 1 << endl;

		/*for (int j = 0; j < objNum; j++) {
			if (objs[i] != objs[j]) {
				float dist = distance(objs[i]->pos, objs[j]->pos);
				float radSum = objs[i]->r + objs[j]->r;
				if (dist > 0.0001f && dist < radSum) {
					const float mass_ratio_1 = objs[i]->mass / (objs[i]->mass + objs[j]->mass);
					const float mass_ratio_2 = objs[j]->mass / (objs[i]->mass + objs[j]->mass);

					const vec2 col_vec = ((objs[i]->pos - objs[j]->pos) / dist);
					const float delta = 0.5f * response_coef * (dist - radSum);
					objs[i]->pos -= col_vec * (mass_ratio_2 * delta);
					objs[j]->pos += col_vec * (mass_ratio_1 * delta);
				}
			}
		}*/

		/*if ((objs[i]->pos.y + objs[i]->mass) >= game->window->GetH() * 5.0f) {
			objs[i]->pos.y = game->window->GetH() * 5.0f - objs[i]->r - 1;
			objs[i]->vel.y = -objs[i]->vel.y * pow(elast, 2);
		}
		if ((objs[i]->pos.y - objs[i]->mass) <= 0) {
			objs[i]->pos.y = objs[i]->r + 1;
			objs[i]->vel.y = -objs[i]->vel.y * pow(elast, 2);
		}
		if ((objs[i]->pos.x + objs[i]->mass) >= game->window->GetW() * 5.0f) {
			objs[i]->pos.x = game->window->GetW() * 5.0f - objs[i]->r - 1;
			objs[i]->vel.x = -objs[i]->vel.x * pow(elast, 2);
		}
		if ((objs[i]->pos.x - objs[i]->mass) <= 0) {
			objs[i]->pos.x = objs[i]->r + 1;
			objs[i]->vel.x = -objs[i]->vel.x * pow(elast, 2);
		}*/

		objs[i]->update(time->dt);

	}

	//player.vel *= pow(0.95f, time->dt * 1);

	/*if (mouseDown[SDL_BUTTON_LEFT]) {
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
	}*/

	//player.vel.y += 9800 * time->dt;
	/*player.vel += player.dir * (player.force / player.mass) * time->dt;
	player.pos += player.vel * time->dt;*/

	/*float elast = 1.01;

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
	}*/

	//float totalVel = sqrt(pow(player.vel.x, 2) + pow(player.vel.y, 2));
	//momentum = totalVel * player.mass;
	//energy = 0.5 * player.mass * pow(totalVel, 2);//pow(momentum, 2)/(2*player.mass);
}
void Screen_Play::Render() {
	Phil::Rect rect = { {100, 100}, {100, 100} };
	Phil::Texture tex("res/gfx/pixel_phil.png", GL_RGBA, GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP);
	game->renderer->Clear();

	game->renderer->camera.SetSize(vec2(game->window->GetW(), game->window->GetH()) * 5.0f);
	for (int i = 0; i < objNum; i++) {
		game->renderer->AddCircle({ objs[i]->pos, objs[i]->r });
	}

	/*game->renderer->camera.SetSize(vec2(game->window->GetW(), game->window->GetH()) * 5.0f);
	game->renderer->AddCircle({ player.pos, player.mass });

	game->renderer->camera.SetSize(vec2(game->window->GetW(), game->window->GetH()) * 1.0f);
	game->renderer->AddLine(vec2(100), vec2(100.f + (player.vel)/10.f));*/

	game->renderer->Present();

	{
		//float newMass = player.mass;
		static int counter = 0;

		ImGui::Begin("Hello, world!");

		ImGui::Text("This is some useful text.");

		for (int i = 0; i < objNum; i++) {
			momentum += objs[i]->mass * objs[i]->getVelocity().y;
		}

		//ImGui::InputFloat("Mass", &player.mass);
		//ImGui::InputFloat("force", &player.force);
		ImGui::Text("Momentum: %0.6f", momentum);
		momentum = 0;
		//ImGui::Text("Kinetic Energy: %0.6f", energy);
		//ImGui::Text("Pos: %0.6f, %0.6f", player.pos.x, player.pos.y);
		//ImGui::Text("Norm Vel: (%0.6f, %0.6f)", glm::normalize(player.vel).x, glm::normalize(player.vel).y);
		ImGui::End();
	}

}
