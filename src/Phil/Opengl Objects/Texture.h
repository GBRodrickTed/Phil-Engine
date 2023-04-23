#pragma once

#include <GLAD/glad.h>
#include "GLDebug.h"
#include "stb_image.h"

#include <string>

namespace Phil
{
	class Texture
	{
	public:
		Texture(std::string path, int internalformat = GL_RGBA, unsigned int type = GL_TEXTURE_2D,
			unsigned int min_filter = GL_NEAREST, unsigned int mag_filter = GL_NEAREST,
			unsigned int texwrap_s = GL_CLAMP, unsigned int texwrap_t = GL_CLAMP);

		Texture() {
			m_texture = new unsigned int;
			m_texture = 0;
			m_width = 0;
			m_height = 0;
			m_type = GL_TEXTURE_2D;
			m_format = GL_RGBA;
			m_bits = 0;
			m_canDelete = true;
		}

		~Texture()
		{
			if (this->m_canDelete == 1 && this->m_texture != nullptr)
			{
				glDeleteTextures(1, m_texture);
				m_texture = nullptr;
			}
		}

		inline int GetWidth() const
		{
			return m_width;
		}

		inline int GetHeight() const
		{
			return m_height;
		}

		inline void Bind(int slot = 0) const
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(this->m_type, *(this->m_texture));
		}

		inline void Unbind() const
		{
			glBindTexture(m_type, 0);
		}

		inline unsigned int GetTextureID() const
		{
			return *m_texture;
		};

		void CreateTexture(std::string path,
			int internalformat, unsigned int type,
			unsigned int min_filter, unsigned int mag_filter,
			unsigned int texwrap_s, unsigned int texwrap_t);

	private:
		int m_width;
		int m_height;
		int m_bits;
		
		int m_format;
		unsigned int* m_texture;
		unsigned int m_type;

		int m_canDelete;
	};
}