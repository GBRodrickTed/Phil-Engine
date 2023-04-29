#include "Window.h"

namespace Phil
{
	Window::Window(SDL_Window* window) {
		this->CreateWindow(window);
	}

	Window::~Window() {
		SDL_DestroyWindow(m_window);
	}

	void Window::CreateWindow(SDL_Window* window) {
		m_window = window;
		SDL_GetWindowSize(m_window, &m_width, &m_height);
		m_context = SDL_GL_CreateContext(m_window);
		gladLoadGLLoader(SDL_GL_GetProcAddress);

		glViewport(0, 0, m_width, m_height);
	}

	void Window::Resize(int width, int height) {
		if (SDL_GetWindowFlags(m_window) & SDL_WINDOW_RESIZABLE) {
			cout << "121" << endl;
		}
	}

}