#pragma once

#include<glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>

class Shader {

private:

	unsigned int m_ID = 0;
public:

	Shader(const char* vertexPath, const char* fragmentPath) {
		int success;
		char infolog[1000];
		std::ifstream vShaderFile, fShaderFile;
		std::stringstream vShaderStream, fShaderStream;
		std::string vertex, fragment;
		const char* vShaderCode, * fShaderCode;

		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vertex = vShaderStream.str();
		fragment = fShaderStream.str();
		vShaderCode = vertex.c_str();
		fShaderCode = fragment.c_str();

		unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShader, 1, &vShaderCode, NULL);
		glCompileShader(vShader);

		glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vShader, 1000, NULL, infolog);
			std::cout << "Vertex Shader Compiling Error!\n" << infolog << '\n';
		}

		unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShader, 1, &fShaderCode, NULL);
		glCompileShader(fShader);

		glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fShader, 1000, NULL, infolog);
			std::cout << "Fragment Shader Compiling Error!\n" << infolog << '\n';
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vShader);
		glAttachShader(m_ID, fShader);
		glLinkProgram(m_ID);

		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_ID, 1000, NULL, infolog);
			std::cout << "Shader Program Linking Error!\n" << infolog << '\n';
		}

		glDeleteShader(vShader);
		glDeleteShader(fShader);
	}

	bool use() {
		if (m_ID == 0)
			return false;
		glUseProgram(m_ID);
		return true;
	}

	bool set3Float(const char* uniformName, float v1, float v2, float v3) {
		int location = glGetUniformLocation(m_ID, uniformName);
		if (location == -1)
			return false;
		glUniform3f(location, v1, v2, v3);
	}

	bool set4Float(const char* uniformName, float v1, float v2, float v3, float v4) {
		int location = glGetUniformLocation(m_ID, uniformName);
		if (location == -1)
			return false;
		glUniform4f(location, v1, v2, v3, v4);
		return true;
	}

	bool set1Float(const char* uniformName, float v) {
		int location = glGetUniformLocation(m_ID, uniformName);
		if (location == -1)
			return false;
		glUniform1f(location, v);
		return true;
	}

	bool set2Float(const char* uniformName, float v1, float v2) {
		int location = glGetUniformLocation(m_ID, uniformName);
		if (location == -1)
			return false;
		glUniform2f(location, v1, v2);
		return true;
	}

	bool setMatrix4f(const char* uniformName, bool Transpose, float* v) {
		int location = glGetUniformLocation(m_ID, uniformName);
		if (location == -1)
			return false;
		glUniformMatrix4fv(location, 1, Transpose, v);
		return true;
	}

	bool setMatrix3f(const char* UniformName, bool Transpose, float* v) {
		int location = glGetUniformLocation(m_ID, UniformName);
		if (location == -1)
			return false;
		glUniformMatrix3fv(location, 1, Transpose, v);
		return true;
	}

	bool set1Int(const char* uniformName, int v) {
		int location = glGetUniformLocation(m_ID, uniformName);
		if (location == -1)
			return false;
		glUniform1i(location, v);
		return true;
	}
};