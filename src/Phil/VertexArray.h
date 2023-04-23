#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>

#include "Rect.h"

#define SCR_W 800
#define SCR_H 600

namespace Phil {
	

	class VertexArray {
		public:
			VertexArray();
			unsigned int vao, vbo, ebo;

			unsigned int vertSize, posSize, colorSize, texCoordSize, texIDSize;

			glm::mat4 projection = glm::ortho(0.0f, SCR_W * 1.0f, SCR_H * 1.0f, 0.0f, -1.0f, 1.0f);

			float texIndex;

			std::vector<float> vertices;
			std::vector<unsigned int> indices;

			void Bind();
			void AddQuad(const glm::vec2& x1, const glm::vec2& x2, const glm::vec2& y1, const glm::vec2& y2);
			void AddQuad(unsigned int textureID, const glm::vec2& x1, const glm::vec2& x2, const glm::vec2& y1, const glm::vec2& y2);
			void AddRect(const Phil::Rect& rect);
			void AddRect(unsigned int textureID, const Phil::Rect& rect);
			void AddRect(unsigned int textureID, const Phil::Rect& rect, float angle);
			void Clear();
	private:
		struct Vertex1 {
			float Position[3];
			float Color[4];
			float TexCoord[2];
			float textureID;
		};
	};

	unsigned int LoadTexture(const std::string& path);
}
