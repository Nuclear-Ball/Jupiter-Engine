#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
//#include <fstream>
#include <iostream>

/*
const std::string texture_shader_v = "#version 330 core\n"
									 "layout(location = 0) in vec2 aPos;\n"

									 "uniform mat4 model;\n"
									 "uniform mat4 pv;\n"

									 "out vec2 TexCoord;\n"

									 "void main(){\n"
										 "gl_Position = pv * model * vec4(aPos, 0.0, 1.0);\n"
										 "TexCoord = aPos;\n"
									 "}";
*/
const std::string common_vs =	"#version 330 core\n"
								"layout(location = 0) in vec2 aPos;\n"

								"uniform mat4 model;\n"
								"uniform mat4 pv;\n"

								"out vec2 TexCoord;\n"

								"void main(){\n"
									"gl_Position = pv * model * vec4(aPos, 0.0, 1.0);\n"
									"TexCoord = aPos;\n"
								"}";


const std::string texture_shader_f = "#version 330 core\n"

									 "out vec4 FragColor;\n"
									 "in vec2 TexCoord;\n"

									 "uniform float transparency;\n"
									 "uniform sampler2D ourTexture;\n"

									 "void main(){\n"
										 "vec4 color = texture(ourTexture, vec2(TexCoord.x, -TexCoord.y));\n"
										 "FragColor = vec4(color.rgb, color.a - transparency);\n"
									 "}";

const std::string flatcolor_shader_f = "#version 330 core\n"

									   "out vec4 FragColor;\n"

									   "uniform vec4 flat_color;\n"

									   "void main(){\n"
										   "FragColor = flat_color;\n"
									   "}";

char deb_log[1024];

class JShader {
private:
	unsigned int handle = 0;
public:
	JShader(){}
	inline JShader(const std::string& vertex, const std::string& fragment) 
		{ Load(vertex, fragment); }

	JShader(const JShader&) = delete;
	JShader& operator=(const JShader&) = delete;

	inline unsigned int Handle() const 
		{ return handle; }
	void Load(const std::string& vertex, const std::string& fragment) {
		int success = 0;

		const char* tmp = vertex.c_str();
		unsigned int vertex_handle = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_handle, 1, &tmp, nullptr);
		glCompileShader(vertex_handle);

		glGetShaderiv(vertex_handle, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex_handle, 1024, nullptr, deb_log);
			std::cout << "Shader compilation failed: \n" << deb_log;
			std::abort();
		}

		tmp = fragment.c_str();
		unsigned int fragment_handle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_handle, 1, &tmp, nullptr);
		glCompileShader(fragment_handle);

		glGetShaderiv(fragment_handle, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment_handle, 1024, nullptr, deb_log);
			std::cout << "Shader compilation failed: \n" << deb_log;
			std::abort();
		}

		handle = glCreateProgram();
		glAttachShader(handle, vertex_handle);
		glAttachShader(handle, fragment_handle);
		glLinkProgram(handle);

		glGetProgramiv(handle, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(handle, 1024, nullptr, deb_log);
			std::cout << "shader program linking failed: \n" << deb_log;
			std::abort();
		}

		glDeleteShader(vertex_handle);
		glDeleteShader(fragment_handle);
	}

	inline void Use() const {
		glUseProgram(handle); }
	inline void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(handle, name.c_str()), (int)value); }
	inline void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(handle, name.c_str()), value); }
	inline void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(handle, name.c_str()), value); }
	inline void setVec2(const std::string& name, const glm::vec2& value) const {
		glUniform2fv(glGetUniformLocation(handle, name.c_str()), 1, &value[0]); }
	inline void setVec2(const std::string& name, float x, float y) const {
		glUniform2f(glGetUniformLocation(handle, name.c_str()), x, y); }
	inline void setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(handle, name.c_str()), 1, &value[0]); }
	inline void setVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(handle, name.c_str()), x, y, z); }
	inline void setVec4(const std::string& name, const glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation(handle, name.c_str()), 1, &value[0]); }
	inline void setVec4(const std::string& name, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(handle, name.c_str()), x, y, z, w); }
	inline void setMat2(const std::string& name, const glm::mat2& mat) const {
		glUniformMatrix2fv(glGetUniformLocation(handle, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
	inline void setMat3(const std::string& name, const glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation(handle, name.c_str()), 1, GL_FALSE, &mat[0][0]); }
	inline void setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(handle, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

	inline ~JShader() 
		{ glDeleteProgram(handle); }
};