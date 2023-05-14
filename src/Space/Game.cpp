#include "Space/Game.h"

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

	m_window = new Phil::Window;
	m_scrSize = glm::vec2(800, 600);
	m_window->CreateWindow(SDL_CreateWindow(
		"Phil Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		m_scrSize.x,
		m_scrSize.y,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	));

	m_gl_context = SDL_GL_CreateContext(m_window->GetWindow());
	SDL_GL_MakeCurrent(m_window->GetWindow(), m_gl_context);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	glViewport(0, 0, m_window->GetW(), m_window->GetH());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(m_window->GetWindow(), m_gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_renderer = new Phil::Renderer;
	m_renderer->Init(m_window);

	running = true;
}

Game::~Game() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	delete m_window;
	delete m_renderer;

	SDL_GL_DeleteContext(m_gl_context);
	SDL_Quit();
}

void Game::Loop() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = false;
		}
	}

	Phil::Rect rect = { {100, 100}, {100, 100} };

	m_renderer->Clear();

	m_renderer->AddRect(rect);

	m_renderer->Present();
	SDL_GL_SwapWindow(m_window->GetWindow());
}