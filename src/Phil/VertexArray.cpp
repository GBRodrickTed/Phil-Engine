#include "VertexArray.h"



namespace Phil {
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		texIndex = 0; // zero in this case means no texture

		posSize = 3;
		colorSize = 4;
		texCoordSize = 2;
		texIDSize = 1;
		
		vertSize = posSize + colorSize + texCoordSize + texIDSize;

		vertices.reserve(1000*vertSize);
		indices.reserve(1000*6);
	}

	void VertexArray::Bind() {

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex1) * 1000, NULL, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 1000, NULL, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, posSize, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (const void*)offsetof(Vertex1, Position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, colorSize, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (const void*)offsetof(Vertex1, Color));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, texCoordSize, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (const void*)offsetof(Vertex1, TexCoord));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, texIDSize, GL_FLOAT, GL_FALSE, sizeof(Vertex1), (const void*)offsetof(Vertex1, textureID));
		glEnableVertexAttribArray(3);
	}

	void VertexArray::AddQuad(const glm::vec2& x1, const glm::vec2& x2, const glm::vec2& y1, const glm::vec2& y2) {
		vertices.insert(vertices.end(), {
			x1.x, x1.y, 0.0f, 0.0f, 0.0f,
			x2.x, x2.y, 0.0f, 0.0f, 0.0f,
			y1.x, y1.y, 0.0f, 0.0f, 0.0f,
			y2.x, y2.y, 0.0f, 0.0f, 0.0f
			}
		);
		
		unsigned int offset = (vertices.size()/vertSize) - 4;

		indices.insert(indices.end(), {
			offset + 0, offset + 1, offset + 2,
			offset + 1, offset + 2, offset + 3
			}
		);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(&vertices[0]) * vertices.size(), &vertices[0]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(&indices[0]) * indices.size(), &indices[0]);
	}

	void VertexArray::AddQuad(unsigned int textureID, const glm::vec2& x1, const glm::vec2& x2, const glm::vec2& y1, const glm::vec2& y2) {
		texIndex++;
		vertices.insert(vertices.end(), {
			x1.x, x1.y, 0.0f, 0.0f, texIndex,
			x2.x, x2.y, 1.0f, 0.0f, texIndex,
			y1.x, y1.y, 0.0f, 1.0f, texIndex,
			y2.x, y2.y, 1.0f, 1.0f, texIndex
			}
		);

		unsigned int offset = (vertices.size() / vertSize) - 4;

		indices.insert(indices.end(), {
			offset + 0, offset + 1, offset + 2,
			offset + 1, offset + 2, offset + 3
			}
		);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(&vertices[0]) * vertices.size(), &vertices[0]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(&indices[0]) * indices.size(), &indices[0]);
		glActiveTexture(GL_TEXTURE0 + texIndex - 1);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void VertexArray::AddRect(const Phil::Rect& rect) {

		glm::vec4 point[4];
		point[0] = glm::vec4{ rect.pos.x, rect.pos.y, 0.0f, 1.0f };
		point[1] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y, 0.0f, 1.0f };
		point[2] = glm::vec4{ rect.pos.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };
		point[3] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };

		for (int i = 0; i < 4; i++) {
			point[i] = projection * point[i];
		}

		vertices.insert(vertices.end(), {
			point[0].x, point[0].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			point[1].x, point[1].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			point[2].x, point[2].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			point[3].x, point[3].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f
			}
		);

		unsigned int offset = (vertices.size() / vertSize) - 4;


		indices.insert(indices.end(), {
			offset + 0, offset + 1, offset + 2,
			offset + 1, offset + 2, offset + 3
			}
		);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(&vertices[0]) * vertices.size(), &vertices[0]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(&indices[0]) * indices.size(), &indices[0]);
		
	}

	void VertexArray::AddRect(unsigned int textureID, const Phil::Rect& rect) {
		texIndex++;

		glm::vec4 point[4];
		point[0] = glm::vec4{ rect.pos.x, rect.pos.y, 0.0f, 1.0f };
		point[1] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y, 0.0f, 1.0f };
		point[2] = glm::vec4{ rect.pos.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };
		point[3] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };

		for (int i = 0; i < 4; i++) {
			point[i] = projection * point[i];
		}

		vertices.insert(vertices.end(), {
			point[0].x, point[0].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, texIndex,
			point[1].x, point[1].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, texIndex,
			point[2].x, point[2].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, texIndex,
			point[3].x, point[3].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, texIndex
			}
		);

		unsigned int offset = (vertices.size() / vertSize) - 4;

		indices.insert(indices.end(), {
			offset + 0, offset + 1, offset + 2,
			offset + 1, offset + 2, offset + 3
			}
		);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(&vertices[0]) * vertices.size(), &vertices[0]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(&indices[0]) * indices.size(), &indices[0]);
		glActiveTexture(GL_TEXTURE0 + texIndex - 1);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void VertexArray::AddRect(unsigned int textureID, const Phil::Rect& rect, float angle) {
		texIndex++;
		glm::mat4 model(1.0f);
		
		model = glm::translate(model, glm::vec3(rect.pos, 0.0));
		model = glm::translate(model, glm::vec3(0.5 * rect.size.x, 0.5 * rect.size.y, 0.0));
		model = glm::rotate(model, glm::radians(angle) * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5 * rect.size.x, -0.5 * rect.size.y, 0.0));
		model = glm::translate(model, glm::vec3(-rect.pos, 0.0));

		glm::vec4 point[4];
		point[0] = glm::vec4{ rect.pos.x, rect.pos.y, 0.0f, 1.0f };
		point[1] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y, 0.0f, 1.0f };
		point[2] = glm::vec4{ rect.pos.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };
		point[3] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };

		for (int i = 0; i < 4; i++) {
			point[i] = projection * model * point[i];
		}

		vertices.insert(vertices.end(), {
			point[0].x, point[0].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, texIndex,
			point[1].x, point[1].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, texIndex,
			point[2].x, point[2].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, texIndex,
			point[3].x, point[3].y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, texIndex
			}
		);

		unsigned int offset = (vertices.size() / vertSize) - 4;

		indices.insert(indices.end(), {
			offset + 0, offset + 1, offset + 2,
			offset + 1, offset + 2, offset + 3
			}
		);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(&vertices[0]) * vertices.size(), &vertices[0]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(&indices[0]) * indices.size(), &indices[0]);
		glActiveTexture(GL_TEXTURE0 + texIndex - 1);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void VertexArray::Clear() {
		vertices.clear();
		indices.clear();
		texIndex = 0;
	}

	unsigned int LoadTexture(const std::string& path) {
		int w, h, bits;

		//stbi_set_flip_vertically_on_load(1);

		unsigned char* data = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);

		if (!data)
		{
			std::cout << "Failed to load texture: " << path << std::endl;
			return 0;
		}

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		stbi_image_free(data);

		return textureID;
	}
}

