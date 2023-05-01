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
		this->SetCameraMat(x, y, w, h);
		centered = false;
	}

	Camera::~Camera(){}

	void Camera::SetCameraMat(float x, float y, float w, float h) {
		m_pos = glm::vec2(x, y);
		m_size = glm::vec2(w, h);

		this->UpdateMatrix();
	}

	void Camera::SetWindowMat(int w, int h) {
		m_windowMat = glm::ortho(0.0f, w*1.0f, h*1.0f, 0.0f);
		m_windowSize = glm::vec2(w, h);
	}

	void Camera::SetPos(const glm::vec2& pos) {
		m_pos = pos;
		this->UpdateMatrix();
	}

	void Camera::SetSize(const glm::vec2& size) {
		m_size = size;
		this->UpdateMatrix();
	}

	glm::vec2 Camera::TransMouse(const glm::ivec2& mouse) {
		glm::vec2 gMouse;
		
		gMouse = (glm::vec2)mouse * (m_size / m_windowSize) + m_pos;
		//gMouse.x = mouse.x * (m_windowSize()) + m_pos.x;
		//gMouse.y = mouse.y + m_pos.y;

		return gMouse;
	}

	void Camera::UpdateMatrix() {
		m_matrix = glm::ortho(m_pos.x, (m_size.x + m_pos.x), (m_size.y + m_pos.y), m_pos.y, -1.0f, 1.0f);
	}
}