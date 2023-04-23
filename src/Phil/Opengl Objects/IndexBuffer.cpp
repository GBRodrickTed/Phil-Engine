#include "IndexBuffer.h"

namespace Phil
{
	IndexBuffer::IndexBuffer()
	{
		this->buffer_id = 0;
		this->type = GL_ELEMENT_ARRAY_BUFFER;
		glGenBuffers(1, &(this->buffer_id));
	}

	void IndexBuffer::BufferData(GLsizeiptr size, void* data, GLenum usage)
	{
		this->Bind();
		glBufferData(this->type, size, data, usage);
	}

	void IndexBuffer::BufferSubData(GLintptr offset, GLsizeiptr size, const void* data)
	{
		this->Bind();
		glBufferSubData(this->type, offset, size, data);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &(this->buffer_id));
		this->Unbind();
	}

	void IndexBuffer::Bind()
	{
		glBindBuffer(this->type, buffer_id);
	}

	void IndexBuffer::Unbind()
	{
		glBindBuffer(this->type, 0);
		glBindVertexArray(0);
	}

}