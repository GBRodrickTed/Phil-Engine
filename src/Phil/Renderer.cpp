#include "Renderer.h"

namespace Phil {
	Renderer::Renderer(Phil::Window* window) :m_VBO(GL_ARRAY_BUFFER), m_VBO_scr(GL_ARRAY_BUFFER), m_maxVerts(1000), m_vertCount(0), m_drawColor(glm::vec4(1.0f)), m_clearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
	{
		m_window = window;

		int window_w, window_h;

		SDL_GetWindowSize(window->GetWindow(), &window_w, &window_h);

		camera.SetCameraMat(0, 0, window_w, window_h);
		camera.SetWindowMat(window_w, window_h);

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

		m_texBatchShader.CreateShaderFromString(
			"#version 330 core\n"
			"layout(location = 0) in vec3 a_Pos;\n"
			"layout(location = 1) in vec4 a_Color;\n"
			"layout(location = 2) in vec2 a_UV;\n"
			"layout(location = 3) in float a_TexIndex;\n"
			"\n"
			"out vec3 v_Pos;\n"
			"out vec4 v_Color;\n"
			"out vec2 v_UV;\n"
			"out float v_TexIndex;\n"
			"\n"
			"void main()\n"
			"{\n"
			"\tgl_Position = vec4(a_Pos, 1.0f);\n"
			"\n"
			"\tv_Pos = gl_Position.xyz;\n"
			"\tv_Color = a_Color;\n"
			"\tv_UV = a_UV;\n"
			"\tv_TexIndex = a_TexIndex;\n"
			"}"
			,
			"#version 330 core\n"
			"\n"
			"out vec4 FragColor;\n"
			"\n"
			"in vec3 v_Pos;\n"
			"in vec4 v_Color;\n"
			"in vec2 v_UV;\n"
			"in float v_TexIndex;\n"
			"\n"
			"uniform sampler2D u_Textures[32];\n"
			"uniform vec4 u_Color;\n"
			"\n"
			"void main() {\n"
			"\n"
			"\tint index = int(v_TexIndex);\n"
			"\tvec4 resultColor;\n"
			"\n"
			"\tif (index < 0) {\n"
			"\t\tresultColor = v_Color;\n"
			"\t}\n"
			"\telse {\n"
			"\t\tswitch (index) {\n"
			"\t\tcase 0:\n"
			"\t\t\tresultColor = texture(u_Textures[0], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 1:\n"
			"\t\t\tresultColor = texture(u_Textures[1], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 2:\n"
			"\t\t\tresultColor = texture(u_Textures[2], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 3:\n"
			"\t\t\tresultColor = texture(u_Textures[3], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 4:\n"
			"\t\t\tresultColor = texture(u_Textures[4], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 5:\n"
			"\t\t\tresultColor = texture(u_Textures[5], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 6:\n"
			"\t\t\tresultColor = texture(u_Textures[6], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 7:\n"
			"\t\t\tresultColor = texture(u_Textures[7], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 8:\n"
			"\t\t\tresultColor = texture(u_Textures[8], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 9:\n"
			"\t\t\tresultColor = texture(u_Textures[9], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 10:\n"
			"\t\t\tresultColor = texture(u_Textures[10], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 11:\n"
			"\t\t\tresultColor = texture(u_Textures[11], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 12:\n"
			"\t\t\tresultColor = texture(u_Textures[12], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 13:\n"
			"\t\t\tresultColor = texture(u_Textures[13], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 14:\n"
			"\t\t\tresultColor = texture(u_Textures[14], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 15:\n"
			"\t\t\tresultColor = texture(u_Textures[15], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 16:\n"
			"\t\t\tresultColor = texture(u_Textures[16], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 17:\n"
			"\t\t\tresultColor = texture(u_Textures[17], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 18:\n"
			"\t\t\tresultColor = texture(u_Textures[18], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 19:\n"
			"\t\t\tresultColor = texture(u_Textures[19], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 20:\n"
			"\t\t\tresultColor = texture(u_Textures[20], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 21:\n"
			"\t\t\tresultColor = texture(u_Textures[21], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 22:\n"
			"\t\t\tresultColor = texture(u_Textures[22], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 23:\n"
			"\t\t\tresultColor = texture(u_Textures[23], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 24:\n"
			"\t\t\tresultColor = texture(u_Textures[24], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 25:\n"
			"\t\t\tresultColor = texture(u_Textures[25], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 26:\n"
			"\t\t\tresultColor = texture(u_Textures[26], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 27:\n"
			"\t\t\tresultColor = texture(u_Textures[27], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 28:\n"
			"\t\t\tresultColor = texture(u_Textures[28], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 29:\n"
			"\t\t\tresultColor = texture(u_Textures[29], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 30:\n"
			"\t\t\tresultColor = texture(u_Textures[30], v_UV);\n"
			"\t\t\tbreak;\n"
			"\t\tcase 31:\n"
			"\t\t\tresultColor = texture(u_Textures[31], v_UV);\n"
			"\t\t\tbreak;\n"
			"\n"
			"\t\tdefault:\n"
			"\t\t\tbreak;\n"
			"\t\t}\n"
			"\t}\n"
			"\n"
			"\tFragColor = resultColor;\n"
			"}"
		);
		m_lineBatchShader.CreateShaderFromString(
			"#version 330 core\n"
			"layout(location = 0) in vec3 a_Pos;\n"
			"layout(location = 1) in vec4 a_Color;\n"
			"\n"
			"out vec3 v_Pos;\n"
			"out vec4 v_Color;\n"
			"\n"
			"void main()\n"
			"{\n"
			"\tgl_Position = vec4(a_Pos, 1.0f);\n"
			"\n"
			"\tv_Pos = gl_Position.xyz;\n"
			"\tv_Color = a_Color;\n"
			"}"
			,
			"#version 330 core\n"
			"\n"
			"out vec4 FragColor;\n"
			"\n"
			"in vec3 v_Pos;\n"
			"in vec4 v_Color;\n"
			"\n"
			"void main() {\n"
			"\n"
			"\tvec4 resultColor = v_Color;\n"
			"\n"
			"\tFragColor = resultColor;\n"
			"}"
		);

		m_screenShader.CreateShaderFromString(
			"#version 330 core\n"
			"layout(location = 0) in vec3 a_Pos;\n"
			"layout(location = 1) in vec2 a_UV;\n"
			"\n"
			"out vec3 v_Pos;\n"
			"out vec2 v_UV;\n"
			"\n"
			"void main()\n"
			"{\n"
			"\tgl_Position = vec4(a_Pos, 1.0f);\n"
			"\n"
			"\tv_Pos = gl_Position.xyz;\n"
			"\tv_UV = a_UV;\n"
			"}"
			,
			"#version 330 core\n"
			"\n"
			"out vec4 FragColor;\n"
			"\n"
			"in vec3 v_Pos;\n"
			"in vec2 v_UV;\n"
			"\n"
			"uniform sampler2D u_Texture;\n"
			"\n"
			"void main() {\n"
			"\n"
			"\tFragColor = texture(u_Texture, v_UV);\n"
			"}\n"
			""
		);

		m_screen_vert[0] = 1.0f;
		m_screen_vert[1] = 1.0f;
		m_screen_vert[2] = 1.0f;
		m_screen_vert[3] = 1.0f;

		m_screen_vert[4] = -1.0f;
		m_screen_vert[5] = 1.0f;
		m_screen_vert[6] = 0.0f;
		m_screen_vert[7] = 1.0f;

		m_screen_vert[8] = -1.0f;
		m_screen_vert[9] = -1.0f;
		m_screen_vert[10] = 0.0f;
		m_screen_vert[11] = 0.0f;

		m_screen_vert[12] = 1.0f;
		m_screen_vert[13] = 1.0f;
		m_screen_vert[14] = 1.0f;
		m_screen_vert[15] = 1.0f;

		m_screen_vert[16] = 1.0f;
		m_screen_vert[17] = -1.0f;
		m_screen_vert[18] = 1.0f;
		m_screen_vert[19] = 0.0f;

		m_screen_vert[20] = -1.0f;
		m_screen_vert[21] = -1.0f;
		m_screen_vert[22] = 0.0f;
		m_screen_vert[23] = 0.0f;

		

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

		m_currTex = false;

		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		// generate texture
		glGenTextures(2, m_scrTexture);
		glBindTexture(GL_TEXTURE_2D, m_scrTexture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_w, window_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, m_scrTexture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_w, window_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_scrTexture[m_currTex], 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

	void Renderer::DrawRect(const Phil::Rect& rect, Phil::Shader& shader) {
		if (m_vertCount > 0) {
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

		m_VAO.Bind();
		m_VBO.BufferData(sizeof(Vertex) * 4 * m_maxVerts, &m_vertices[0], GL_STATIC_DRAW);
		m_EBO.BufferData(sizeof(unsigned int) * 6 * m_maxVerts, &m_indices[0], GL_STATIC_DRAW);

		m_VBO.VertexAttribPointer(0, m_posSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
		m_VBO.VertexAttribPointer(1, m_colorSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
		m_VBO.VertexAttribPointer(2, m_uvSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, UV));
		m_VBO.VertexAttribPointer(3, m_texIDSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_scrTexture[m_currTex]);

		shader.Bind();
		shader.set_i("SCREEN_TEXTURE", 0);
		glDrawElements(GL_TRIANGLES, 6 * (m_vertCount / 4), GL_UNSIGNED_INT, 0);
		shader.Unbind();

		m_vertCount = 0;
		m_vertBufferEnd = 0;
		m_texBufferEnd = 0;
		m_VAO.Unbind();

		for (int i = 0; i < m_maxTexSlots; i++)
		{
			m_slottedTexs[i] = -1;
		}

	}

	void Renderer::DrawRect(Texture* texture, const Phil::Rect& rect, Phil::Shader& shader) {
		if (m_vertCount > 0) {
			this->DrawBatch();
		}

		this->SwapFrameBuffer();

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

		m_VAO.Bind();
		m_VBO.BufferData(sizeof(Vertex) * 4 * m_maxVerts, &m_vertices[0], GL_STATIC_DRAW);
		m_EBO.BufferData(sizeof(unsigned int) * 6 * m_maxVerts, &m_indices[0], GL_STATIC_DRAW);

		m_VBO.VertexAttribPointer(0, m_posSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
		m_VBO.VertexAttribPointer(1, m_colorSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
		m_VBO.VertexAttribPointer(2, m_uvSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, UV));
		m_VBO.VertexAttribPointer(3, m_texIDSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

		//this->SwapFrameBuffer();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_scrTexture[!m_currTex]);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

		shader.Bind();
		shader.set_i("SCREEN_TEXTURE", 0);
		glDrawElements(GL_TRIANGLES, 6 * (m_vertCount / 4), GL_UNSIGNED_INT, 0);
		shader.Unbind();

		//this->SwapFrameBuffer();

		m_vertCount = 0;
		m_vertBufferEnd = 0;
		m_texBufferEnd = 0;
		m_VAO.Unbind();
	}

	void Renderer::DrawScreen(Phil::Shader& shader) {
		if (m_vertCount > 0) {
			this->DrawBatch();
		}

		this->SwapFrameBuffer();

		int texIndex = -1;

		glm::vec4 point[4];

		point[0] = glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f };
		point[1] = glm::vec4{ -1.0f, 1.0f, 0.0f, 1.0f };
		point[2] = glm::vec4{ 1.0f, -1.0f, 0.0f, 1.0f };
		point[3] = glm::vec4{ -1.0f, -1.0f, 0.0f, 1.0f };

		// Vertex 1
		m_vertices[m_vertBufferEnd + 0] = point[0].x;
		m_vertices[m_vertBufferEnd + 1] = point[0].y;
		m_vertices[m_vertBufferEnd + 2] = point[0].z;

		m_vertices[m_vertBufferEnd + 3] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 4] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 5] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 6] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 7] = 1.0f;
		m_vertices[m_vertBufferEnd + 8] = 1.0f;
		m_vertices[m_vertBufferEnd + 9] = float(texIndex);

		// Vertex 2
		m_vertices[m_vertBufferEnd + 10] = point[1].x;
		m_vertices[m_vertBufferEnd + 11] = point[1].y;
		m_vertices[m_vertBufferEnd + 12] = point[1].z;

		m_vertices[m_vertBufferEnd + 13] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 14] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 15] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 16] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 17] = 0.0f;
		m_vertices[m_vertBufferEnd + 18] = 1.0f;
		m_vertices[m_vertBufferEnd + 19] = float(texIndex);

		// Vertex 3
		m_vertices[m_vertBufferEnd + 20] = point[2].x;
		m_vertices[m_vertBufferEnd + 21] = point[2].y;
		m_vertices[m_vertBufferEnd + 22] = point[2].z;

		m_vertices[m_vertBufferEnd + 23] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 24] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 25] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 26] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 27] = 1.0f;
		m_vertices[m_vertBufferEnd + 28] = 0.0f;
		m_vertices[m_vertBufferEnd + 29] = float(texIndex);

		// Vertex 4
		m_vertices[m_vertBufferEnd + 30] = point[3].x;
		m_vertices[m_vertBufferEnd + 31] = point[3].y;
		m_vertices[m_vertBufferEnd + 32] = point[3].z;

		m_vertices[m_vertBufferEnd + 33] = m_drawColor.r;
		m_vertices[m_vertBufferEnd + 34] = m_drawColor.g;
		m_vertices[m_vertBufferEnd + 35] = m_drawColor.b;
		m_vertices[m_vertBufferEnd + 36] = m_drawColor.a;

		m_vertices[m_vertBufferEnd + 37] = 0.0f;
		m_vertices[m_vertBufferEnd + 38] = 0.0f;
		m_vertices[m_vertBufferEnd + 39] = float(texIndex);

		m_vertBufferEnd += m_vertSize * 4;
		m_vertCount += 4;

		m_VAO.Bind();
		m_VBO.BufferData(sizeof(Vertex) * 4 * m_maxVerts, &m_vertices[0], GL_STATIC_DRAW);
		m_EBO.BufferData(sizeof(unsigned int) * 6 * m_maxVerts, &m_indices[0], GL_STATIC_DRAW);

		m_VBO.VertexAttribPointer(0, m_posSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
		m_VBO.VertexAttribPointer(1, m_colorSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
		m_VBO.VertexAttribPointer(2, m_uvSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, UV));
		m_VBO.VertexAttribPointer(3, m_texIDSize, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_scrTexture[!m_currTex]);

		shader.Bind();
		shader.set_i("SCREEN_TEXTURE", 0);
		glDrawElements(GL_TRIANGLES, 6 * (m_vertCount / 4), GL_UNSIGNED_INT, 0);
		shader.Unbind();

		m_vertCount = 0;
		m_vertBufferEnd = 0;
		m_texBufferEnd = 0;
		m_VAO.Unbind();
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

			m_texBatchShader.Bind();
			m_texBatchShader.set_iv("u_Textures", 32, m_samplerArray);

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
			m_texBatchShader.Unbind();

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

			m_lineBatchShader.Bind();
			glDrawArrays(GL_LINES, 0, m_vertCount);
			m_texBatchShader.Unbind();

			m_vertCount = 0;
			m_vertBufferEnd = 0;
			m_VAO.Unbind();
			break;
		}
		
	}

	void Renderer::Present() {
		this->DrawBatch();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		

		m_VAO_scr.Bind();
		m_VBO_scr.BufferData(sizeof(float) * 4 * 6, &m_screen_vert[0], GL_STATIC_DRAW);

		m_VBO_scr.VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(0));
		m_VBO_scr.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_scrTexture[m_currTex]);

		m_screenShader.Bind();
		m_screenShader.set_i("u_Texture", 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_screenShader.Unbind();
		m_VAO_scr.Unbind();
		
	}

	void Renderer::Clear()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::SwapFrameBuffer() {
		this->DrawBatch();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_scrTexture[!m_currTex], 0);

		m_VAO_scr.Bind();
		m_VBO_scr.BufferData(sizeof(float) * 4 * 6, &m_screen_vert[0], GL_STATIC_DRAW);

		m_VBO_scr.VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(0));
		m_VBO_scr.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float) * 2));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_scrTexture[m_currTex]);

		m_screenShader.Bind();
		m_screenShader.set_i("u_Texture", 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_screenShader.Unbind();
		m_VAO_scr.Unbind();
		m_currTex = !m_currTex;
	}

	unsigned int Renderer::GetScreenTexture() const {
		return m_scrTexture[m_currTex];
	}

	void Renderer::SetDrawColor(const glm::vec4& color) {
		m_drawColor = color;
	}

	void Renderer::SetClearColor(const glm::vec4& color) {
		m_clearColor = color;
	}

	void Renderer::SetShader(const Shader& shader) {
		m_texBatchShader = shader;
	}
	
}
