#pragma once
#include <Tilemap.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>
#include <shader.h>
//#include <filewrite.h>
#include <texture_generated.h>
#include <locale>
#include <codecvt>
#include <filesystem>
#include <camera.h>


using namespace std;

namespace je_core {



	vector <float> verts = {
	-1,  1,        0, 1,
	-1, -1,        0, 0,
	 1, -1,        1, 0,

	-1,  1,        0, 1,
	 1, -1,        1, 0,
	 1,  1,        1, 1
	};

	float quadVertices[] = {
		-1,  1,        0, 1,
		-1, -1,        0, 0,
		 1, -1,        1, 0,

		-1,  1,        0, 1,
		 1, -1,        1, 0,
		 1,  1,        1, 1
	};

	enum folder_types {
		JE_texture,
		JE_tileset,
		JE_prefab
	};

	struct vertex_attribute {
		int amount_of_attr, type = 0, normalization = GL_FALSE;
	};


	void generate_vertattibs(int amount, vertex_attribute* attr) {
		int pos = 0, bytesize = 0, datatype_length = sizeof(float), datatype = GL_FLOAT;
		for (int i = 0; i < amount; i++) {
			switch (attr[i].type)
			{
			case 0: datatype_length = sizeof(float); break;
			case 1: datatype_length = sizeof(bool); break;
			case 2: datatype_length = sizeof(int); break;
			case 3: datatype_length = sizeof(unsigned int); break;
			default:
				datatype_length = sizeof(double); break;
			}
			bytesize += attr[i].amount_of_attr * datatype_length;
		}
		for (int i = 0; i < amount; i++) {
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

	class vao {
	public:
		unsigned int VBO, VAO;
		vao() {
			vertex_attribute* attr = new vertex_attribute[2];
			attr[0].amount_of_attr = 2, attr[1].amount_of_attr = 2;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(), GL_STATIC_DRAW);
			generate_vertattibs(2, attr);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	};


	class rectlnvao {
	public:
		int thickness = 1;
		vector<float> coords;
		unsigned int VBO, VAO;
		void init(pos2d start, pos2d end) {
			//coords.resize(24);

			coords = {
				float(start.x), float(start.y),     0, 0,
				float(start.x + 1), float(start.y), 0, 0,
				float(end.x), float(end.y),         0, 0,

				float(start.x), float(start.y),     0, 0,
				float(end.x), float(end.y),         0, 0,
				float(start.x + 1), float(start.y), 0, 0
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
			//cout << "shift: x:" << shift_x << " y:" << shift_y << " k:" << k << " a:" << a << endl;
			vertex_attribute* attr = new vertex_attribute[2];
			attr[0].amount_of_attr = 2, attr[1].amount_of_attr = 2;

			coords = {
				 float(end.x),  float(end.y),                                     0, 0,
				 float(start.x),  float(start.y),                                 0, 0,
				 float(start.x) + shift_y, float(start.y) + shift_x,              0, 0,

				 float(end.x),  float(end.y),                                     0, 0,
				 float(start.x) + shift_y, float(start.y) + shift_x,              0, 0,
				 float(end.x) + shift_y, float(end.y) + shift_x,                  0, 0
			};

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coords.size(), coords.data(), GL_STATIC_DRAW);
			generate_vertattibs(2, attr);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	};

	class texture {
	public:
		string pth;
		string dat;
		u16string name = u"Без имени";
		pos2d size;
		unsigned int id;
		/*
		void loadfromstr(u16string str, pos2d sizen) {
			size = sizen;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			string tmp(str.begin(), str.end());
			const unsigned char* data = reinterpret_cast<const unsigned char*> (tmp.c_str());
			if (data)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else cout << "Failed to load texture" << endl;
			glBindTexture(GL_TEXTURE_2D, 0);
			//string tmp1((char*)data);
		}*/

		void loadfromstr() {
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			if (dat.c_str())
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, dat.c_str());
			else cout << "Failed to load texture" << endl;
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void loadtexture(string path) {
			u16string tmp1;
			pth = path;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			int nrChannels;
			unsigned char* data = stbi_load(path.c_str(), &size.x, &size.y, &nrChannels, 0);
			char* tmp = reinterpret_cast<char*>(data);
			dat.resize(size.x * size.y * 4);
			for (int i = 0; i < size.x * size.y * 4; i++) {
				dat[i] = tmp[i];
			}
			if (data)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp);
			}
			else
			{
				cout << "Failed to load texture" << endl;
			}

			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);

		}

		void use() {
			glBindTexture(GL_TEXTURE_2D, id);
		}
	};

	class je_file {
	public:
		fstream file;

		const Engine::Scene* scne;
		flatbuffers::FlatBufferBuilder builder;

		void savescene(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Engine::Texture>>> txts) {
			auto scene = Engine::CreateScene(builder, txts);
			builder.Finish(scene);
			file.write((char*)builder.GetBufferPointer(), builder.GetSize());
			file.close();
		}

		flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<Engine::Texture>>> save_textures(vector<je_core::texture> txts) {
			vector<flatbuffers::Offset<Engine::Texture>> texture_vec;
			for (int i = 0; i < txts.size(); i++) {
				auto tmp_texture = Engine::CreateTexture(builder, builder.CreateString(wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(txts[i].name)), builder.CreateString(txts[i].dat), txts[i].size.x, txts[i].size.y);
				texture_vec.push_back(tmp_texture);
			}
			return builder.CreateVector(texture_vec);
		}

		void openscene(u16string path) {
			file.open(filesystem::path(path), ios::in | ios::binary);
			file.seekg(0, std::ios::end);
			int length = file.tellg();
			file.seekg(0, std::ios::beg);
			char* data = new char[length];
			file.read(data, length);
			file.close();
			scne = Engine::GetScene(data);
		}

		vector<je_core::texture> get_textures() {
			vector<je_core::texture> txts;
			je_core::texture tmp_texture;

			int vecsize = scne->textures()->size();
			for (int i = 0; i < vecsize; i++) {
				tmp_texture.dat = scne->textures()->Get(i)->load_str()->str();
				tmp_texture.name = wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t >{}.from_bytes(scne->textures()->Get(i)->name()->str());
				tmp_texture.size.x = scne->textures()->Get(i)->width();
				tmp_texture.size.y = scne->textures()->Get(i)->height();
				txts.push_back(tmp_texture);
			}
			return txts;
		}
	};

	struct t_tile {
		int id;
		float rotation = 0, scalex = 0, scaley = 0;
		pos2d center;
	};

	class tileset {
	public:
		vector<t_tile> tiles;
		void add(int id) {
			tiles.resize(tiles.size() + 1);
			tiles[tiles.size() - 1].id = id;
		}
	};

	struct folder_accesser {
		vector<int> folder_ids;
		int item_id;
	};

	struct folder_item {
		u16string name;
		int id;
		folder_types type;
	};

	struct folder {
		u16string name;
		vector<folder> folders;
		vector<folder_item> items;
	};

	enum type
	{
		layer,
		tset,
		camera,
		pfab,
		txtre,
		jroom
	};

	class prefab {
	public:
		int txt_id;
		pos2d default_origin;
		void init(int id) {
			txt_id = id;
		}
	};

	class object {
	public:
		pos2d origin;
		pos2d pos;
		int pfab_id = 0, pfolder_id = 0;
		float rotation = 0, scalex = 0, scaley = 0;
	};

	class Layer {
	public:
		u16string name = u"Без имени";
		vecmap tmap;
		vector<object> objs;

		void init_tmap(pos2d size) {
			tmap.cnew(size.x, size.y);
		}

		void add_obj(pos2d pos, int pfab_id, int pfolder_id = 0) {
			objs.resize(objs.size() + 1);
			objs[objs.size() - 1].pfab_id = pfab_id, objs[objs.size() - 1].pfolder_id = pfolder_id, objs[objs.size() - 1].pos = pos;
		}

		void remove_obj(int id) {
			objs.erase(objs.begin() + id, objs.end() - (objs.size() - id) + 1);
		}
	};

	class room {
	public:
		vector<Layer> Layers;
		vector<cam2d> cams;
		int selecteed_cam = 0;
		void add_layers(int amount = 1) {
			Layers.resize(Layers.size() + amount);
		}
	};

	class Scene {
	public:
		//vector<prefab> prefs;

		vector<room> rooms;
		vector<prefab> prefabs;
		vector<texture> textures;
		vector<tileset> tsets;
		Shader sh;
		folder je_filesystem;
		int current_room = 0;

		void save_scene(u16string path) {
			je_file jfile;
			jfile.file.open(filesystem::path(path), ios::out | ios::binary);
			jfile.savescene(jfile.save_textures(textures));
		}

		void load_scene(u16string path) {
			je_file jfile;
			jfile.openscene(path);
			textures = jfile.get_textures();
			for (int i = 0; i < textures.size(); i++)
				textures[i].loadfromstr();
		}

		folder* get_folder(folder_accesser accesser) {
			folder* tmp = &je_filesystem;
			for (int i : accesser.folder_ids) {
				tmp = &tmp->folders[i];
			}
			return tmp;
		}

		void init(pos2d pos1, pos2d pos2) {
			LnVAO.init(pos1, pos2);
		}

		void drawrect(pos2d start, pos2d end) {
			pos2d pos1, pos2;
			sh.setInt("isLine", 1);
			sh.setMat4("model", model);

			pos1.x = start.x, pos1.y = start.y, pos2.x = start.x, pos2.y = end.y;
			LnVAO.update(pos1, pos2);
			glBindVertexArray(LnVAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			pos1 = pos2, pos2.x = end.x, pos2.y = end.y;
			LnVAO.update(pos1, pos2);
			glBindVertexArray(LnVAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			pos1 = pos2, pos2.x = end.x, pos2.y = start.y;
			LnVAO.update(pos1, pos2);
			glBindVertexArray(LnVAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			pos1 = pos2, pos2.x = start.x, pos2.y = start.y;
			LnVAO.update(pos1, pos2);
			glBindVertexArray(LnVAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			sh.setInt("isLine", 0);
		}

		void drawtexture(pos2d pos, int txt_id, int origin_x = 0, int origin_y = 0, int scale_x = 0, int scale_y = 0, int rotation = 0) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
			model = glm::translate(model, glm::vec3(origin_x, origin_y, 0.0f));
			model = glm::rotate(model, glm::radians((float)rotation), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-1 * origin_x, -1 * origin_y, 0.0f));
			model = glm::scale(model, glm::vec3((textures[txt_id].size.x + scale_x) / 2, (textures[txt_id].size.y + scale_y) / 2, 1.0f));
			sh.setMat4("model", model);
			sh.setFloat("transparency", 0.5f);
			textures[txt_id].use();
			glBindVertexArray(VAO.VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			model = glm::mat4(1.0f);
			sh.setFloat("transparency", 0);
		}

		void setvecs(pos2d screen_size) {
			sh.use();
			sh.setMat4("view", rooms[current_room].cams[rooms[current_room].selecteed_cam].set_vec(screen_size));
			projection = glm::ortho(0.0f, (float)screen_size.x, (float)screen_size.y, 0.0f, -1.0f, 1.0f);
			sh.setMat4("projection", projection);
		}

		void add(type a, int amount = 1) {
			switch (a) {
			case tset:
				tsets.resize(tsets.size() + amount); break;
			case pfab:
				prefabs.resize(prefabs.size() + amount); break;
			case txtre:
				textures.resize(textures.size() + amount); break;
			case jroom:
				rooms.resize(rooms.size() + amount); break;
			}
		}

		void render(pos2d screen_size) {
			sh.use();

			for (int i = 0; i < rooms[current_room].Layers.size(); i++) {
				t_tile curr_tile;
				cam2d curr_cam = rooms[current_room].cams[rooms[current_room].selecteed_cam];
				for (int x = (curr_cam.center.x - screen_size.x / 2 / curr_cam.zoom) / rooms[current_room].Layers[i].tmap.tileSize; x < ((curr_cam.center.x + screen_size.x / 2 / curr_cam.zoom) / rooms[current_room].Layers[i].tmap.tileSize + 1); x++) { //for (int x = cams[selected_cam].pos.x / Layers[i].tmap.tileSize / cams[selected_cam].zoom; x < ((cams[selected_cam].pos.x + screen_size.x) / Layers[i].tmap.tileSize + 1) / cams[selected_cam].zoom; x++) {
					for (int y = (curr_cam.center.y - screen_size.y / 2 / curr_cam.zoom) / rooms[current_room].Layers[i].tmap.tileSize; y < ((curr_cam.center.y + screen_size.y / 2 / curr_cam.zoom) / rooms[current_room].Layers[i].tmap.tileSize + 1); y++) { //for (int y = cams[selected_cam].pos.y / Layers[i].tmap.tileSize / cams[selected_cam].zoom; y < ((cams[selected_cam].pos.y + screen_size.y) / Layers[i].tmap.tileSize + 1) / cams[selected_cam].zoom; y++) {
						if (rooms[current_room].Layers[i].tmap.gTile(x, y).tile > -1) {
							curr_tile = tsets[rooms[current_room].Layers[i].tmap.gTile(x, y).tset].tiles[rooms[current_room].Layers[i].tmap.gTile(x, y).tile];
							model = glm::translate(model, glm::vec3(x * rooms[current_room].Layers[i].tmap.tileSize + textures[curr_tile.id].size.x / 2 + rooms[current_room].Layers[i].tmap.offset.x, y * rooms[current_room].Layers[i].tmap.tileSize + textures[curr_tile.id].size.y / 2 + rooms[current_room].Layers[i].tmap.offset.y, 0.0f));
							model = glm::translate(model, glm::vec3(curr_tile.center.x, curr_tile.center.y, 0.0f));
							model = glm::rotate(model, glm::radians((float)curr_tile.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
							model = glm::translate(model, glm::vec3(-1 * curr_tile.center.x, -1 * curr_tile.center.y, 0.0f));

							model = glm::scale(model, glm::vec3((textures[curr_tile.id].size.x + curr_tile.scalex) / 2, (textures[curr_tile.id].size.y + curr_tile.scaley) / 2, 1.0f)); // last scale
							sh.setMat4("model", model);
							model = glm::mat4(1.0f);
							textures[curr_tile.id].use();
							glBindVertexArray(VAO.VAO);
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
					}
				}
				for (int obj_c = 0; obj_c < rooms[current_room].Layers[i].objs.size(); obj_c++) {
					object curr_obj = rooms[current_room].Layers[i].objs[obj_c];
					prefab curr_pfab = prefabs[curr_obj.pfab_id];

					model = glm::translate(model, glm::vec3(curr_obj.pos.x, curr_obj.pos.y, 0.0f));
					//model = glm::translate(model, glm::vec3(tf[prefs[curr_obj.pfab_id].folder].textures[prefs[curr_obj.pfab_id].txt_id].size.x / 2, tf[prefs[curr_obj.pfab_id].folder].textures[prefs[curr_obj.pfab_id].txt_id].size.y / 2, 0.0f));
					model = glm::scale(model, glm::vec3(textures[curr_pfab.txt_id].size.x + curr_obj.scalex / 2, (textures[curr_pfab.txt_id].size.y + curr_obj.scalex) / 2, 1.0f));
					sh.setMat4("model", model);
					model = glm::mat4(1.0f);
					textures[curr_pfab.txt_id].use();
					glBindVertexArray(VAO.VAO);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
				pos2d pos1, pos2;
				pos1.x = 0, pos1.y = 0, pos2.x = rooms[current_room].Layers[i].tmap.gwidth() * 16, pos2.y = rooms[current_room].Layers[i].tmap.gheigh() * 16;
				drawrect(pos1, pos2);
			}
		}



	private:
		vao VAO;
		rectlnvao LnVAO;
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
	};

	class core {
		Scene sc;
		texture err_txt;

	};

}


/*class lnvao {
public:
	vector<float> coords;
	unsigned int VBO, VAO;
	void init(pos2d start, pos2d end) {
		coords.resize(4);
		coords[0] = start.x, coords[1] = start.y, coords[2] = end.x, coords[3] = end.y;
		vertex_attribute* attr = new vertex_attribute[1];
		attr[0].amount_of_attr = 2;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coords.size(), coords.data(), GL_STATIC_DRAW);
		generate_vertattibs(1, attr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void update(pos2d start, pos2d end) {
		coords[0] = start.x, coords[1] = start.y, coords[2] = end.x, coords[3] = end.y;
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		cout << sizeof(float) * coords.size();
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * coords.size(), coords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};
*/