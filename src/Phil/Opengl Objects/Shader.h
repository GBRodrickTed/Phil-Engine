#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Phil {
	class Shader
	{
	public:
		Shader(const char* vertexPath, const char* fragmentPath) {
			CreateShaderFromFile(vertexPath, fragmentPath);
		}

		Shader(){}

		~Shader();

		void Bind();
		void Unbind();
		void CreateShaderFromFile(const char* vertexPath, const char* fragmentPath);
		void CreateShaderFromString(const char* vertexCode, const char* fragmentCode);
		void set_b(const std::string& name, bool value) const;
		void set_i(const std::string& name, int value) const;
		void set_iv(const std::string& name, int count, int* value) const;
		void set_f(const std::string& name, float value) const;
		void set_3f(const std::string& name, float x, float y, float z) const;
		void set_4f(const std::string& name, float x, float y, float z, float w) const;
		void set_vec3(const std::string& name, const glm::vec3& vec) const;
		void set_vec4(const std::string& name, const glm::vec4& vec) const;
		void set_mat4(const std::string& name, glm::mat4 mat) const;
	
	private:
		unsigned int m_Program;
	};
}


#endif