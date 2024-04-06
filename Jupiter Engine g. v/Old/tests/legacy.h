#pragma once
#include <Tilemap.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Bits.h>
#include <String+.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>
#include <shader.h>
#include <objects.h>

class Line {
	int shaderProgram;
	unsigned int VBO, VAO;
	vector<float> vertices;
	//glm::vec3 startPoint;
	//glm::vec3 endPoint;
	glm::mat4 MVP;
	glm::vec3 lineColor;
public:
	Line(pos2d start, pos2d end) {

		//startPoint = start;
		//endPoint = end;
		lineColor = glm::vec3(1, 1, 1);
		MVP = glm::mat4(1.0f);

		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec2 aPos;\n"
			"//uniform mat4 MVP;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
			"}\0";
		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"uniform vec3 color;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(color, 1.0f);\n"
			"}\n\0";

		// vertex shader
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors

		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors

		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		vertices = {
			 float(start.x), float(start.y),
			 float(end.x), float(end.y),

		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	int setMVP(glm::mat4 mvp) {
		MVP = mvp;
		return 1;
	}

	int setColor(glm::vec3 color) {
		lineColor = color;
		return 1;
	}

	int draw() {
		glUseProgram(shaderProgram);
		//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &lineColor[0]);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
		return 1;
	}

	~Line() {

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shaderProgram);
	}
};

class testobj {
public:
	float pixels_to_ndc(int size, float pos) {
		return (float)(2.0f / size * pos) - 1.0f;
	}
	testobj() {}
	texture* obj_texture;
	Shader* sh;
	float x = 0.0f, y = 0.0f;
	float rotation = 0, scalex = 0.0f, scaley = 0.0f;
	void init() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		vertex_attribute* attr = new vertex_attribute[2];
		attr[0].amount_of_attr = 2, attr[1].amount_of_attr = 2;
		generate_vertattibs(2, attr);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void render(pos2d screen_size, glm::mat4 projection) {
		sh->use();
		model = glm::translate(model, glm::vec3(x + (obj_texture->size.x + scalex) / 2, y + (obj_texture->size.x + scalex) / 2, 0.0f));
		model = glm::scale(model, glm::vec3((float)(obj_texture->size.x + scalex) / 2.0f, (float)(obj_texture->size.y + scaley) / 2.0f, 1.0f));
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		sh->setMat4("model", model);
		model = glm::mat4(1.0f);
		obj_texture->use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void destroy() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
private:
	unsigned int VBO, VAO;
	//glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
};

/*#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Bits.h>
#include <String+.h>

using namespace std;

SDL_Surface* loadSurface(string path, SDL_Surface* window)
{
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image |%s|! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, window->format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

class sprite {
public:
	string name;
	//point center;
	//int tilt;
	SDL_Rect rect;
	SDL_Texture* texture;
	sprite(string data, SDL_Renderer* renderer, SDL_Surface* window) {
		string path = "";
		rect.w = Merge(data, 0), rect.h = Merge(data, 2), rect.x = 0, rect.y = 0;
		path = cut(data, 4, Find(data, 0, char(1)));
		name = cut(data, Find(data, 4, char(1)) + 1, data.size() - 1);
		texture = SDL_CreateTextureFromSurface(renderer, loadSurface(path, window));
	}
	sprite() {
		name = "", rect.w = 0, rect.h = 0;
	}
	void load(string data, SDL_Renderer* renderer, SDL_Surface* window) {
		string path = "";
		rect.w = Merge(data, 0), rect.h = Merge(data, 2), rect.x = 0, rect.y = 0;
		path = cut(data, 4, Find(data, 0, char(1)));
		name = cut(data, Find(data, 4, char(1)), data.size());
		texture = SDL_CreateTextureFromSurface(renderer, loadSurface(path, window));
	}
};

class Tileset {
public:
	sprite* map = new sprite [0];
	int num;
	void load(string path, SDL_Renderer* renderer, SDL_Surface* window) {
		fstream file(path, ios::in);
		string data, tmp;
		int tmp1 = 2, old_tmp = 0;
		getline(file, data);
		num = Merge(data, 0);
		map = new sprite[num];
		string cell = "", texture = "", name = "";
		for (int i = 0; i < num; i++) {
			old_tmp = tmp1, tmp1 = Find(data, old_tmp, char(0));
			cell = cut(data, old_tmp, tmp1);
			map[i].rect.w = Merge(cell, 0), map[i].rect.h = Merge(cell, 2);
			texture = cut(cell, 1, Find(cell, 0, char(1)) + 1);
			map[i].load(texture, renderer, window);
			map[i].name = cut(cell, Find(cell, 4, char(1)) + 1, cell.size());
		}
		file.close();
	}
	Tileset(string path, SDL_Renderer* renderer, SDL_Surface* window) {
		fstream file(path, ios::in);
		string data, tmp;
		int tmp1 = 2, old_tmp = 0;
		getline(file, data);
		num = Merge(data, 0);
		map = new sprite[num];
		string cell = "", texture = "", name = "";
		for (int i = 0; i < num; i++) {
			old_tmp = tmp1, tmp1 = Find(data, old_tmp, char(0));
			cell = cut(data, old_tmp, tmp1);
			map[i].rect.w = Merge(cell, 0), map[i].rect.h = Merge(cell, 2);
			texture = cut(cell, 1, Find(cell, 0, char(1)) + 1);
			map[i].load(texture, renderer, window);
			map[i].name = cut(cell, Find(cell, 4, char(1)) + 1, cell.size());
		}
		file.close();
	}
	Tileset(){
	}
	void test() {
		cout << map[0].name << " " << map[0].rect.w << endl;
	}
};

void GenerateTilesetSprite(string path) {
	fstream file(path, ios::out);
	cout << "Number of sprites?" << endl;
	int num, width, heigh;
	string filename, name, tmp;
	cin >> num;
	file << Separate(num) << char(0);
	cout << "Width, heigh, filename, name" << endl;
	for (int i = 0; i < num; i++) {
		cin >> width >> heigh >> filename >> name;
		tmp = Separate(width) + Separate(heigh) + filename + char(1) + name + char(0);
		file << tmp;
	}
	file.close();
}
*/

/*
class Sprite {
public:
	//point pos;
	unsigned int texture, VAO, VBO;
	float rotation, scalex = 0.0f, scaley = 0.0f;
	int width, height;
	float x = 0.0f, y = 0.0f;
	void loadtexture(string path) {
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		int nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			//glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			cout << "Failed to load texture" << endl;
		}
		stbi_image_free(data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void init() {
		glBindVertexArray(0);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		vertex_attribute* a = new vertex_attribute[2];
		a[0].amount_of_attr = 2, a[0].type = 0, a[1].amount_of_attr = 2, a[1].type = 0;
		generate_vertattibs(2, a);
	}

	void render(Shader* shader, position screen_size) {

		shader->use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, y, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		model = glm::translate(model, glm::vec3(0.5f * width, 0.5f * height, 0.0f)); // move origin of rotation to center of quad
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
		model = glm::translate(model, glm::vec3(-0.5f * width, -0.5f * height, 0.0f)); // move origin back

		model = glm::scale(model, glm::vec3((width + scalex) / screen_size.x, (height + scaley) / screen_size.y, 1.0f)); // last scale
		shader->setMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};*/