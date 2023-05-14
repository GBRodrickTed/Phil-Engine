#include "VertexBuffer.h"

namespace Phil
{
	VertexBuffer::VertexBuffer()
	{
		this->buffer_id = 0;
		this->type = 0;
	}

	VertexBuffer::VertexBuffer(GLenum type)
	{
		Init(type);
	}

	void VertexBuffer::Init(GLenum type)
	{
		this->buffer_id = 0;
		this->type = type;
		glGenBuffers(1, &(this->buffer_id));
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &(this->buffer_id));
		this->Unbind();
	}


	void VertexBuffer::BufferData(GLsizeiptr size, void* data, GLenum usage)
	{
		this->Bind();
		glBufferData(this->type, size, data, usage);
	}

	void VertexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data)
	{
		this->Bind();
		glBufferSubData(this->type, offset, size, data);
	}

	void VertexBuffer::Bind()
	{
		glBindBuffer(this->type, buffer_id);
	}

	void VertexBuffer::Unbind()
	{
		glBindBuffer(this->type, 0);
	}

	void VertexBuffer::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
	{
		this->Bind();
		glVertexAttribPointer(index, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(index);
	}
}