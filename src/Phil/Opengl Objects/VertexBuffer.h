#pragma once

#include <GLAD/glad.h>
//#include <GLFW/glfw3.h>
//#include <SDL2/SDL.h>

namespace Phil
{
	class VertexBuffer
	{
	public:
		VertexBuffer(GLenum type);
		VertexBuffer();
		~VertexBuffer();

		void Init(GLenum type);
		void BufferData(GLsizeiptr size, void* data, GLenum usage);
		void BufferSubData(GLintptr offset, GLsizeiptr size, const void* data);
		void Bind();
		void Unbind();
		void VertexAttribPointer(GLuint index, GLint size, GLenum type,
			GLboolean normalized, GLsizei stride, const GLvoid* pointer);

		
		GLenum type;
	private:
		
		GLuint buffer_id;
	};
}