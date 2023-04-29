#pragma once

#include <GLAD/glad.h>
#include <SDL2/SDL.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Opengl Objects/VertexArray.h"
#include "Opengl Objects/VertexBuffer.h"
#include "Opengl Objects/IndexBuffer.h"
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
		Renderer(Phil::Window* window);
		~Renderer();

		void AddRect(const Phil::Rect& rect);

		void AddRect(const Phil::Rect& rect, float angle);

		void AddRect(Texture* texture, const Phil::Rect& rect);

		void AddRect(Texture* texture, const Phil::Rect& rect, float angle);

		void AddLine(float x1, float x2, float y1, float y2);

		void AddLineRect(const Phil::Rect& rect);

		void AddLineRect(const Phil::Rect& rect, float angle);

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

		VertexArray m_VAO;
		VertexBuffer m_VBO;
		IndexBuffer m_EBO;

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

		Shader m_basicShader;
		Shader m_lineShader;

		int m_samplerArray[32];
		int m_slottedTexs[32];

		int m_texBufferEnd;

		int m_maxTexSlots;
		int m_maxVerts;

		glm::vec4 m_drawColor;
		glm::vec4 m_clearColor;

		Phil::Window* m_window;

	};
}

