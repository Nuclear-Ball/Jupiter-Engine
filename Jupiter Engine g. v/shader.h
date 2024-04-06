#pragma once
#include <glad/glad.h>
#include <glm.hpp>
//#include <fstream>
#include <iostream>


const std::string texture_shader_v = "#version 330 core\nlayout(location = 0) in vec2 aPos;\nuniform mat4 model;\nuniform mat4 projection;\nuniform mat4 view;\nout vec2 TexCoord;\nvoid main(){\ngl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);\nTexCoord = aPos;}";
const std::string texture_shader_f = "#version 330 core\nout vec4 FragColor;\nin vec2 TexCoord;\nuniform float transparency;\nuniform sampler2D ourTexture;\nvoid main(){\nvec4 color = texture(ourTexture, TexCoord);\ncolor.w = color.w - transparency;\nFragColor = color;}";

const std::string flatcolor_shader_v = "#version 330 core\nlayout(location = 0) in vec2 aPos;\nuniform mat4 model;\nuniform mat4 projection;\nuniform mat4 view;\nout vec2 TexCoord;\nvoid main(){\ngl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);}";
const std::string flatcolor_shader_f = "#version 330 core\nout vec4 FragColor;\nin vec2 TexCoord;\nuniform vec4 flat_color;\nvoid main(){\nFragColor = flat_color;}";

char deb_log[1024];

class JShader {
private:
	unsigned int handle;
public:
	JShader(){}
	inline JShader(const std::string& vertex, const std::string& fragment) 
		{ Load(vertex, fragment); }

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
/*
class Shader {
public:
    unsigned int id;
    string vsrc, fsrc;
    Shader() {}

    Shader(string vertexPath, string fragmentPath) {
        string vertexCode, fragmentCode;
        ifstream vShaderFile, fShaderFile;
        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        try{
            
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            vsrc = vertexCode;
            fsrc = fragmentCode;
        }
        catch (ifstream::failure& e){
            cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);
        checkCompileErrors(id, "PROGRAM");
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void from_string() {
        const char* vShaderCode = vsrc.c_str();
        const char* fShaderCode = fsrc.c_str();
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);
        checkCompileErrors(id, "PROGRAM");
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void createShader(string vertexPath, string fragmentPath) {
        string vertexCode, fragmentCode;
        ifstream vShaderFile, fShaderFile;
        vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        try{
            
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();*/
            /*
            vShaderFile.open(filesystem::path(vertexPath), ios::binary);
            fShaderFile.open(filesystem::path(fragmentPath), ios::binary);

            vShaderFile.seekg(0, std::ios::end);
            int lengthv = vShaderFile.tellg();
            vShaderFile.seekg(0, std::ios::beg);
            char* datav = new char[lengthv];
            vShaderFile.read(datav, lengthv);

            fShaderFile.seekg(0, std::ios::end);
            int lengthf = fShaderFile.tellg();
            fShaderFile.seekg(0, std::ios::beg);
            char* dataf = new char[lengthf];
            fShaderFile.read(dataf, lengthf);
            
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = datav;
            fragmentCode = dataf;*//*
            vsrc = vertexCode;
            fsrc = fragmentCode;
        }
        catch (ifstream::failure& e){
            cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);
        checkCompileErrors(id, "PROGRAM");
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Use() const{
        glUseProgram(id);
    }

    void setBool(const string& name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }
    
    void setInt(const string& name, int value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }
    
    void setFloat(const string& name, float value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }
    
    void setVec2(const string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec2(const string& name, float x, float y) const {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }
    
    void setVec3(const string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec3(const string& name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }
    
    void setVec4(const string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }
    void setVec4(const string& name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }

    void setMat2(const string& name, const glm::mat2& mat) const {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    void setMat3(const string& name, const glm::mat3& mat) const {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    void setMat4(const string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    void checkCompileErrors(GLuint shader, string type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
            }
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
            }
        }
    }
};*/