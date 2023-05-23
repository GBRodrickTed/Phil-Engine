#include "Space/Game.h"
#include "Space/GameState.h"

std::chrono::duration<double> Time::getTime() {
	return std::chrono::system_clock::now() - programStart;
}

void Time::FPS(unsigned char fps) {
	using namespace std::chrono;

	static duration<double> currTime = duration<double>::zero();
	static duration<double> eventTime = duration<double>::zero();

	duration<double> invFps = duration<double>{ 1. / fps };

	eventTime = getTime() - currTime;
	// :)
	if (eventTime < invFps) {
		std::this_thread::sleep_for(invFps - eventTime);
	}

	frameTime = static_cast<float>(getTime().count() - currTime.count());
	currTime = getTime();
}

Game::Game(){

	SDL_Init(SDL_INIT_VIDEO);

	// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
	const char* glsl_version = "#version 100";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
	// GL 3.2 Core + GLSL 150
	const char* glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
		SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetSwapInterval(1); // vsync

	window = new Phil::Window;
	m_scrSize = glm::vec2(800, 600);
	window->CreateWindow(SDL_CreateWindow(
		"Phil Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_scrSize.x,
		m_scrSize.y,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	));

	m_gl_context = SDL_GL_CreateContext(window->GetWindow());
	SDL_GL_MakeCurrent(window->GetWindow(), m_gl_context);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	glViewport(0, 0, window->GetW(), window->GetH());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window->GetWindow(), m_gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderer = new Phil::Renderer;
	renderer->Init(window);

	running = true;
	stateManager = new GameStateManager;
}

Game::~Game() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete window;
	delete renderer;

	SDL_GL_DeleteContext(m_gl_context);
	SDL_Quit();
}

void Game::Loop() {
	stateManager->Init(this);
	stateManager->PushState(GS_Screen_Play);

	while (running) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window->GetWindow());
		ImGui::NewFrame();

		stateManager->HandleEvent();

		time.accumulator += time.frameTime * 2;

		while (time.accumulator >= time.dt) {
			stateManager->Update(&time);
			time.accumulator -= time.dt;
			time.gameTime += time.dt;
		}
		stateManager->Render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window->GetWindow());
		time.FPS(60);
	}

	stateManager->Destroy();
}