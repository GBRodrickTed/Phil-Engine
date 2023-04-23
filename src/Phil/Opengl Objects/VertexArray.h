#pragma once

#include <GLAD/glad.h>

namespace Phil
{

	class VertexArray
	{
	public:

		VertexArray();
		~VertexArray();

		void Bind();
		void Unbind();

	private:

		GLuint array_id;
	};
}