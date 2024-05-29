#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include <Tilemap.h>
#include <texture.h>
#include <pos2d.h>

namespace JE {
	namespace WorldTypes {
		namespace SerializationData {
			namespace LR {
				const JSerial::Template _template = { JSerial::Type<std::string>("name"),
													  JSerial::Type<std::string>("tmap"),
													  JSerial::Type<bool>("hidden"),
													  JSerial::Type<float>("transparency"),
													  JSerial::Type<pos2df>("parallax speed") };
				JSerial::Write writer(_template);
				JSerial::Read reader(_template);
			}
			namespace RM {
				const JSerial::Template _template = { JSerial::Type<rgb>("background color"),
													  JSerial::Type<std::string>("name"),
													  JSerial::Array<std::string>("layers"),
													  JSerial::Type<pos2d>("chunksize"),
													  JSerial::Type<pos2d>("camerapos") };
				JSerial::Write writer(_template);
				JSerial::Read reader(_template);
			}
			namespace GMR {
				const JSerial::Template _template = { JSerial::Array<std::string>("tilesets") };
				JSerial::Write writer(_template);
				JSerial::Read reader(_template);
			}
			namespace SCN {
				const JSerial::Template _template = { JSerial::Type<std::string>("name"),
													  JSerial::Array<std::string>("rooms"),
													  JSerial::Type<std::string>("local res"),
													  JSerial::Type<unsigned short>("room id") };
				JSerial::Write writer(_template);
				JSerial::Read reader(_template);
			}
			namespace PRJ {
				const JSerial::Template _template = { JSerial::Type<std::string>("name"),
													  JSerial::Type<std::string>("global res"),
													  JSerial::Array<std::string>("scenes"),
													  JSerial::Type<unsigned short>("scene id"),
													  JSerial::Type<bool>("fullscreen"),
													  JSerial::Type<unsigned short>("screen height"),
													  JSerial::Type<std::string>("icon") };
				JSerial::Write writer(_template);
				JSerial::Read reader(_template);
			}
		}

		class Chunkmap {
		private:
			std::vector<std::vector<unsigned short>> map[4];

			pos2ds pos_size = { 0, 0 }, neg_size = { 0, 0 };

			inline void ResizeYP(short y) {
				map[0].resize(y * pos_size.x);
				map[1].resize(y * neg_size.x);
				pos_size.y = y;
			}

			inline void ResizeYN(short y) {
				map[2].resize(y * pos_size.x);
				map[3].resize(y * neg_size.x);
				neg_size.y = y;
			}

			void ResizeXP(short x) {
				map[0].resize(x * pos_size.y);
				map[2].resize(x * neg_size.y);

				for (short i = pos_size.y - 1; i >= 0; i--) { //Перенос элементов со старого места на новое
					for (short j = 0; j < pos_size.x; j++) {
						const short old_pos = i * pos_size.x + j;
						const short new_pos = i * x + j;

						map[new_pos] = std::move(map[old_pos]);
					}
					for (short j = x - pos_size.x; j < x; j++)
						map[0][i * x + j] = std::move(std::vector<unsigned short>(0));
					//memmove((quarters[0] + i * x), (quarters[0] + i * pos_size.x), pos_size.x * sizeof(Tile)),
					//	memset((quarters[0] + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(Tile));
				}
				for (short i = neg_size.y - 1; i >= 0; i--)
					memmove((quarters[2] + i * x), (quarters[2] + i * pos_size.x), pos_size.x * sizeof(Tile)),
					memset((quarters[2] + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(Tile));

				pos_size.x = x;
			}

			void ResizeXN(short x) {
				short cx = -(x + 1);

				quarters[1] = reinterpret_cast<Tile*>(realloc(quarters[1], pos_size.y * cx * sizeof(Tile)));
				quarters[3] = reinterpret_cast<Tile*>(realloc(quarters[3], neg_size.y * cx * sizeof(Tile)));

				for (short i = pos_size.y - 1; i >= 0; i--) {
					memmove((quarters[1] + i * cx), (quarters[1] + i * neg_size.x), neg_size.x * sizeof(Tile));
					memset((quarters[1] + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(Tile));
				}

				for (short i = neg_size.y - 1; i >= 0; i--) {
					memmove((quarters[3] + i * cx), (quarters[3] + i * neg_size.x), neg_size.x * sizeof(Tile));
					memset((quarters[3] + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(Tile));
				}

				neg_size.x = cx;
			}

		};

		struct Object {
			pos2df pos = { 0, 0 };

			JTexture::Frame::TransfProps props;

			bool is_prefab_local = 0;
			unsigned short prefab_id = 0;
		};

		struct Layer {
			std::string name = u8"Без имени";

			bool hidden = 0;
			float transparency = 1.0f;
			pos2df parallax_speed = { 0, 0 };



			//ObjectSystem::ChunkMap objs;
			Tiles::Tilemap tmap;

			std::string Serialize() {
				using SerializationData::LR::writer;
				writer.Reset();

				writer.WriteType(name);
				writer.WriteType(tmap.Serialize());

				writer.WriteType(hidden);
				writer.WriteType(transparency);
				writer.WriteType(parallax_speed);

				return writer.Result();
			}

			void Deserialize(const std::string& src) {
				using SerializationData::LR::reader;
				reader.Source(src);

				name		   = reader.ReadType<std::string>("name");
				tmap.Deserialize(reader.ReadType<std::string>("tmap"));

				hidden		   = reader.ReadType<bool>("hidden");
				transparency   = reader.ReadType<float>("transparency");
				parallax_speed = reader.ReadType<pos2df>("parallax speed");
			}
		};

		struct Room {
			std::string name = u8"Без имени";

			rgb background_color = { 0, 80, 170 };
			pos2d chunksize = { 512, 512 };

			std::vector<Layer> layers;
			std::unordered_map<std::string, unsigned short> layer_hash;

			std::unordered_map<unsigned int, Object> objects;
			
			pos2d camera_pos = { 0, 0 };

			std::string Serialize() {
				using SerializationData::RM::writer;
				writer.Reset();

				writer.WriteType(background_color);
				writer.WriteType(name);

				std::vector<std::string> serialized_layers(layers.size());
				for (int i = 0; i < layers.size(); i++)
					serialized_layers[i] = layers[i].Serialize();
				writer.WriteArray(serialized_layers);

				writer.WriteType(chunksize);
				writer.WriteType(camera_pos);

				return writer.Result();
			}

			void Deserialize(const std::string& src) {
				using SerializationData::RM::reader;
				reader.Source(src);

				background_color = reader.ReadType<rgb>("background color");
				name = reader.ReadType<std::string>("name");

				std::vector<std::string> serialized_layers = reader.ReadArray<std::string>("layers");
				layers.resize(serialized_layers.size());

				for (int i = 0; i < serialized_layers.size(); i++) {
					layers[i].Deserialize(serialized_layers[i]);
					layer_hash[layers[i].name] = i + 1;
				}

				chunksize = reader.ReadType<pos2d>("chunksize");
				camera_pos = reader.ReadType<pos2d>("camerapos");
			}
		};

		struct GameResources {
			std::vector<Tiles::tileset> tilesets;
			std::unordered_map<std::string, unsigned short> tileset_hash;
			std::vector<JTexture::Texture> textures;

			//vector<prefab> prefabs;
			//unordered_map<string, unsigned short> prefab_hash;
			//vector<sound> sounds;
			//vector<variable> variables;
			//vector<je_class> classes;
			//vector<function> functions;
			//vector<hitbox_tag> hitbox_tags;

			std::string Serialize() {
				using SerializationData::GMR::writer;
				writer.Reset();

				std::vector<std::string> serialized_tsets(tilesets.size());
				for (int i = 0; i < tilesets.size(); i++)
					serialized_tsets[i] = tilesets[i].Serialize();

				writer.WriteArray(serialized_tsets);

				return writer.Result();
			}

			void Deserialize(const std::string& src) {
				using SerializationData::GMR::reader;
				reader.Source(src);

				std::vector<std::string> serialized_tsets = reader.ReadArray<std::string>("tilesets");
				tilesets.resize(serialized_tsets.size());

				for (int i = 0; i < serialized_tsets.size(); i++) {
					tilesets[i].Deserialize(serialized_tsets[i]);
					tileset_hash[tilesets[i].name] = i + 1;
				}
			}
		};

		struct Scene {
			std::string name = u8"Без имени";
			std::vector<Room> rooms;
			std::unordered_map<std::string, unsigned short> room_hash;

			GameResources local_res;
			unsigned short c_room_id = 0;

			std::string Serialize() {
				using SerializationData::SCN::writer;
				writer.Reset();

				writer.WriteType(name);

				std::vector<std::string> serilaized_rooms(rooms.size());
				for (int i = 0; i < rooms.size(); i++)
					serilaized_rooms[i] = rooms[i].Serialize();
				writer.WriteArray(serilaized_rooms);

				writer.WriteType(local_res.Serialize());
				writer.WriteType(c_room_id);
				return writer.Result();
			}

			void Deserialize(const std::string& src) {
				using SerializationData::SCN::reader;
				reader.Source(src);

				name = reader.ReadType<std::string>("name");

				std::vector<std::string> ser_rooms = reader.ReadArray<std::string>("rooms");
				rooms.resize(ser_rooms.size());
				for (int i = 0; i < ser_rooms.size(); i++) {
					rooms[i].Deserialize(ser_rooms[i]);
					room_hash[rooms[i].name] = i + 1;
				}

				local_res.Deserialize(reader.ReadType<std::string>("local res"));
				c_room_id = reader.ReadType<unsigned short>("room id");
			}
		};

		struct Project {
			std::string name = u8"Без имени";

			GameResources global_res;
			std::vector<Scene> scenes;
			std::unordered_map<std::string, unsigned short> scene_hash;
			unsigned short c_scene_id = 0;

			bool fullscreen = 0;
			unsigned short screen_height = 256;

			JTexture::BasicTexture icon;

			std::string Serialize() {
				using SerializationData::PRJ::writer;
				writer.Reset();

				writer.WriteType(name);
				writer.WriteType(global_res.Serialize());

				std::vector<std::string> serialized_scenes(scenes.size());
				for (int i = 0; i < scenes.size(); i++)
					serialized_scenes[i] = scenes[i].Serialize();

				writer.WriteArray(serialized_scenes);
				writer.WriteType(c_scene_id);
				writer.WriteType(fullscreen);
				writer.WriteType(screen_height);
				writer.WriteType(icon.Serialize());

				return writer.Result();
			}

			void Deserialize(const std::string src) {
				using SerializationData::PRJ::reader;

				name = reader.ReadType<std::string>("name");

				global_res.Deserialize(reader.ReadType<std::string>("global res"));

				std::vector<std::string> serialized_scenes = reader.ReadArray<std::string>("scenes");
				scenes.resize(serialized_scenes.size());
				for (int i = 0; i < serialized_scenes.size(); i++) {
					scenes[i].Deserialize(serialized_scenes[i]);
					scene_hash[scenes[i].name] = i + 1;
				}

				c_scene_id = reader.ReadType<unsigned short>("scene id");
				fullscreen = reader.ReadType<bool>("fullscreen");
				screen_height = reader.ReadType<unsigned short>("screen height");

				icon.Deserialize(reader.ReadType<std::string>("icon"));
			}

			Scene& CurrScene() {
				return scenes[c_scene_id];
			}

			Room& curr_room() {
				return scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id];
			}

			GameResources& Res(bool is_local) {
				return is_local ? scenes[c_scene_id].local_res : global_res;
			}
			/*
			void replace_tile_in_all_scenes(Tiles::Tile from, Tiles::Tile to) {
				unsigned short range[] = { (from.is_local || to.is_local) ? c_scene_id : 0,
										   (from.is_local || to.is_local) ? c_scene_id + 1 : scenes.size() };

				for (unsigned short sc_id = range[0]; sc_id < range[1]; sc_id++) {
					for (unsigned short r_id = 0; r_id < scenes[sc_id].rooms.size(); r_id++) {
						Room* c_room = &scenes[sc_id].rooms[r_id];
						for (unsigned short l_id = 0; l_id < c_room->layers.size(); l_id++) {
							Layer* c_layer = &c_room->layers[l_id];
							c_layer->tmap.Replace(from, to);
						}
					}
				}
			}

			void swap_replace_tile_in_all_scenes(Tiles::Tile from, Tiles::Tile to) {
				unsigned short range[] = { (from.is_local || to.is_local) ? c_scene_id : 0,
										   (from.is_local || to.is_local) ? c_scene_id + 1 : scenes.size() };

				for (unsigned short sc_id = range[0]; sc_id < range[1]; sc_id++) {
					for (unsigned short r_id = 0; r_id < scenes[sc_id].rooms.size(); r_id++) {
						Room* c_room = &scenes[sc_id].rooms[r_id];
						for (unsigned short l_id = 0; l_id < c_room->layers.size(); l_id++) {
							Layer* c_layer = &c_room->layers[l_id];
							c_layer->tmap.SwapReplace(from, to);
						}
					}
				}
			}*/
		};
	}
}


/*
void update_pointer(unsigned short pointer_id, unsigned short new_id) {
	room& c_room = *get_curr_room();
	for (unsigned short i = 0; i < c_room.global_object_pointers[pointer_id].usage_list.size(); i++) {
		object_pointer::pointer_usage& c_usage = c_room.global_object_pointers[pointer_id].usage_list[i];
		switch (c_usage.type) {
			//case object_pointer::animated_texture_list: loaded_anims[c_usage.id].id = new_id;
		}
	}
}

void create_obj(pos2d pos, unsigned short pfab_id, bool is_local, unsigned short layer_id) {
	room* c_room = get_curr_room();
	Layer* c_layer = &c_room->Layers[layer_id];
	chunk* zero_chunk = c_layer->chunks.get_chunk({ 0, 0 });

	const pos2ds pos_by_chunk = { pos.x / c_room->chunksize.x,						pos.y / c_room->chunksize.y };
	const pos2ds pos_in_chunk = { pos.x - pos_by_chunk.x * c_room->chunksize.x,		pos.y - pos_by_chunk.y * c_room->chunksize.y };

	c_room->global_object_pointers.push_back({ object_pointer::_object, layer_id, {0, 0}, static_cast<unsigned short>(zero_chunk->objs.size()) });
	zero_chunk->objs.push_back({ pfab_id, is_local, pos, unsigned short(c_room->global_object_pointers.size() - 1) });
	c_layer->chunks.check_object(c_room->chunksize, zero_chunk->objs.size() - 1, { 0, 0 }, &c_room->global_object_pointers);
}

void delete_obj(unsigned short pointer_id) {
	room* c_room = get_curr_room();
	vector<object_pointer::object_pointer>* c_pointer_list = &c_room->global_object_pointers;
	object_pointer::object_pointer* c_pointer = &c_pointer_list->operator[](pointer_id);
	Layer* c_layer = &c_room->Layers[c_pointer->layer_id];
	chunk* c_chunk = c_layer->chunks.get_chunk(c_pointer->chunk_id);
	object* c_obj = &c_chunk->objs[c_pointer->in_chunk_object_id];

	for (unsigned short i = 0; i < c_pointer_list->operator[](pointer_id).usage_list.size(); i++) {
		object_pointer::pointer_usage& c_usage = c_pointer_list->operator[](pointer_id).usage_list[i];
		switch (c_usage.type) {
			//case object_pointer::animated_texture_list: loaded_anims[c_usage.id].id = -1;
		}
	}

	c_obj = &c_chunk->objs.back();
	c_pointer_list->operator[](c_obj->global_object_pointer_id).in_chunk_object_id = c_pointer->in_chunk_object_id;
	c_chunk->objs.pop_back();
	c_pointer_list->operator[](pointer_id) = c_pointer_list->back();
	update_pointer(pointer_id, pointer_id);
	c_pointer_list->pop_back();
}
*/


/*
namespace object_pointer {
	enum pointer_types {
		_object,
		_camera
	};

	enum usage_types {
		object_var,
		global_var,
		scene_var
	};

	struct pointer_usage {
		usage_types type;
		unsigned short id;
		unsigned short id2;
	};

	struct object_pointer {
		pointer_types type;
		unsigned short layer_id;
		pos2ds chunk_id;
		unsigned short in_chunk_object_id;
		vector<pointer_usage> usage_list;
	};
}*/


//struct chunk_quarter {
//	vector<vector<ObjectSystem::chunk>> quarter;
//	pos2d size;
//};
/*
struct chunkmap {
	chunk_quarter xpyp, xmyp, xpym, xmym;

	void check_object(pos2d chunk_size, object *obj, unsigned short id, pos2ds chunk_id, chunk* source, vector<object_pointer::object_pointer>* ptr) {
		if (obj->pos.x < chunk_size.x && obj->pos.x >= 0 && obj->pos.y < chunk_size.y && obj->pos.y >= 0)
			return;

		pos2ds target_chunk_id = { chunk_id.x + obj->pos.x / chunk_size.x - (1 * obj->pos.x < 0), chunk_id.y + obj->pos.y / chunk_size.y - (1 * obj->pos.y < 0) };
		chunk* target = get_chunk(target_chunk_id);

		obj->pos = { obj->pos.x - (target_chunk_id.x - chunk_id.x) * chunk_size.x, obj->pos.y - (target_chunk_id.y - chunk_id.y) * chunk_size.y };

		if (target == nullptr) {
			bool xq = target_chunk_id.x >= 0, yq = target_chunk_id.y >= 0;
			chunk_quarter* curr_quarter = xq ? (yq ? &xpyp : &xpym) : (yq ? &xmyp : &xmym);
			pos2ds converted_pos = { target_chunk_id.x * xq + -(target_chunk_id.x + 1) * !xq, target_chunk_id.y * yq + -(target_chunk_id.y + 1) * !yq };

			if (converted_pos.x >= curr_quarter->size.x) {
				curr_quarter->quarter.resize(converted_pos.x + 1);
				for (int i = curr_quarter->size.x; i < converted_pos.x + 1; i++)
					curr_quarter->quarter[i].resize(curr_quarter->size.y);
				curr_quarter->size.x = converted_pos.x + 1;
			}
			if (converted_pos.y >= curr_quarter->size.y) {
				for (int i = 0; i < curr_quarter->size.x; i++)
					curr_quarter->quarter[i].resize(converted_pos.y + 1);
				curr_quarter->size.y = converted_pos.y + 1;
			}
			target = get_chunk(target_chunk_id);
		}
		source = get_chunk(chunk_id);
		object tmp = *obj;
		source->objs[id] = source->objs.back();

		ptr->operator[](source->objs[id].global_object_pointer_id).in_chunk_object_id = id; //обновить указатель на обьект того перемещенного обьекта

		source->objs.pop_back();
		target->objs.push_back(tmp);

		ptr->operator[](target->objs.back().global_object_pointer_id).chunk_id = target_chunk_id;
		ptr->operator[](target->objs.back().global_object_pointer_id).in_chunk_object_id = static_cast<unsigned short>(target->objs.size() - 1); //обновить указатель на обьект, перемещенный в другой чанк
	}

	void check_object(pos2d chunk_size, unsigned short id, pos2ds chunk_id, vector<object_pointer::object_pointer>* ptr) {
		chunk* source = get_chunk(chunk_id);
		if (source == nullptr) return;
		object* const curr_obj = &source->objs[id];
		if (curr_obj->pos.x < chunk_size.x && curr_obj->pos.x >= 0 && curr_obj->pos.y < chunk_size.y && curr_obj->pos.y >= 0) return;

		const pos2ds target_chunk_id = { chunk_id.x + curr_obj->pos.x / chunk_size.x - (1 * curr_obj->pos.x < 0), chunk_id.y + curr_obj->pos.y / chunk_size.y - (1 * curr_obj->pos.y < 0) };
		chunk* target = get_chunk(target_chunk_id);

		curr_obj->pos = {curr_obj->pos.x - (target_chunk_id.x - chunk_id.x) * chunk_size.x, curr_obj->pos.y - (target_chunk_id.y - chunk_id.y) * chunk_size.y };

		if (target == nullptr) {
			bool xq = target_chunk_id.x >= 0, yq = target_chunk_id.y >= 0;
			chunk_quarter* curr_quarter = xq ? (yq ? &xpyp : &xpym) : (yq ? &xmyp : &xmym);
			pos2ds converted_pos = { target_chunk_id.x * xq + -(target_chunk_id.x + 1) * !xq, target_chunk_id.y * yq + -(target_chunk_id.y + 1) * !yq };

			if (converted_pos.x >= curr_quarter->size.x) {
				curr_quarter->quarter.resize(converted_pos.x + 1);
				for (int i = curr_quarter->size.x; i < converted_pos.x + 1; i++)
					curr_quarter->quarter[i].resize(curr_quarter->size.y);
				curr_quarter->size.x = converted_pos.x + 1;
			}
			if (converted_pos.y >= curr_quarter->size.y) {
				for (int i = 0; i < curr_quarter->size.x; i++)
					curr_quarter->quarter[i].resize(converted_pos.y + 1);
				curr_quarter->size.y = converted_pos.y + 1;
			}
			target = get_chunk(target_chunk_id);
		}
		source = get_chunk(chunk_id);
		object tmp = source->objs[id];
		source->objs[id] = source->objs.back();

		ptr->operator[](source->objs[id].global_object_pointer_id).in_chunk_object_id = id; //обновить указатель на обьект того перемещенного обьекта

		source->objs.pop_back();
		target->objs.push_back(tmp);

		ptr->operator[](target->objs.back().global_object_pointer_id).chunk_id = target_chunk_id;
		ptr->operator[](target->objs.back().global_object_pointer_id).in_chunk_object_id = static_cast<unsigned short>(target->objs.size() - 1); //обновить указатель на обьект, перемещенный в другой чанк
	}

	chunk* get_chunk(pos2ds id) {
		bool xq = id.x >= 0, yq = id.y >= 0;
		chunk_quarter* curr_quarter = xq ? (yq ? &xpyp : &xpym) : (yq ? &xmyp : &xmym);
		pos2ds converted_pos = { id.x * xq + -(id.x + 1) * !xq, id.y * yq + -(id.y + 1) * !yq };

		if (converted_pos.x < curr_quarter->size.x && converted_pos.y < curr_quarter->size.y)
			return &curr_quarter->quarter[converted_pos.x][converted_pos.y];
		return nullptr;
	}
};
*/

/*
	struct prefab {
		std::string name = u8"Без имени";
		Texture::Texture txt;
		std::vector<std::string> script;
		std::vector<programming_lang::var_declaration> vartypes;
		std::unordered_map<std::string, unsigned short> var_hash;

		std::string Serialize() {
			JSerial::Template pref_tmpl = { JSerial::Type<std::string>("name"), JSerial::Type<std::string>("texture"), JSerial::Array<std::string>("script"), JSerial::Array<std::string>("vartypes") };
			JSerial::Write<> prfw(pref_tmpl);
			prfw.WriteType(name);
			prfw.WriteType(txt.Serialize());
			prfw.WriteArray(script);

			std::vector<std::string> s_vartypes(vartypes.size());

			for (int i = 0; i < vartypes.size(); i++)
				s_vartypes[i] = vartypes[i].Serialize();
			prfw.WriteArray<std::string>(s_vartypes);
			return prfw.data;
		}

		void Deserialize(std::string src) {
			JSerial::Template pref_tmpl = { JSerial::Type<std::string>("name"), JSerial::Type<std::string>("texture"), JSerial::Array<std::string>("script"), JSerial::Array<std::string>("vartypes") };
			JSerial::Read<> rd(src, pref_tmpl);

			name = rd.ReadType<std::string>("name");
			txt.Deserialize(rd.ReadType<string>("texture"));
			script = rd.ReadArray<string>("script");

			std::vector<std::string> s_vartypes = rd.ReadArray<std::string>("vartypes");
			vartypes.resize(s_vartypes.size());

			for (int i = 0; i < vartypes.size(); i++) {
				vartypes[i].Deserialize(s_vartypes[i]);
				var_hash[vartypes[i].name] = i + 1;
			}
		}
	};

	struct object {
		unsigned short pfab_id;
		bool is_pfab_local;
		pos2d pos;
		std::vector<programming_lang::var> vars;
		Texture::Properties texture_props;
		Texture::AnimPlayerProperties animation_props;

		std::string Serialize() {
			JSerial::Write<> wr({ JSerial::Type<unsigned short>("pfid"), JSerial::Type<bool>("plc"), JSerial::Type<pos2d>("pos"), JSerial::Array<string>("vars"), JSerial::Type<string>("tpr"), JSerial::Type<string>("apr") });
			wr.WriteType(pfab_id);
			wr.WriteType(is_pfab_local);
			wr.WriteType(pos);
			vector<string> s_vars(vars.size());
			for (int i = 0; i < vars.size(); i++) {

			}
		}
		void Deserialize() {

		}
	};

	namespace ObjectSystem {
		class ObjectPointer;
		class ObjectWrapper;
		class System;

		class ObjectPointer {
				friend class ObjectWrapper;
			private:
				struct usage_record {
					usage_record* prev = nullptr;
					usage_record* next = nullptr;
					ObjectPointer* pointer_usage = nullptr;
				};

				//Указатель на элемент в списке использования объекта
				usage_record* curr_record = nullptr;
				ObjectWrapper* target = nullptr;

			public:
				ObjectPointer() {}
				ObjectPointer(const ObjectPointer& cpy);
				ObjectPointer& operator=(const ObjectPointer& cpy);
				~ObjectPointer();

				bool IsNull() {
					return target == nullptr;
				}

				unsigned short GetSerializationID();

				object* GetObj();

				void DeleteObj();
		};

		class ObjectWrapper {
				friend class ObjectPointer;
				friend class System;
			private:
				//Список использований
				ObjectPointer::usage_record *last_usage = nullptr;
				unsigned short usage_count = 0;

				ObjectWrapper *prev = nullptr;
				ObjectWrapper *next = nullptr;

				System *owner;

				ObjectPointer::usage_record *NewUsage(ObjectPointer *pointer_usage);
				void RemoveUsage(ObjectPointer::usage_record *target);

			public:
				~ObjectWrapper();
				ObjectPointer GivePointer();

				object data;
				unsigned short serialization_id;
		};

		class System {
				friend class ObjectWrapper;
				friend class ObjectPointer;
			private:
				ObjectWrapper *last_obj = nullptr;
				unsigned short object_count = 0;
				bool ids_correct = 0;

			public:
				ObjectPointer NewObject(const object &data);
				void SerializationPrepare();
				vector<ObjectPointer> GetAllObjectPointers() {
					vector<ObjectPointer> result(object_count);

					ObjectWrapper* c_wrap = last_obj;
					for (int i = 0; i < object_count; i++) {
						result[i] = c_wrap->GivePointer();
						c_wrap = c_wrap->prev;
					}
					return result;
				}

				System(){}

				System(const System& cpy);
				System& operator=(const System& cpy);


				~System();
		};

		ObjectPointer::ObjectPointer(const ObjectPointer& cpy) {
			//Удалить старый указатель, если в нем что-то было
			if (target != nullptr)
				this->~ObjectPointer();

			//Скопировать сам указатель на обьект
			target = cpy.target;

			if (target != nullptr) {
				//Добавить элемент в список использований требуемого объекта
				curr_record = target->NewUsage(this);
			}
		}
		ObjectPointer& ObjectPointer::operator=(const ObjectPointer& cpy) {
			//Удалить старый указатель, если в нем что-то было
			if (target != nullptr)
				this->~ObjectPointer();

			//Скопировать сам указатель на обьект
			target = cpy.target;

			if (target != nullptr) {
				//Добавить элемент в список использований требуемого объекта
				curr_record = target->NewUsage(this);
			}

			return *this;
		}
		ObjectPointer::~ObjectPointer() {
			if (target != nullptr) {
				target->RemoveUsage(curr_record);

				if (!target->usage_count)
					delete target;
			}
		}
		object* ObjectPointer::GetObj() {
			return &target->data;
		}
		unsigned short ObjectPointer::GetSerializationID() {
			if (!target->owner->ids_correct)
				target->owner->SerializationPrepare();

			return target->serialization_id;
		}
		void ObjectPointer::DeleteObj() {
			delete target;
		}

		ObjectPointer::usage_record* ObjectWrapper::NewUsage(ObjectPointer* pointer_usage) {
			ObjectPointer::usage_record* old_last = last_usage;

			last_usage = new ObjectPointer::usage_record;
			last_usage->pointer_usage = pointer_usage;

			if (old_last != nullptr) {
				old_last->next = last_usage;
				last_usage->prev = old_last;
			}
			usage_count++;

			return last_usage;
		}

		void ObjectWrapper::RemoveUsage(ObjectPointer::usage_record* target) {
			if (target != nullptr) {
				if (target->next == nullptr)
					last_usage = target->prev;

				else {
					target->next->prev = target->prev;

					if (target->prev != nullptr)
						target->prev->next = target->next;
				}

				delete target;
				usage_count--;
			}
			else cout << "null target\n";
		}
		ObjectWrapper::~ObjectWrapper() {

			while (usage_count != 0) {
				if (last_usage != nullptr) {
					last_usage->pointer_usage->target = nullptr;
					ObjectPointer::usage_record* old_last = last_usage;
					last_usage = last_usage->prev;
					delete old_last;
					usage_count--;
				}
				else {
					cout << "last_usage for some reason was null\n"; usage_count--;
				}
			}

			if (next != nullptr)
				next->prev = prev;
			else
				owner->last_obj = prev;

			if (prev != nullptr)
				prev->next = next;

			owner->ids_correct = 0;
			owner->object_count--;

		}
		ObjectPointer ObjectWrapper::GivePointer() {
			ObjectPointer a;
			a.target = this;
			a.curr_record = NewUsage(&a);

			return a;
		}

		ObjectPointer System::NewObject(const object& data) {
			if (last_obj != nullptr) {
				last_obj->next = new ObjectWrapper;
				last_obj->next->prev = last_obj;
				last_obj = last_obj->next;
			}
			else
				last_obj = new ObjectWrapper;

			++object_count;

			last_obj->owner = this;
			last_obj->data = data;
			last_obj->serialization_id = object_count - 1;

			return last_obj->GivePointer();
		}
		void System::SerializationPrepare() {
			ObjectWrapper* c_obj = last_obj;
			for (unsigned short i = 0; i < object_count; ++i) {
				c_obj->serialization_id = (object_count - 1) - i;
				c_obj = c_obj->prev;
			}
		}

		//При копировании(например при сдвиге внутри std::vector) отвязать от старого обьекта указатели
		System::System(const System& cpy) {
			last_obj = cpy.last_obj;
			object_count = cpy.object_count;
			ids_correct = cpy.ids_correct;

			const_cast<System&>(cpy).last_obj = nullptr;
			const_cast<System&>(cpy).object_count = 0;
		}
		System& System::operator=(const System& cpy) {
			last_obj = cpy.last_obj;
			object_count = cpy.object_count;
			ids_correct = cpy.ids_correct;

			const_cast<System&>(cpy).last_obj = nullptr;
			const_cast<System&>(cpy).object_count = 0;
			return *this;
		}
		System::~System() {
			if (last_obj != nullptr) {
				//Удалить все обьекты
				while (object_count != 0) {
					ObjectWrapper* old_last = last_obj;
					last_obj = last_obj->prev;
					delete old_last;
				}
			}
		}

		struct HeapChunk {
			vector<ObjectPointer> objs;

			~HeapChunk() {
				for (ObjectPointer& i : objs)
					i.DeleteObj();
				objs.~vector();
			}
		};

		class ChunkMap {
		private:
			//                          x+ y+    x- y+    x+ y-    x- y-
			//HeapChunk **quarters[4] = {nullptr, nullptr, nullptr, nullptr};
			vector<HeapChunk*> qtrs[4];
		public:
			pos2ds pos_size, neg_size;
			//System* owner;
			ChunkMap() {
				//quarters[0] = reinterpret_cast<HeapChunk**>(malloc(0));
				//quarters[1] = reinterpret_cast<HeapChunk**>(malloc(0));
				//quarters[2] = reinterpret_cast<HeapChunk**>(malloc(0));
				//quarters[3] = reinterpret_cast<HeapChunk**>(malloc(0));
			}

			ChunkMap(const ChunkMap& cpy) {
				qtrs[0] = cpy.qtrs[0];
				qtrs[1] = cpy.qtrs[1];
				qtrs[2] = cpy.qtrs[2];
				qtrs[3] = cpy.qtrs[3];

				pos_size = cpy.pos_size;
				neg_size = cpy.neg_size;

				const_cast<ChunkMap&>(cpy).qtrs[0].clear();
				const_cast<ChunkMap&>(cpy).qtrs[1].clear();
				const_cast<ChunkMap&>(cpy).qtrs[2].clear();
				const_cast<ChunkMap&>(cpy).qtrs[3].clear();
				const_cast<ChunkMap&>(cpy).pos_size = { 0,0 };
				const_cast<ChunkMap&>(cpy).neg_size = { 0,0 };
			}

			ChunkMap& operator=(const ChunkMap& cpy) {
				qtrs[0] = cpy.qtrs[0];
				qtrs[1] = cpy.qtrs[1];
				qtrs[2] = cpy.qtrs[2];
				qtrs[3] = cpy.qtrs[3];

				pos_size = cpy.pos_size;
				neg_size = cpy.neg_size;

				const_cast<ChunkMap&>(cpy).qtrs[0].clear();
				const_cast<ChunkMap&>(cpy).qtrs[1].clear();
				const_cast<ChunkMap&>(cpy).qtrs[2].clear();
				const_cast<ChunkMap&>(cpy).qtrs[3].clear();
				const_cast<ChunkMap&>(cpy).pos_size = { 0,0 };
				const_cast<ChunkMap&>(cpy).neg_size = { 0,0 };
				return *this;
			}

			HeapChunk* GetChunk(pos2ds id) {
				vector<HeapChunk*> &c_qtr = qtrs[((id.y < 0) << 1) + (id.x < 0)];
				pos2ds c_size = { (id.x >= 0) ? pos_size.x : neg_size.x, (id.y >= 0) ? pos_size.y : neg_size.y };
				pos2ds conv_id = { std::abs(id.x) - (id.x < 0), std::abs(id.y) - (id.y < 0)};

				return (conv_id.x < c_size.x&& conv_id.y < c_size.y) ? c_qtr[conv_id.y * c_size.x + conv_id.x] : nullptr;
			}

			void ResizePos(pos2ds size){
				if (size.x >= pos_size.x && size.y >= pos_size.y) {
					//x+ y+
					qtrs[0].resize(size.x * size.y);
					//quarters[0] = reinterpret_cast<HeapChunk**>(realloc(quarters[0], size.x * size.y * sizeof(void*)));
					//x- y+
					qtrs[1].resize(neg_size.x * size.y);
					//quarters[1] = reinterpret_cast<HeapChunk**>(realloc(quarters[1], neg_size.x * size.y * sizeof(void*)));
					//x+ y-
					qtrs[2].resize(size.x * neg_size.y);
					//quarters[2] = reinterpret_cast<HeapChunk**>(realloc(quarters[2], size.x * neg_size.y * sizeof(void*)));

					//Изменение размера по Y
					for (short q_y = pos_size.y; q_y < size.y; ++q_y) {
						for (short q_x = 0; q_x < size.x; ++q_x)
							qtrs[0][q_y * size.x + q_x] = new HeapChunk;
					}
					for (short q_y = pos_size.y; q_y < size.y; ++q_y) {
						for (short q_x = 0; q_x < neg_size.x; ++q_x)
							qtrs[1][q_y * neg_size.x + q_x] = new HeapChunk;
					}

					//Изменение размера по X
					for (short q_y = pos_size.y - 1; q_y >= 0; --q_y) {
						memmove((qtrs[0].data() + q_y * size.x), (qtrs[0].data() + q_y * pos_size.x), pos_size.x * sizeof(void*));
						for (short q_x = pos_size.x; q_x < size.x; ++q_x)
							qtrs[0][q_y * size.x + q_x] = new HeapChunk;
					}

					for (short q_y = neg_size.y - 1; q_y >= 0; --q_y) {
						memmove((qtrs[2].data() + q_y * size.x), (qtrs[2].data() + q_y * pos_size.x), pos_size.x * sizeof(void*));
						for (short q_x = pos_size.x; q_x < size.x; ++q_x)
							qtrs[2][q_y * size.x + q_x] = new HeapChunk;
					}

					pos_size = size;
				}
			}

			void ResizeNeg(pos2ds size){
				//pos2ds c_size = { -size.x - 1, -size.y - 1 };
				pos2ds c_size = size;
				if (c_size.x >= neg_size.x && c_size.y >= neg_size.y) {
					//x- y+
					qtrs[1].resize(c_size.x * pos_size.y);
					//x+ y-
					qtrs[2].resize(pos_size.x * c_size.y);
					//x- y-
					qtrs[3].resize(c_size.x * c_size.y);

					//Изменение размера по Y
					for (short q_y = neg_size.y; q_y < c_size.y; ++q_y) {
						for (short q_x = 0; q_x < pos_size.x; ++q_x)
							qtrs[2][q_y * pos_size.x + q_x] = new HeapChunk;
					}
					for (short q_y = neg_size.y; q_y < c_size.y; ++q_y) {
						for (short q_x = 0; q_x < c_size.x; ++q_x)
							qtrs[3][q_y * c_size.x + q_x] = new HeapChunk;
					}

					//Изменение размера по X
					for (short q_y = pos_size.y - 1; q_y >= 0; --q_y) {
						memmove((qtrs[1].data() + q_y * c_size.x), (qtrs[1].data() + q_y * neg_size.x), neg_size.x * sizeof(void*));
						for (short q_x = neg_size.x; q_x < c_size.x; ++q_x)
							qtrs[1][q_y * c_size.x + q_x] = new HeapChunk;
					}

					for (short q_y = neg_size.y - 1; q_y >= 0; --q_y) {
						memmove((qtrs[3].data() + q_y * c_size.x), (qtrs[3].data() + q_y * neg_size.x), neg_size.x * sizeof(void*));
						for (short q_x = neg_size.x; q_x < c_size.x; ++q_x)
							qtrs[3][q_y * c_size.x + q_x] = new HeapChunk;
					}

					neg_size = c_size;
				}
			}

			HeapChunk* AutoResize(pos2ds id) {
				if ((-id.x - 1) >= neg_size.x || (-id.y - 1) >= neg_size.y) {
					ResizeNeg({ ((-id.x - 1) >= neg_size.x) ? static_cast<short>(-id.x) : neg_size.x,
								((-id.y - 1) >= neg_size.y) ? static_cast<short>(-id.y) : neg_size.y });
				}
				if (id.x >= pos_size.x || id.y >= pos_size.y) {
					ResizePos({ (id.x >= pos_size.x) ? static_cast<short>(id.x + 1) : pos_size.x,
								(id.y >= pos_size.y) ? static_cast<short>(id.y + 1) : pos_size.y });
				}
				return GetChunk(id);
			}

			~ChunkMap() {
				//Удаление чанков
				{
					for (short chunk_y = -neg_size.y; chunk_y < pos_size.y; chunk_y++) {
						for (short chunk_x = -neg_size.x; chunk_x < pos_size.x; chunk_x++) {
							delete GetChunk({ chunk_x, chunk_y });
						}
					}
				}
				qtrs[0].~vector();
				qtrs[1].~vector();
				qtrs[2].~vector();
				qtrs[3].~vector();
			}
		};

	}
	*/