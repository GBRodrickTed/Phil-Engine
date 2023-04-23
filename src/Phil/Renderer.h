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

namespace Phil {
	
	class Renderer {
	public:
		Renderer();
		~Renderer();

		void Bind();

		void AddRect(const Phil::Rect& rect);

		void AddRect(const Phil::Rect& rect, float angle);

		void AddRect(Texture* texture, const Phil::Rect& rect);

		void AddRect(Texture* texture, const Phil::Rect& rect, float angle);

		void DrawBatch();

		void Clear();

		void SetDrawColor(const glm::vec4& color);

		void SetClearColor(const glm::vec4& color);

		void SetProjection(const glm::mat4& projection);

		void SetShader(const Shader& shader);

	private:
		struct Vertex {
			float Position[3];
			float Color[4];
			float TexCoord[2];
			float TexID;
		};
		VertexArray m_VAO;
		VertexBuffer m_VBO;
		IndexBuffer m_EBO;

		float* m_vertices;
		unsigned int* m_indices;

		int m_quadCount;
		int m_vertBufferEnd;

		glm::mat4 m_projection;

		glm::vec4 m_drawColor;
		glm::vec4 m_clearColor;

		unsigned int m_vertSize, m_posSize, m_colorSize, m_texCoordSize, m_texIDSize;

		Shader m_basicShader;

		int m_samplerArray[32];
		int m_slottedTexs[32];

		int m_texBufferEnd;

		int m_maxTexSlots;
		int m_maxQuads;

	};
}

