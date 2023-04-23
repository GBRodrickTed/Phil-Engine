#pragma once

#include <GLAD/glad.h>

namespace Phil
{
	class IndexBuffer
	{
	public:

		IndexBuffer();
		~IndexBuffer();

		void BufferData(GLsizeiptr size, void* data, GLenum usage);
		void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data);
		void Bind();
		void Unbind();


	private:

		GLuint buffer_id;
		GLenum type;
	};

}