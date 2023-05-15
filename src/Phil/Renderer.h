#pragma once

#include <GLAD/glad.h>
#include <SDL2/SDL.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Opengl Objects/Texture.h"
#include "Opengl Objects/GLDebug.h"

#include "Opengl Objects/Shader.h"
#include "Rect.h"
#include "Window.h"
#include "Camera.h"

using namespace std;

namespace Phil {
	
	class Renderer {
	public:
		Renderer();
		~Renderer();

		void Init(Phil::Window* window);

		unsigned int GetScreenTexture() const;

		void AddRect(const Phil::Rect& rect);

		void AddRect(const Phil::Rect& rect, float angle);

		void AddRect(Texture* texture, const Phil::Rect& rect);

		void AddRect(Texture* texture, const Phil::Rect& rect, float angle);

		void AddLine(float x1, float x2, float y1, float y2);

		void AddLine(const glm::vec2& p1, const glm::vec2& p2);

		void AddLineRect(const Phil::Rect& rect);

		void AddLineRect(const Phil::Rect& rect, float angle);

		void DrawRect(const Phil::Rect& rect, Phil::Shader& shader);

		void DrawRect(Texture* texture, const Phil::Rect& rect, Phil::Shader& shader);

		void DrawScreen(Phil::Shader& shader);

		void Present();

		void Clear();

		void SetDrawColor(const glm::vec4& color);

		void SetClearColor(const glm::vec4& color);

		void SetShader(const Shader& shader);

		Camera camera;

	private:
		struct Vertex {
			float Position[3];
			float Color[4];
			float UV[2];
			float TexID;
		};

		void DrawBatch();

		void SwapFrameBuffer();

		unsigned int m_VAO, m_VBO, m_EBO, m_VAO_scr, m_VBO_scr;

		enum DrawType {
			QUAD = 0,
			LINE = 1
		};

		DrawType m_drawType;

		float* m_vertices;
		unsigned int* m_indices;

		int m_vertCount;

		int m_vertBufferEnd;

		unsigned int m_vertSize, m_posSize, m_colorSize, m_uvSize, m_texIDSize;

		Shader m_texBatchShader;
		Shader m_lineBatchShader;

		Phil::Shader m_screenShader;

		int m_samplerArray[32];
		int m_slottedTexs[32];

		float m_screen_vert[24];

		int m_texBufferEnd;

		int m_maxTexSlots;
		int m_maxVerts;

		unsigned int m_framebuffer;
		unsigned int m_scrTexture[2];

		bool m_currTex;

		glm::vec4 m_drawColor;
		glm::vec4 m_clearColor;

		Phil::Window* m_window;

	};
}

