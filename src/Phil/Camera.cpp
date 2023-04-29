#include "Camera.h"

namespace Phil
{
	Camera::Camera()
	{
		m_matrix = glm::mat4(1.0f);
		m_pos = glm::vec2(1.0f);
		m_size = glm::vec2(1.0f);

		centered = false;
	}

	Camera::Camera(float x, float y, float w, float h)
	{
		this->SetCamera(x, y, w, h);
		centered = false;
	}

	Camera::~Camera(){}

	void Camera::SetCamera(float x, float y, float w, float h) {
		m_pos = glm::vec2(x, y);
		m_size = glm::vec2(w, h);

		this->UpdateMatrix();
	}

	void Camera::SetPos(const glm::vec2& pos) {
		m_pos = pos;
		this->UpdateMatrix();
	}

	void Camera::SetSize(const glm::vec2& size) {
		m_size = size;
		this->UpdateMatrix();
	}

	void Camera::UpdateMatrix() {
		if (centered) {
			m_matrix = glm::ortho((-m_size.x / 2 + m_pos.x), (m_size.x / 2 + m_pos.x), (m_size.y / 2 + m_pos.y), (-m_size.y / 2 + m_pos.y), -1.0f, 1.0f);
		}
		else {
			m_matrix = glm::ortho(m_pos.x, (m_size.x + m_pos.x), (m_size.y + m_pos.y), m_pos.y, -1.0f, 1.0f);
		}
	}
}