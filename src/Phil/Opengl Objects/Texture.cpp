#include "Texture.h"

namespace Phil
{
	Texture::Texture(std::string path,
		int internalformat, unsigned int type,
		unsigned int min_filter, unsigned int mag_filter,
		unsigned int texwrap_s, unsigned int texwrap_t) 
	{
		this->CreateTexture(path, internalformat, type, min_filter, mag_filter, texwrap_s, texwrap_t);
	}

	void Texture::CreateTexture(std::string path,
		int internalformat, unsigned int type,
		unsigned int min_filter, unsigned int mag_filter,
		unsigned int texwrap_s, unsigned int texwrap_t)
	{
		stbi_set_flip_vertically_on_load(false);

		m_canDelete = true;
		m_texture = new unsigned int;
		m_type = type;
		m_format = internalformat;

		unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_bits, 4);

		if (!data)
		{
			std::cout << "Failed to load texture: " << path << std::endl;
		}

		glGenTextures(1, m_texture);
		glBindTexture(m_type, *m_texture);
		
		glTexParameteri(m_type, GL_TEXTURE_WRAP_S, texwrap_s);
		glTexParameteri(m_type, GL_TEXTURE_WRAP_T, texwrap_t);

		glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, min_filter);
		glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, mag_filter);

		glTexImage2D(m_type, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(m_type);

		if (data) {
			stbi_image_free(data);
		}
	}
}