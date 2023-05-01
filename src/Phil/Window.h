#pragma once

#include <GLAD/glad.h>
#include <SDL2/SDL.h>

#include "Opengl Objects/GLDebug.h"

using namespace std;

namespace Phil
{
	class Window
	{
	public:
		Window(SDL_Window* window);
		Window() {
			m_window = NULL;
			m_width = 0;
			m_height = 0;
		};

		~Window();

		void CreateWindow(SDL_Window* window);

		void Resize(int width, int height);

		SDL_Window* GetWindow() const { return m_window; };

		int GetW() const { return m_width; };
		int GetH() const { return m_height; };

	private:
		SDL_Window* m_window;
		int m_width;
		int m_height;
	};
}