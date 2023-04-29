#pragma once

#include <GLAD/glad.h>
#include <SDL2/SDL.h>

#include <vector>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

namespace Phil
{
	class Camera
	{
	public:

		Camera();

		Camera(float x, float y, float w, float h);

		~Camera();

		glm::mat4 GetMatrix() const { return m_matrix; };

		float GetX() const { return m_pos.x; };
		float GetY() const { return m_pos.y; };
		float GetW() const { return m_size.x; };
		float GetH() const { return m_size.y; };

		glm::vec2 TransMouse(const glm::ivec2& mouse);

		void SetCamera(float x, float y, float w, float h);

		void SetPos(const glm::vec2& pos);

		void SetSize(const glm::vec2& size);

		bool centered;

	private:

		void UpdateMatrix();

		glm::mat4 m_matrix;

		glm::vec2 m_pos;
		glm::vec2 m_size;
	};
}