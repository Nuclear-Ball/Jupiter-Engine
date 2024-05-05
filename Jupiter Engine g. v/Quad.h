#pragma once
#include <glad/glad.h>


namespace JE {
	namespace QuadData {
		const float vertices[] = {
			1, 1, // top right
			1, 0, // bottom right
			0, 0, // bottom left
			0, 1, // top left 
		};
		const unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};
	}

	class Quad {
	private:
		unsigned int vao = 0;
		unsigned int vbo = 0;
		unsigned int ebo = 0;
	public:
		Quad(bool) {}

		void Init() {
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(QuadData::vertices), QuadData::vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QuadData::indices), QuadData::indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 2, reinterpret_cast<void*>(0));
			glEnableVertexAttribArray(0);
		}

		inline Quad() {
			Init();
		}

		inline ~Quad() {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
		}

		inline unsigned int Handle() const
		{
			return vao;
		}
	};
}

//#include <pos2d.h>
//#include <iostream>
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//#include <vector>

//using namespace std;
/*
const float verts[] = {
 0,  1,        0, 1,
 0,  0,        0, 0,
 1,  0,        1, 0,

 0,  1,        0, 1,
 1,  0,        1, 0,
 1,  1,        1, 1
};*/
/*
const float verts[] = {
-1,  1,        0, 1,
-1, -1,        0, 0,
 1, -1,        1, 0,

-1,  1,        0, 1,
 1, -1,        1, 0,
 1,  1,        1, 1
};
*//*
const float quadVertices[] = {
	-1,  1,        0, 1,
	-1, -1,        0, 0,
	 1, -1,        1, 0,

	-1,  1,        0, 1,
	 1, -1,        1, 0,
	 1,  1,        1, 1
};

struct vertex_attribute {
	int amount_of_attr, type = 0, normalization = GL_FALSE;
};



void generate_vertattibs(vector<vertex_attribute> attr) {
	int pos = 0, bytesize = 0, datatype_length = sizeof(float), datatype = GL_FLOAT;
	int typesizes[] = { sizeof(float), sizeof(bool), sizeof(int), sizeof(unsigned int), sizeof(double) };
	for (int i = 0; i < attr.size(); i++) {
		bytesize += attr[i].amount_of_attr * typesizes[attr[i].type];
	}
	for (int i = 0; i < attr.size(); i++) {
		switch (attr[0].type) {
		case 0: datatype_length = sizeof(float); break;
		case 1: datatype_length = sizeof(bool), datatype = GL_BOOL; break;
		case 2: datatype_length = sizeof(int), datatype = GL_INT; break;
		case 3: datatype_length = sizeof(unsigned int), datatype = GL_UNSIGNED_INT; break;
		default:
			datatype_length = sizeof(double), datatype = GL_DOUBLE; break;
		}
		glVertexAttribPointer(i, attr[i].amount_of_attr, datatype, attr[i].normalization, bytesize, (void*)pos);
		glEnableVertexAttribArray(i);
		pos += attr[i].amount_of_attr * sizeof(float);
	}
}
*/

/*
class vao {
public:
	unsigned int VBO, VAO;
	void initvao() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);
		generate_vertattibs({ {2}, {2} });
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};*/



/*
class rectlnvao {
public:
	int thickness = 1;
	float* coords;
	unsigned int VBO, VAO;
	void init(pos2d start, pos2d end) {
		coords = new float[24];
		coords[0] = float(start.x);
		coords[1] = float(start.y * -1);
		coords[2] = float(0);
		coords[3] = float(0);
		coords[4] = float(start.x + 1);
		coords[5] = float(start.y * -1);
		coords[6] = float(0);
		coords[7] = float(0);
		coords[8] = float(end.x);
		coords[9] = float(end.y * -1);
		coords[10] = float(0);
		coords[11] = float(0);
		coords[12] = float(start.x);
		coords[13] = float(start.y * -1);
		coords[14] = float(0);
		coords[15] = float(0);
		coords[16] = float(end.x);
		coords[17] = float(end.y * -1);
		coords[18] = float(0);
		coords[19] = float(0);
		coords[20] = float(start.x + 1);
		coords[21] = float(start.y * -1);

		coords[0] = float(0);
		coords[1] = float(0);
		coords[2] = float(0);
		coords[3] = float(0);
		coords[4] = float(1);
		coords[5] = float(0);
		coords[6] = float(0);
		coords[7] = float(0);
		coords[8] = float(10);
		coords[9] = float(100);
		coords[10] = float(0);
		coords[11] = float(0);
		coords[12] = float(0);
		coords[13] = float(0);
		coords[14] = float(0);
		coords[15] = float(0);
		coords[16] = float(10);
		coords[17] = float(100);
		coords[18] = float(0);
		coords[19] = float(0);
		coords[20] = float(1);
		coords[21] = float(0);

		vertex_attribute* attr = new vertex_attribute[2];
		attr[0].amount_of_attr = 2, attr[1].amount_of_attr = 2;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, coords, GL_STATIC_DRAW);
		generate_vertattibs(2, attr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void update(pos2d start, pos2d end) {
		float shift_x, shift_y;
		float a = sqrt(pow(abs(start.x - end.x), 2) + pow(abs(start.y - end.y), 2));
		float k = a / float(thickness);
		shift_x = abs(start.x - end.x) / k;
		if (start.x < end.x) {
			shift_y = (start.y - end.y) / k;
		}
		else { 
			shift_y = -1 * (start.y - end.y) / k; 
		}

		vertex_attribute* attr = new vertex_attribute[2];
		attr[0].amount_of_attr = 2, attr[1].amount_of_attr = 2;

		coords[0] = float(end.x);
		coords[1] = float(end.y * -1);
		coords[2] = float(0);
		coords[3] = float(0);
		coords[4] = float(start.x);
		coords[5] = float(start.y * -1);
		coords[6] = float(0);
		coords[7] = float(0);
		coords[8] = float(start.x) + shift_y;
		coords[9] = float(start.y * -1) + shift_x;
		coords[10] = float(0);
		coords[11] = float(0);
		coords[12] = float(end.x);
		coords[13] = float(end.y * -1);
		coords[14] = float(0);
		coords[15] = float(0);
		coords[16] = float(start.x) + shift_y;
		coords[17] = float(start.y * -1) + shift_x;
		coords[18] = float(0);
		coords[19] = float(0);
		coords[20] = float(end.x) + shift_y;
		coords[21] = float(end.y * -1) + shift_x;

		coords[0] = float(0);
		coords[1] = float(0);
		coords[2] = float(0);
		coords[3] = float(0);
		coords[4] = float(1);
		coords[5] = float(0);
		coords[6] = float(0);
		coords[7] = float(0);
		coords[8] = float(10);
		coords[9] = float(100);
		coords[10] = float(0);
		coords[11] = float(0);
		coords[12] = float(0);
		coords[13] = float(0);
		coords[14] = float(0);
		coords[15] = float(0);
		coords[16] = float(10);
		coords[17] = float(100);
		coords[18] = float(0);
		coords[19] = float(0);
		coords[20] = float(1);
		coords[21] = float(0);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, coords, GL_STATIC_DRAW);
		generate_vertattibs(2, attr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};*/

/*
class rectlnvao {
public:
	int thickness = 1;
	vector<float> coords;
	unsigned int VBO, VAO;
	void init(pos2d start, pos2d end) {
		//coords.resize(24);

		coords = {
			float(start.x), float(-start.y),     0, 0,
			float(start.x + 1), float(-start.y), 0, 0,
			float(end.x), float(-end.y),         0, 0,

			float(start.x), float(-start.y),     0, 0,
			float(end.x), float(-end.y),         0, 0,
			float(start.x + 1), float(-start.y), 0, 0
		};

		coords = {
			float(0), float(0),     0, 0,
			float(1), float(0), 0, 0,
			float(10), float(100),         0, 0,

			float(0), float(0),     0, 0,
			float(10), float(100),         0, 0,
			float(1), float(0), 0, 0
		};

		vertex_attribute* attr = new vertex_attribute[2];
		attr[0].amount_of_attr = 2, attr[1].amount_of_attr = 2;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coords.size(), coords.data(), GL_STATIC_DRAW);
		generate_vertattibs(2, attr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void update(pos2d start, pos2d end) {
		float shift_x, shift_y;
		float a = sqrt(pow(abs(start.x - end.x), 2) + pow(abs(start.y - end.y), 2));
		float k = a / float(thickness);
		shift_x = abs(start.x - end.x) / k;
		if (start.x < end.x)
			shift_y = (start.y - end.y) / k;
		else shift_y = -1 * (start.y - end.y) / k;

		vertex_attribute* attr = new vertex_attribute[2];
		attr[0].amount_of_attr = 2, attr[1].amount_of_attr = 2;

		coords = {
			 float(end.x),  float(-end.y),                                     0, 0,
			 float(start.x),  float(-start.y),                                 0, 0,
			 float(start.x) + shift_y, float(-start.y) + shift_x,              0, 0,

			 float(end.x),  float(-end.y),                                     0, 0,
			 float(start.x) + shift_y, float(-start.y) + shift_x,              0, 0,
			 float(end.x) + shift_y, float(-end.y) + shift_x,                  0, 0
		};

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coords.size(), coords.data(), GL_STATIC_DRAW);
		generate_vertattibs(2, attr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};




class rectlnvao {
public:
	int thickness = 1;
	float* coords;
	unsigned int VBO, VAO;
	void init(pos2d start, pos2d end) {
		coords = new float[24];
		coords[0] = float(start.x);
		coords[1] = float(start.y * -1);
		coords[2] = float(0);
		coords[3] = float(0);
		coords[4] = float(start.x + 1);
		coords[5] = float(start.y * -1);
		coords[6] = float(0);
		coords[7] = float(0);
		coords[8] = float(end.x);
		coords[9] = float(end.y * -1);
		coords[10] = float(0);
		coords[11] = float(0);
		coords[12] = float(start.x);
		coords[13] = float(start.y * -1);
		coords[14] = float(0);
		coords[15] = float(0);
		coords[16] = float(end.x);
		coords[17] = float(end.y * -1);
		coords[18] = float(0);
		coords[19] = float(0);
		coords[20] = float(start.x + 1);
		coords[21] = float(start.y * -1);

		coords[0] = float(0);
		coords[1] = float(0);
		coords[2] = float(0);
		coords[3] = float(0);
		coords[4] = float(1);
		coords[5] = float(0);
		coords[6] = float(0);
		coords[7] = float(0);
		coords[8] = float(10);
		coords[9] = float(100);
		coords[10] = float(0);
		coords[11] = float(0);
		coords[12] = float(0);
		coords[13] = float(0);
		coords[14] = float(0);
		coords[15] = float(0);
		coords[16] = float(10);
		coords[17] = float(100);
		coords[18] = float(0);
		coords[19] = float(0);
		coords[20] = float(1);
		coords[21] = float(0);

		vertex_attribute* attr = new vertex_attribute[2];
		attr[0].amount_of_attr = 2, attr[1].amount_of_attr = 2;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, coords, GL_STATIC_DRAW);
		generate_vertattibs(2, attr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void update(pos2d start, pos2d end) {
		float shift_x, shift_y;
		float a = sqrt(pow(abs(start.x - end.x), 2) + pow(abs(start.y - end.y), 2));
		float k = a / float(thickness);
		shift_x = abs(start.x - end.x) / k;
		if (start.x < end.x)
			shift_y = (start.y - end.y) / k;
		else shift_y = -1 * (start.y - end.y) / k;

		vertex_attribute* attr = new vertex_attribute[2];
		attr[0].amount_of_attr = 2, attr[1].amount_of_attr = 2;

		coords[0] = float(end.x);
		coords[1] = float(end.y * -1);
		coords[2] = float(0);
		coords[3] = float(0);
		coords[4] = float(start.x);
		coords[5] = float(start.y * -1);
		coords[6] = float(0);
		coords[7] = float(0);
		coords[8] = float(start.x) + shift_y;
		coords[9] = float(start.y * -1) + shift_x;
		coords[10] = float(0);
		coords[11] = float(0);
		coords[12] = float(end.x);
		coords[13] = float(end.y * -1);
		coords[14] = float(0);
		coords[15] = float(0);
		coords[16] = float(start.x) + shift_y;
		coords[17] = float(start.y * -1) + shift_x;
		coords[18] = float(0);
		coords[19] = float(0);
		coords[20] = float(end.x) + shift_y;
		coords[21] = float(end.y * -1) + shift_x;

		coords[0] = float(0);
		coords[1] = float(0);
		coords[2] = float(0);
		coords[3] = float(0);
		coords[4] = float(1);
		coords[5] = float(0);
		coords[6] = float(0);
		coords[7] = float(0);
		coords[8] = float(10);
		coords[9] = float(100);
		coords[10] = float(0);
		coords[11] = float(0);
		coords[12] = float(0);
		coords[13] = float(0);
		coords[14] = float(0);
		coords[15] = float(0);
		coords[16] = float(10);
		coords[17] = float(100);
		coords[18] = float(0);
		coords[19] = float(0);
		coords[20] = float(1);
		coords[21] = float(0);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, coords, GL_STATIC_DRAW);
		generate_vertattibs(2, attr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};
*/