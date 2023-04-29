#include "Renderer.h"

namespace Phil {
	Renderer::Renderer(Phil::Window* window) :m_VBO(GL_ARRAY_BUFFER), m_maxVerts(1000), m_vertCount(0), m_drawColor(glm::vec4(1.0f)), m_clearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
	{
		m_window = window;

		int window_w, window_h;

		SDL_GetWindowSize(window->GetWindow(), &window_w, &window_h);

		camera.SetCamera(0, 0, window_w, window_h);

		memset(m_slottedTexs, -1, 32 * sizeof(int));
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_maxTexSlots);
		for (int i = 0; i < 32; i++) {
			m_samplerArray[i] = i;
		}

		m_posSize = 3;
		m_colorSize = 4;
		m_uvSize = 2;
		m_texIDSize = 1;

		m_vertSize = m_posSize + m_colorSize + m_uvSize + m_texIDSize;

		m_vertices = new float[m_vertSize * 4 * (m_maxVerts + 2)]; // + 2 just in case
		m_indices = new unsigned int[6 * (m_maxVerts + 2)];

		m_drawType = QUAD;

		m_vertBufferEnd = 0;
		m_texBufferEnd = 0;

		m_basicShader.CreateShaderFromFile("res/shaders/batch_tex_V.shader", "res/shaders/batch_tex_F.shader");
		m_lineShader.CreateShaderFromFile("res/shaders/batch_basic_V.shader", "res/shaders/batch_basic_F.shader");

		int index_offset = 0;
		size_t index_size = 6 * m_maxVerts;

		for (size_t i = 0; i < index_size; i += 6) {
			m_indices[i + 0] = 0 + index_offset;
			m_indices[i + 1] = 1 + index_offset;
			m_indices[i + 2] = 2 + index_offset;

			m_indices[i + 3] = 1 + index_offset;
			m_indices[i + 4] = 2 + index_offset;
			m_indices[i + 5] = 3 + index_offset;

			index_offset += 4;
		}
	}

	Renderer::~Renderer() {
		delete[] m_vertices;
		delete[] m_indices;
	}

	void Renderer::AddRect(const Phil::Rect& rect) {

		if (m_vertCount >= m_maxVerts || m_drawType != QUAD) {
			this->DrawBatch();
		}

		int texIndex = -1;

		glm::vec4 point[4];

		point[0] = glm::vec4{ rect.pos.x, rect.pos.y, 0.0f, 1.0f };
		point[1] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y, 0.0f, 1.0f };
		point[2] = glm::vec4{ rect.pos.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };
		point[3] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };

		for (int i = 0; i < 4; i++) {
			point[i] = camera.GetMatrix() * point[i];
		}

		// Vertex 1
		m_vertices[m_vertBufferEnd + 0] = point[0].x;
		m_vertices[m_vertBufferEnd + 1] = point[0].y;
		m_vertices[m_vertBufferEnd + 2] = point[0].z;

		m_vertices[m_vertBufferEnd + 3] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 4] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 5] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 6] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 7] = 0.0f;
		m_vertices[m_vertBufferEnd + 8] = 0.0f;
		m_vertices[m_vertBufferEnd + 9] = float(texIndex);

		// Vertex 2
		m_vertices[m_vertBufferEnd + 10] = point[1].x;
		m_vertices[m_vertBufferEnd + 11] = point[1].y;
		m_vertices[m_vertBufferEnd + 12] = point[1].z;

		m_vertices[m_vertBufferEnd + 13] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 14] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 15] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 16] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 17] = 1.0f;
		m_vertices[m_vertBufferEnd + 18] = 0.0f;
		m_vertices[m_vertBufferEnd + 19] = float(texIndex);

		// Vertex 3
		m_vertices[m_vertBufferEnd + 20] = point[2].x;
		m_vertices[m_vertBufferEnd + 21] = point[2].y;
		m_vertices[m_vertBufferEnd + 22] = point[2].z;

		m_vertices[m_vertBufferEnd + 23] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 24] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 25] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 26] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 27] = 0.0f;
		m_vertices[m_vertBufferEnd + 28] = 1.0f;
		m_vertices[m_vertBufferEnd + 29] = float(texIndex);

		// Vertex 4
		m_vertices[m_vertBufferEnd + 30] = point[3].x;
		m_vertices[m_vertBufferEnd + 31] = point[3].y;
		m_vertices[m_vertBufferEnd + 32] = point[3].z;

		m_vertices[m_vertBufferEnd + 33] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 34] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 35] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 36] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 37] = 1.0f;
		m_vertices[m_vertBufferEnd + 38] = 1.0f;
		m_vertices[m_vertBufferEnd + 39] = float(texIndex);

		m_vertBufferEnd += m_vertSize * 4;
		m_vertCount += 4;
		m_drawType = QUAD;
	}

	void Renderer::AddRect(const Phil::Rect& rect, float angle) {

		if (m_vertCount >= m_maxVerts || m_drawType != QUAD) {
			this->DrawBatch();
		}

		int texIndex = -1;

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
			point[i] = camera.GetMatrix() * model * point[i];
		}

		// Vertex 1
		m_vertices[m_vertBufferEnd + 0] = point[0].x;
		m_vertices[m_vertBufferEnd + 1] = point[0].y;
		m_vertices[m_vertBufferEnd + 2] = point[0].z;

		m_vertices[m_vertBufferEnd + 3] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 4] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 5] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 6] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 7] = 0.0f;
		m_vertices[m_vertBufferEnd + 8] = 0.0f;
		m_vertices[m_vertBufferEnd + 9] = float(texIndex);

		// Vertex 2
		m_vertices[m_vertBufferEnd + 10] = point[1].x;
		m_vertices[m_vertBufferEnd + 11] = point[1].y;
		m_vertices[m_vertBufferEnd + 12] = point[1].z;

		m_vertices[m_vertBufferEnd + 13] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 14] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 15] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 16] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 17] = 1.0f;
		m_vertices[m_vertBufferEnd + 18] = 0.0f;
		m_vertices[m_vertBufferEnd + 19] = float(texIndex);

		// Vertex 3
		m_vertices[m_vertBufferEnd + 20] = point[2].x;
		m_vertices[m_vertBufferEnd + 21] = point[2].y;
		m_vertices[m_vertBufferEnd + 22] = point[2].z;

		m_vertices[m_vertBufferEnd + 23] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 24] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 25] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 26] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 27] = 0.0f;
		m_vertices[m_vertBufferEnd + 28] = 1.0f;
		m_vertices[m_vertBufferEnd + 29] = float(texIndex);

		// Vertex 4
		m_vertices[m_vertBufferEnd + 30] = point[3].x;
		m_vertices[m_vertBufferEnd + 31] = point[3].y;
		m_vertices[m_vertBufferEnd + 32] = point[3].z;

		m_vertices[m_vertBufferEnd + 33] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 34] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 35] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 36] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 37] = 1.0f;
		m_vertices[m_vertBufferEnd + 38] = 1.0f;
		m_vertices[m_vertBufferEnd + 39] = float(texIndex);

		m_vertBufferEnd += m_vertSize * 4;
		m_vertCount += 4;
		m_drawType = QUAD;
	}

	void Renderer::AddRect(Texture* texture, const Phil::Rect& rect) {

		if (m_vertCount >= m_maxVerts || m_drawType != QUAD) {
			this->DrawBatch();
		}

		int texIndex = -1;

		bool repeated_texture = false;

		for (int i = 0; i < m_maxTexSlots; i++)
		{
			if (m_slottedTexs[i] == texture->GetTextureID())
			{
				texIndex = i;
				repeated_texture = true;
				break;
			}
		}

		if (repeated_texture == false)
		{
			if (m_texBufferEnd == m_maxTexSlots)
			{
				this->DrawBatch();
			}
			m_slottedTexs[m_texBufferEnd] = texture->GetTextureID();
			texIndex = m_texBufferEnd;
			m_texBufferEnd++;
		}

		glm::vec4 point[4];

		point[0] = glm::vec4{ rect.pos.x, rect.pos.y, 0.0f, 1.0f };
		point[1] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y, 0.0f, 1.0f };
		point[2] = glm::vec4{ rect.pos.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };
		point[3] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };

		for (int i = 0; i < 4; i++) {
			point[i] = camera.GetMatrix() * point[i];
		}

		// Vertex 1
		m_vertices[m_vertBufferEnd + 0] = point[0].x;
		m_vertices[m_vertBufferEnd + 1] = point[0].y;
		m_vertices[m_vertBufferEnd + 2] = point[0].z;

		m_vertices[m_vertBufferEnd + 3] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 4] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 5] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 6] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 7] = 0.0f;
		m_vertices[m_vertBufferEnd + 8] = 0.0f;
		m_vertices[m_vertBufferEnd + 9] = float(texIndex);

		// Vertex 2
		m_vertices[m_vertBufferEnd + 10] = point[1].x;
		m_vertices[m_vertBufferEnd + 11] = point[1].y;
		m_vertices[m_vertBufferEnd + 12] = point[1].z;

		m_vertices[m_vertBufferEnd + 13] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 14] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 15] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 16] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 17] = 1.0f;
		m_vertices[m_vertBufferEnd + 18] = 0.0f;
		m_vertices[m_vertBufferEnd + 19] = float(texIndex);

		// Vertex 3
		m_vertices[m_vertBufferEnd + 20] = point[2].x;
		m_vertices[m_vertBufferEnd + 21] = point[2].y;
		m_vertices[m_vertBufferEnd + 22] = point[2].z;

		m_vertices[m_vertBufferEnd + 23] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 24] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 25] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 26] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 27] = 0.0f;
		m_vertices[m_vertBufferEnd + 28] = 1.0f;
		m_vertices[m_vertBufferEnd + 29] = float(texIndex);

		// Vertex 4
		m_vertices[m_vertBufferEnd + 30] = point[3].x;
		m_vertices[m_vertBufferEnd + 31] = point[3].y;
		m_vertices[m_vertBufferEnd + 32] = point[3].z;

		m_vertices[m_vertBufferEnd + 33] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 34] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 35] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 36] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 37] = 1.0f;
		m_vertices[m_vertBufferEnd + 38] = 1.0f;
		m_vertices[m_vertBufferEnd + 39] = float(texIndex);

		m_vertBufferEnd += m_vertSize * 4;
		m_vertCount += 4;

		m_drawType = QUAD;
	}

	void Renderer::AddRect(Texture* texture, const Phil::Rect& rect, float angle) {

		if (m_vertCount >= m_maxVerts || m_drawType != QUAD) {
			this->DrawBatch();
		}

		int texIndex = -1;

		bool repeated_texture = false;

		for (int i = 0; i < m_maxTexSlots; i++)
		{
			if (m_slottedTexs[i] == texture->GetTextureID())
			{
				texIndex = i;
				repeated_texture = true;
				break;
			}
		}

		if (repeated_texture == false)
		{
			if (m_texBufferEnd == m_maxTexSlots)
			{
				this->DrawBatch();
			}
			m_slottedTexs[m_texBufferEnd] = texture->GetTextureID();
			texIndex = m_texBufferEnd;
			m_texBufferEnd++;
		}

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
			point[i] = camera.GetMatrix() * model * point[i];
		}

		// Vertex 1
		m_vertices[m_vertBufferEnd + 0] = point[0].x;
		m_vertices[m_vertBufferEnd + 1] = point[0].y;
		m_vertices[m_vertBufferEnd + 2] = point[0].z;

		m_vertices[m_vertBufferEnd + 3] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 4] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 5] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 6] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 7] = 0.0f;
		m_vertices[m_vertBufferEnd + 8] = 0.0f;
		m_vertices[m_vertBufferEnd + 9] = float(texIndex);

		// Vertex 2
		m_vertices[m_vertBufferEnd + 10] = point[1].x;
		m_vertices[m_vertBufferEnd + 11] = point[1].y;
		m_vertices[m_vertBufferEnd + 12] = point[1].z;

		m_vertices[m_vertBufferEnd + 13] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 14] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 15] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 16] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 17] = 1.0f;
		m_vertices[m_vertBufferEnd + 18] = 0.0f;
		m_vertices[m_vertBufferEnd + 19] = float(texIndex);

		// Vertex 3
		m_vertices[m_vertBufferEnd + 20] = point[2].x;
		m_vertices[m_vertBufferEnd + 21] = point[2].y;
		m_vertices[m_vertBufferEnd + 22] = point[2].z;

		m_vertices[m_vertBufferEnd + 23] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 24] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 25] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 26] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 27] = 0.0f;
		m_vertices[m_vertBufferEnd + 28] = 1.0f;
		m_vertices[m_vertBufferEnd + 29] = float(texIndex);

		// Vertex 4
		m_vertices[m_vertBufferEnd + 30] = point[3].x;
		m_vertices[m_vertBufferEnd + 31] = point[3].y;
		m_vertices[m_vertBufferEnd + 32] = point[3].z;

		m_vertices[m_vertBufferEnd + 33] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 34] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 35] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 36] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 37] = 1.0f;
		m_vertices[m_vertBufferEnd + 38] = 1.0f;
		m_vertices[m_vertBufferEnd + 39] = float(texIndex);

		m_vertBufferEnd += m_vertSize * 4;
		m_vertCount += 4;

		m_drawType = QUAD;
	}

	void Renderer::AddLine(float x1, float x2, float y1, float y2) {
		if (m_vertCount >= m_maxVerts || m_drawType != LINE) {
			this->DrawBatch();
		}

		int texIndex = -1;
		
		glm::vec4 point[2];

		point[0] = glm::vec4{ x1, x2, 0.0f, 1.0f };
		point[1] = glm::vec4{ y1, y2, 0.0f, 1.0f };

		for (int i = 0; i < 2; i++) {
			point[i] = camera.GetMatrix() * point[i];
		}

		// Vertex 1
		m_vertices[m_vertBufferEnd + 0] = point[0].x;
		m_vertices[m_vertBufferEnd + 1] = point[0].y;
		m_vertices[m_vertBufferEnd + 2] = point[0].z;

		m_vertices[m_vertBufferEnd + 3] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 4] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 5] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 6] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 7] = 0.0f;
		m_vertices[m_vertBufferEnd + 8] = 0.0f;
		m_vertices[m_vertBufferEnd + 9] = float(texIndex);

		// Vertex 2
		m_vertices[m_vertBufferEnd + 10] = point[1].x;
		m_vertices[m_vertBufferEnd + 11] = point[1].y;
		m_vertices[m_vertBufferEnd + 12] = point[1].z;

		m_vertices[m_vertBufferEnd + 13] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 14] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 15] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 16] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 17] = 1.0f;
		m_vertices[m_vertBufferEnd + 18] = 0.0f;
		m_vertices[m_vertBufferEnd + 19] = float(texIndex);

		m_vertBufferEnd += m_vertSize * 2;
		m_vertCount += 2;
		m_drawType = LINE;
	}

	void Renderer::AddLineRect(const Phil::Rect& rect) {
		if (m_vertCount >= m_maxVerts || m_drawType != LINE) {
			this->DrawBatch();
		}

		m_drawType = LINE;

		glm::vec4 point[4];

		point[0] = glm::vec4{ rect.pos.x, rect.pos.y, 0.0f, 1.0f };
		point[1] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y, 0.0f, 1.0f };
		point[2] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };
		point[3] = glm::vec4{ rect.pos.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };
		
		this->AddLine(point[0].x, point[0].y, point[1].x, point[1].y);
		this->AddLine(point[1].x, point[1].y, point[2].x, point[2].y);
		this->AddLine(point[2].x, point[2].y, point[3].x, point[3].y);
		this->AddLine(point[3].x, point[3].y, point[0].x, point[0].y);
	}

	void Renderer::AddLineRect(const Phil::Rect& rect, float angle) {
		if (m_vertCount >= m_maxVerts || m_drawType != LINE) {
			this->DrawBatch();
		}

		m_drawType = LINE;

		glm::mat4 model(1.0f);

		model = glm::translate(model, glm::vec3(rect.pos, 0.0));
		model = glm::translate(model, glm::vec3(0.5 * rect.size.x, 0.5 * rect.size.y, 0.0));
		model = glm::rotate(model, glm::radians(angle) * 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5 * rect.size.x, -0.5 * rect.size.y, 0.0));
		model = glm::translate(model, glm::vec3(-rect.pos, 0.0));

		glm::vec4 point[4];

		point[0] = glm::vec4{ rect.pos.x, rect.pos.y, 0.0f, 1.0f };
		point[1] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y, 0.0f, 1.0f };
		point[2] = glm::vec4{ rect.pos.x + rect.size.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };
		point[3] = glm::vec4{ rect.pos.x, rect.pos.y + rect.size.y, 0.0f, 1.0f };

		for (int i = 0; i < 4; i++) {
			point[i] = model * point[i];
		}

		this->AddLine(point[0].x, point[0].y, point[1].x, point[1].y);
		this->AddLine(point[1].x, point[1].y, point[2].x, point[2].y);
		this->AddLine(point[2].x, point[2].y, point[3].x, point[3].y);
		this->AddLine(point[3].x, point[3].y, point[0].x, point[0].y);
	}

	void Renderer::DrawBatch()
	{
		switch (m_drawType) {
		case QUAD:
			m_VAO.Bind();
			m_VBO.BufferData(sizeof(Vertex) * 4 * m_maxVerts, &m_vertices[0], GL_STATIC_DRAW);
			m_EBO.BufferData(sizeof(unsigned int) * 6 * m_maxVerts, &m_indices[0], GL_STATIC_DRAW);

			m_VBO.VertexAttribPointer(0, m_posSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
			m_VBO.VertexAttribPointer(1, m_colorSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
			m_VBO.VertexAttribPointer(2, m_uvSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, UV));
			m_VBO.VertexAttribPointer(3, m_texIDSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

			m_basicShader.use();
			m_basicShader.set_iv("u_Textures", 32, m_samplerArray);

			for (int i = 0; i < m_maxTexSlots; i++)
			{
				if (m_slottedTexs[i] < 0)
				{
					continue;
				}

				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_slottedTexs[i]);
			}

			glDrawElements(GL_TRIANGLES, 6 * (m_vertCount/4), GL_UNSIGNED_INT, 0);

			m_vertCount = 0;
			m_vertBufferEnd = 0;
			m_texBufferEnd = 0;
			m_VAO.Unbind();

			for (int i = 0; i < m_maxTexSlots; i++)
			{
				m_slottedTexs[i] = -1;
			}
			break;
		case LINE:
			m_VAO.Bind();
			m_VBO.BufferData(sizeof(Vertex) * 4 * m_maxVerts, &m_vertices[0], GL_STATIC_DRAW);

			m_VBO.VertexAttribPointer(0, m_posSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
			m_VBO.VertexAttribPointer(1, m_colorSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
			m_VBO.VertexAttribPointer(2, m_uvSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, UV));
			m_VBO.VertexAttribPointer(3, m_texIDSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

			m_lineShader.use();
			glDrawArrays(GL_LINES, 0, m_vertCount);

			m_vertCount = 0;
			m_vertBufferEnd = 0;
			m_VAO.Unbind();
			break;
		}
	}

	void Renderer::Present() {
		this->DrawBatch();
		SDL_GL_SwapWindow(m_window->GetWindow());
	}

	void Renderer::Clear()
	{
		glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::SetDrawColor(const glm::vec4& color) {
		m_drawColor = color;
	}

	void Renderer::SetClearColor(const glm::vec4& color) {
		m_clearColor = color;
	}

	void Renderer::SetShader(const Shader& shader) {
		m_basicShader = shader;
	}
	
}
