#include "Shader.h"

namespace Phil {
	
	void Shader::CreateShaderFromFile(const char* vertexPath, const char* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			//open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			//read files buffer contents into stream
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// close file handler
			vShaderFile.close();
			fShaderFile.close();

			//convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure)
		{
			std::cout << "Shader FILE_NO_READ_RIGHT_:(_SAD" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "Shader.h: Error! Vertex No Compile Good: " << vertexPath << "\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "Shader.h: Error! Fragment No Compile Good: " << fragmentPath << "\n" << infoLog << std::endl;
		}

		m_Program = glCreateProgram();
		glAttachShader(m_Program, vertex);
		glAttachShader(m_Program, fragment);
		glLinkProgram(m_Program);

		glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
			std::cout << "Shader.h: Error! Link Fail\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	Shader::~Shader()
	{
		glUseProgram(0);
	}


	void Shader::use()
	{
		glUseProgram(m_Program);
	}

	void Shader::set_b(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_Program, name.c_str()), (int)value);
	}

	void Shader::set_i(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value);
	}

	void Shader::set_iv(const std::string& name, int count, int* value) const
	{
		glUniform1iv(glGetUniformLocation(m_Program, name.c_str()), 2, value);
	}

	void Shader::set_f(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
	}

	void Shader::set_3f(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(m_Program, name.c_str()), x, y, z);
	}

	void Shader::set_4f(const std::string& name, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(m_Program, name.c_str()), x, y, z, w);
	}

	void Shader::set_vec3(const std::string& name, const glm::vec3& vec) const {
		glUniform3f(glGetUniformLocation(m_Program, name.c_str()), vec.x, vec.y, vec.z);
	}

	void Shader::set_vec4(const std::string& name, const glm::vec4& vec) const {
		glUniform4f(glGetUniformLocation(m_Program, name.c_str()), vec.x, vec.y, vec.z, vec.w);
	}

	void Shader::set_mat4(const std::string& name, glm::mat4 mat) const {
		glUniformMatrix4fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
}

