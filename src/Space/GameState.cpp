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
	objNum = 500;
	force = 15000;
	scrScale = 100.0f;

	/*objs[0] = {
		200.0f,
		vec2(300.0f, 300.0f),
		vec2(300.0f, 300.0f),
		vec2(0.0f),
		200.0f
	};
	
	objs[1] = {
		100.0f,
		vec2(1100.0f, 1000.0f),
		vec2(1100.0f, 1000.0f),
		vec2(0.0f),
		500.0f
	};*/
	
	for (int i = 0; i < objNum; i++) {
		vec2 init_pos = vec2(
			rand_f(-game->window->GetW() * scrScale/2, game->window->GetW() * scrScale/2),
			rand_f(-game->window->GetH() * scrScale/2, game->window->GetH() * scrScale/2)
		);
		float rad = rand_f(25, 150);
		objs[i] = {
			rad,
			init_pos,
			init_pos,
			vec2(0.f),
			rad
		};
		//objs.push_back(new Object());
	}
}

void Screen_Play::Cleanup() {
}

void Screen_Play::HandleEvent() {
	SDL_Event event;

	player.dir = vec2(0);
	const unsigned char* keystate = SDL_GetKeyboardState(NULL);

	if (chosen > -1) {
		if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
		{
			objs[chosen].acc.y -= 1000;
		}

		if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
		{
			objs[chosen].acc.y += 1000;
		}

		if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
		{
			objs[chosen].acc.x -= 1000;
		}

		if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
		{
			objs[chosen].acc.x += 1000;
		}
	}
	
	if (keystate[SDL_SCANCODE_LSHIFT])
	{
		chosen = -1;
		isForceMouse = false;
		
	}
	
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
		case SDL_MOUSEWHEEL:
			if (event.wheel.y > 0) // scroll up
			{
				scrScale *= 0.85;
			}
			else if (event.wheel.y < 0) // scroll down
			{
				scrScale *= 1.15;
			}
		}
	}

}
void Screen_Play::Update(Time* time) {
	using namespace std;
	vec2 scr = vec2(game->window->GetW(), game->window->GetH());
	game->renderer->camera.SetSize(scr * scrScale);
	//vec2(game->window->GetW(), game->window->GetH()) * scrScale
	ivec2 mouse;
	vec2 gMouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);

	gMouse = game->renderer->camera.TransMouse(mouse);

	gameTime = time->gameTime;

	for (int i = 0; i < objNum; i++) {
		objs[i].setVelocity(objs[i].getVelocity() * pow(fric, time->dt * 1));
		
		if (mouseDown[SDL_BUTTON_LEFT]) {
			if (isForceMouse) {
				vec2 delta = objs[i].pos - gMouse;
				objs[i].acc.x += cosf((atan2f(delta.y, delta.x)) + M_PI) * (force / objs[i].mass);
				objs[i].acc.y += sinf((atan2f(delta.y, delta.x)) + M_PI) * (force / objs[i].mass);
			}
		}
		vec2 delta = objs[i].pos - (vec2(0,0));
		objs[i].acc.x += cosf((atan2f(delta.y, delta.x)) + M_PI) * (pointForce / objs[i].mass);
		objs[i].acc.y += sinf((atan2f(delta.y, delta.x)) + M_PI) * (pointForce / objs[i].mass);

		if (mouseDown[SDL_BUTTON_RIGHT]) 
		{
			//chosen = -1;
			if (pointCircle(gMouse, objs[i].pos, objs[i].r)) {
				//objs[i].pos_prev = gMouse;
				//objs[i].pos = gMouse;
				chosen = i;
			}
			
			//objs[i].pos_prev = gMouse;
		}

		if (mouseDown[SDL_BUTTON_MIDDLE]) {
			objs[i].pos_prev = objs[i].pos;
		}
		if (isGravity) {
			objs[i].acc += gravity * gStrength;
		}

		for (int j = i; j < objNum; j++) {
			if (i != j) {
				float dist = distance(objs[i].pos, objs[j].pos);
				float radSum = objs[i].r + objs[j].r;
				if (dist > 0.0001f && dist < radSum) {

					const float mass_ratio_1 = objs[i].mass / (objs[i].mass + objs[j].mass);
					const float mass_ratio_2 = objs[j].mass / (objs[i].mass + objs[j].mass);

					const float delta = 0.50f * (dist - radSum);
					const vec2 col_vec = coef * ((objs[i].pos - objs[j].pos) / dist);

					objs[i].pos -= col_vec * (mass_ratio_2 * delta);
					objs[j].pos += col_vec * (mass_ratio_1 * delta);

					objs[i].pos_prev -= col_vec * (mass_ratio_2 * delta);
					objs[j].pos_prev += col_vec * (mass_ratio_1 * delta);

					vec2 vrel = objs[i].getVelocity() - objs[j].getVelocity();

					vec2 normal = normalize(objs[i].pos - objs[j].pos);

					float impMag = -(1 + elast) * dot(vrel, normal) / ((1.0f / objs[i].mass) + (1.0f / objs[j].mass));
					
					vec2 jn = normal * impMag;

					objs[i].addImpulse(jn);
					objs[j].addImpulse(-jn);
				}
			}
		}

		vec2 vel = objs[i].getVelocity();
		vec2 newScr = scr * scrScale / 2.0f;
		if (boarder) {
			if ((objs[i].pos.y + objs[i].r) >= newScr.y) {
				objs[i].pos.y = newScr.y - objs[i].r - 1;
				vel.y = -vel.y * pow(response_coef, 2);
				objs[i].setVelocity(vel);
			}
			if ((objs[i].pos.y - objs[i].r) <= -newScr.y) {
				objs[i].pos.y = -newScr.y + objs[i].r;
				vel.y = -vel.y * pow(response_coef, 2);
				objs[i].setVelocity(vel);
			}
			if ((objs[i].pos.x + objs[i].r) >= newScr.x) {
				objs[i].pos.x = newScr.x - objs[i].r - 1;
				vel.x = -vel.x * pow(response_coef, 2);
				objs[i].setVelocity(vel);
			}
			if ((objs[i].pos.x - objs[i].r) <= -newScr.x) {
				objs[i].pos.x = -newScr.x + objs[i].r;
				vel.x = -vel.x * pow(response_coef, 2);
				objs[i].setVelocity(vel);
			}
		}
	}

	for (int i = 0; i < objNum; i++) {
		objs[i].update(time->dt);
	}
	
}
void Screen_Play::Render() {
	Phil::Rect rect = { {100, 100}, {100, 100} };
	Phil::Texture tex("res/gfx/pixel_phil.png", GL_RGBA, GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, GL_CLAMP, GL_CLAMP);
	game->renderer->Clear();
	vec2 scr = vec2(game->window->GetW(), game->window->GetH());
	game->renderer->camera.SetSize(scr * scrScale);
	game->renderer->camera.SetPos((-scr / 2.0f) * scrScale);

	ivec2 mouse;
	vec2 gMouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);
	gMouse = game->renderer->camera.TransMouse(mouse);

	for (int i = 0; i < objNum; i++) {
		if (i == chosen) {
			game->renderer->SetDrawColor({vec3(1.0f) + sin(gameTime)*0.5f, 1});
		}
		else {
			vec2 vel = objs[i].getVelocity();
			game->renderer->SetDrawColor({ 0, 0, 1, 1 });
			game->renderer->m_drawColor = vec4(sin(vec3(0.f, 2.f, 4.f) + (objs[i].mass/100.0f)), 1.0f);
		}
		game->renderer->AddCircle({ objs[i].pos, objs[i].r });
	}

	game->renderer->Present();

	{
		//float newMass = player.mass;
		static int counter = 0;

		ImGui::Begin("Toolbox");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
			ImGui::GetIO().Framerate);

		for (int i = 0; i < objNum; i++) {
			momentum += abs(objs[i].mass * length(objs[i].getVelocity()));
		}
		ImGui::Checkbox("Gravity", &isGravity);
		if (isGravity) {
			ImGui::SliderFloat("Strength", &this->gStrength, -3.0f, 3.0f);
		}
		ImGui::Checkbox("Force Mouse", &isForceMouse);
		ImGui::Text("Momentum: %0.6f", momentum);
		momentum = 0;
		ImGui::Checkbox("Boarder", &boarder);
		ImGui::InputFloat("Point Force", &pointForce);
		ImGui::InputFloat("Elasticty", &elast);
		ImGui::InputFloat("Wall Elast", &response_coef);
		ImGui::InputFloat("Air Friction", &fric);
		ImGui::InputFloat("Coef", &coef);
		ImGui::Text("Zoom: %0.6f", scrScale);
		ImGui::Text("World Mouse: [%0.2f, %0.2f]", gMouse.x, gMouse.y);
		if (chosen > -1) {
			ImGui::Begin("Object");
				vec2 vel = objs[chosen].getVelocity();
				ImGui::Text("Pos: [%0.2f, %0.2f]", objs[chosen].pos.x, objs[chosen].pos.y);
				ImGui::Text("Velocity: [%0.2f, %0.2f] (%0.2f)", vel.x, vel.y, length(vel));
				ImGui::Text("Mass: %0.2f", objs[chosen].mass);

				ImGui::InputFloat("Mass", &objs[chosen].mass);
				if (objs[chosen].mass < 0.01f) {
					objs[chosen].mass = 0.01f;
				}
				ImGui::Text("Radius: %0.2f", objs[chosen].r);
				ImGui::InputFloat("R", &objs[chosen].r);
				if (objs[chosen].r < 0.01f) {
					objs[chosen].r = 0.01f;
				}
			ImGui::End();
		}
		
		//ImGui::Text("Kinetic Energy: %0.6f", energy);
		//ImGui::Text("Pos: %0.6f, %0.6f", player.pos.x, player.pos.y);
		//ImGui::Text("Norm Vel: (%0.6f, %0.6f)", glm::normalize(player.vel).x, glm::normalize(player.vel).y);
		ImGui::End();
	}

}
