#pragma once
#include <Tilemap.h>
//#include <iostream>
//#include <fstream>
#include <string>
#include <cmath>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <codecvt>
//#include <filesystem>
#include <camera.h>
#include <window.h>
#include <texture.h>
#include <pos2d.h>

/*
namespace programming_lang {
	bool is_number(const std::string& s) {
		return(strspn(s.c_str(), "-.0123456789") == s.size());
	}

	std::vector<std::string> words(std::string src, char spacing_char = ' ') {
		std::vector<std::string> res;
		if (src.size()) {
			std::string::iterator from = src.begin();
			std::string::iterator to = find(from, src.end(), spacing_char);

			if (to == src.end()) return { src };
			++to;

			while (from != to) {
				std::string tmp(from, to - (std::string(to - 1, to)[0] == spacing_char ? 1 : 0));
				if (tmp.size())
					res.push_back(tmp);
				from = to;
				to = find(from, src.end(), spacing_char);
				if (to != src.end()) ++to;
			}
		}
		else res.push_back("");
		return res;
	}

	enum vartypes {
		integer,
		string,
		position
	};

	struct var_declaration {
		vartypes type;
		std::string name;
		//std::string default_value;

		std::string Serialize() {
			JSerial::Write<> wr({ JSerial::Type<int>("type"), JSerial::Type<std::string>("name") });
			wr.WriteType<int>(this->type);
			wr.WriteType(name);
			return wr.data;
		}

		void Deserialize(std::string src) {
			JSerial::Read<> rd(src, { JSerial::Type<int>("type"), JSerial::Type<std::string>("name") });
			int tmp = rd.ReadType<int>("type");
			type = *reinterpret_cast<programming_lang::vartypes*>(&tmp);
			name = rd.ReadType<std::string>("name");
		}
	};

	struct var {
		std::string data = { 0, 0, 0, 0 };
		vartypes type = integer;
	};

	var IntToVar(int data) {
		return { std::string(reinterpret_cast<const char*>(&data), sizeof(int)), integer };
	}

	var Pos2dToVar(pos2d data) {
		return { std::string(reinterpret_cast<const char*>(&data), sizeof(pos2d)), integer };
	}

	int VarToInt(var data) {
		return *reinterpret_cast<const int*>(data.data.data());
	}

	pos2d VarToPos2d(var data) {
		return *reinterpret_cast<const pos2d*>(data.data.data());
	}

	var LiteralToVar(std::string src, std::unordered_map<std::string, unsigned short>& var_ids, std::vector<var>& vars) {
		if (src[0] == char(34) && src.back() == char(34)) {
			return { std::string(src.begin() + 1, src.end() - 1), string };
		}
		else if (src[0] == '{' && src.back() == '}') {
			pos2d tmp;
			std::vector<std::string> tmp1 = words(std::string(src.begin() + 1, src.end() - 1), ',');

			if (is_number(tmp1[0]))
				tmp.x = atoi(tmp1[0].c_str());
			else if (var_ids[tmp1[0]])
				tmp.x = *reinterpret_cast<const int*>(vars[var_ids[tmp1[0]] - 1].data.c_str());

			if (is_number(tmp1[1]))
				tmp.y = atoi(tmp1[1].c_str());
			else if (var_ids[tmp1[1]])
				tmp.y = *reinterpret_cast<int*>(const_cast<char*>(vars[var_ids[tmp1[1]] - 1].data.c_str()));

			return { std::string(reinterpret_cast<const char*>(&tmp), sizeof(pos2d)), position };
		}
		else if (is_number(src)) {
			int tmp = atoi(src.c_str());
			return { std::string(reinterpret_cast<const char*>(&tmp), sizeof(int)), integer };
		}
		else if (var_ids[src]) {
			return vars[var_ids[src] - 1];
		}
		else if (src.back() == '%') {
			pos2d tmp = *reinterpret_cast<pos2d*>(const_cast<char*>(LiteralToVar(std::string(src.begin(), src.end() - 2), var_ids, vars).data.c_str()));
			return { std::string(reinterpret_cast<const char*>(src[src.size() - 2] == 'y' ? &tmp.y : &tmp.x), sizeof(int)), integer };
		}
	}

	void OperationTwoComponents(var operand_1, var operand_2, var* output, int (func)(int, int), bool is_member_of_pos2d, bool to_y) {
		if (is_member_of_pos2d) {
			pos2d tmp = *reinterpret_cast<pos2d*>(const_cast<char*>(output->data.c_str()));

			if (to_y)
				tmp.y = func(*reinterpret_cast<const int*>(operand_1.data.c_str()), *reinterpret_cast<const int*>(operand_2.data.c_str()));
			else
				tmp.x = func(*reinterpret_cast<const int*>(operand_1.data.c_str()), *reinterpret_cast<const int*>(operand_2.data.c_str()));

			output->data = std::string(reinterpret_cast<const char*>(&tmp), sizeof(pos2d));
		}
		else if (output->type == integer) {
			int tmp = func(*reinterpret_cast<const int*>(operand_1.data.c_str()), *reinterpret_cast<const int*>(operand_2.data.c_str()));
			output->data = std::string(reinterpret_cast<const char*>(&tmp), sizeof(int));
		}
		else if (output->type == position) {
			pos2d tmp = { func(reinterpret_cast<const pos2d*>(operand_1.data.c_str())->x, reinterpret_cast<const pos2d*>(operand_2.data.c_str())->x),
						  func(reinterpret_cast<const pos2d*>(operand_1.data.c_str())->y, reinterpret_cast<const pos2d*>(operand_2.data.c_str())->y) };

			output->data = std::string(reinterpret_cast<const char*>(&tmp), sizeof(pos2d));
		}
	}
}
*/

namespace gameobjs {
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
	struct Layer {
		std::string name = u8"Без имени";
		//ObjectSystem::ChunkMap objs;
		Tiles::Tilemap tmap;
		bool hidden = 0;
		float transparency = 1.0f;
		pos2df parallax_speed;


		std::string Serialize() {
			JSerial::Template layer_templ = { JSerial::Type<std::string>("name"),
											  JSerial::Type<std::string>("tmap"),
											  JSerial::Type<bool>("hidden"),
											  JSerial::Type<float>("transparency"),
											  JSerial::Type<pos2df>("parallax speed") };

			JSerial::Write ser_layer(layer_templ);

			ser_layer.WriteType(name);
			ser_layer.WriteType(tmap.Serialize());
			ser_layer.WriteType(hidden);
			ser_layer.WriteType(transparency);
			ser_layer.WriteType(parallax_speed);

			return ser_layer.Result();
		}

		void Deserialize(const std::string& src) {
			JSerial::Template layer_templ = { JSerial::Type<std::string>("name"),
											  JSerial::Type<std::string>("tmap"),
											  JSerial::Type<bool>("hidden"),
											  JSerial::Type<float>("transparency"),
											  JSerial::Type<pos2df>("parallax speed") };

			JSerial::Read read_layer(layer_templ, src);
			name = read_layer.ReadType<string>("name");
			tmap.Deserialize(read_layer.ReadType<string>("tmap"));
			hidden = read_layer.ReadType<bool>("hidden");
			transparency = read_layer.ReadType<float>("transparency");
			parallax_speed = read_layer.ReadType<pos2df>("parallax speed");
		}
	};

	struct room {
		rgb background_color = { 0, 80, 170 };
		string name = u8"Без имени";
		vector<Layer> layers;

		//ObjectSystem::System all_objs;

		unordered_map<string, unsigned short> layer_hash;
		pos2d chunksize = {512, 512};

		//Используется только во время воспроизведения (а не в редакторе)
		pos2d camera_pos = {0, 0};

		void hash_layers() {
			for (int i = 0; i < layers.size(); i++) {
				layer_hash[layers[i].name] = i + 1;
			}
		}

		string Serialize() {
			JSerial::Template room_templ = { JSerial::Type<rgb>("background color"), 
											 JSerial::Type<string>("name"), 
											 JSerial::Array<string>("layers"), 
											 JSerial::Type<pos2d>("chunksize"), 
											 JSerial::Type<pos2d>("camerapos") };

			JSerial::Write wrt(room_templ);

			wrt.WriteType(background_color);
			wrt.WriteType(name);

			vector<string> serialized_layers(layers.size());
			for (int i = 0; i < layers.size(); i++) {
				serialized_layers[i] = layers[i].Serialize();
			}

			wrt.WriteArray(serialized_layers);

			wrt.WriteType(chunksize);
			wrt.WriteType(camera_pos);
			return wrt.Result();
		}

		void Deserialize(string src) {
			JSerial::Template room_templ = { JSerial::Type<rgb>("background color"),
											 JSerial::Type<string>("name"),
											 JSerial::Array<string>("layers"),
											 JSerial::Type<pos2d>("chunksize"),
											 JSerial::Type<pos2d>("camerapos") };

			JSerial::Read rd(room_templ, src);

			background_color = rd.ReadType<rgb>("background color");
			name = rd.ReadType<string>("name");

			vector<string> serialized_layers = rd.ReadArray<string>("layers");
			layers.resize(serialized_layers.size());

			for (int i = 0; i < serialized_layers.size(); i++) {
				
				layers[i].Deserialize(serialized_layers[i]);
			}
			hash_layers();

			chunksize = rd.ReadType<pos2d>("chunksize");
			camera_pos = rd.ReadType<pos2d>("camerapos");
		}
	};

	struct resources {
		vector<Tiles::tileset> tilesets;
		//vector<prefab> prefabs;
		unordered_map<string, unsigned short> tileset_hash;
		unordered_map<string, unsigned short> prefab_hash;

		void hash_tilesets() {
			for (int i = 0; i < tilesets.size(); i++) {
				tileset_hash[tilesets[i].name] = i + 1;
			}
		}
		
		//void hash_prefabs() {
		//	for (int i = 0; i < prefabs.size(); i++) {
		//		prefab_hash[tilesets[i].name] = i + 1;
		//	}
		//}
		//vector<sound> sounds;
		//vector<variable> variables;
		//vector<je_class> classes;
		//vector<function> functions;
		//vector<hitbox_tag> hitbox_tags;

		string Serialize() {
			JSerial::Template rs_templ = {JSerial::Array<string>("tilesets")};

			JSerial::Write res_wr(rs_templ);

			vector<string> serialized_tsets(tilesets.size());
			for (int i = 0; i < tilesets.size(); i++) {
				serialized_tsets[i] = tilesets[i].Serialize();
			}

			res_wr.WriteArray(serialized_tsets);

			return res_wr.Result();
		}

		void Deserialize(string src) {
			JSerial::Template rs_templ = { JSerial::Array<string>("tilesets") };

			JSerial::Read res_rd(rs_templ, src);

			vector<string> serialized_tsets = res_rd.ReadArray<string>("tilesets");
			tilesets.resize(serialized_tsets.size());

			for (int i = 0; i < serialized_tsets.size(); i++) {
				tilesets[i].Deserialize(serialized_tsets[i]);
			}
			hash_tilesets();
		}

		//void destroy() {
		//	for (Tiles::tileset& i : tilesets) {
		//		for (Tiles::t_tile& j : i.tiles) {
		//			j.tile_texture.destroy();
		//		}
		//		tileset_hash[i.name] = 0;
		//	}
		//	tilesets.clear();
		//
		//	for (prefab& i : prefabs)
		//		i.txt.destroy();
		//	prefabs.clear();
		//}
	};

	struct Scene {
		string name = u8"Без имени";
		vector<room> rooms;
		unordered_map<string, unsigned short> room_hash;

		resources local_res;
		unsigned short c_room_id = 0;

		string Serialize() {
			JSerial::Template sc_tmpl = { JSerial::Type<string>("name"), 
										  JSerial::Array<string>("rooms"), 
										  JSerial::Type<string>("local res"), 
										  JSerial::Type<unsigned short>("room id")};

			JSerial::Write sc_wr(sc_tmpl);

			sc_wr.WriteType(name);

			vector<string> serilaized_rooms(rooms.size());
			for (int i = 0; i < rooms.size(); i++) {
				serilaized_rooms[i] = rooms[i].Serialize();
			}
			sc_wr.WriteArray(serilaized_rooms);
			sc_wr.WriteType(local_res.Serialize());
			sc_wr.WriteType(c_room_id);

			return sc_wr.Result();
		}

		void Deserialize(string src) {
			JSerial::Template sc_tmpl = { JSerial::Type<string>("name"),
										  JSerial::Array<string>("rooms"),
										  JSerial::Type<string>("local res"),
										  JSerial::Type<unsigned short>("room id") };

			JSerial::Read sc_rd(sc_tmpl, src);

			name = sc_rd.ReadType<string>("name");

			vector<string> ser_rooms = sc_rd.ReadArray<string>("rooms");
			rooms.resize(ser_rooms.size());
			for (int i = 0; i < ser_rooms.size(); i++) {
				rooms[i].Deserialize(ser_rooms[i]);
			}
			hash_rooms();

			local_res.Deserialize(sc_rd.ReadType<string>("local res"));
			c_room_id = sc_rd.ReadType<unsigned short>("room id");
		}

		void hash_rooms() {
			for (int i = 0; i < rooms.size(); i++) {
				room_hash[rooms[i].name] = i + 1;
			}
		}

		//void destroy() {
		//	rooms.clear();
		//
		//	local_res.destroy();
		//}
	};

	struct Project {
#ifdef EDITOR
		std::string name = u8"Без имени";
		std::vector<JTexture::BasicTexture> tmp_textures;
#endif
		
		resources global_res;
		std::vector<Scene> scenes;
		std::unordered_map<std::string, unsigned short> scene_hash;
		unsigned short c_scene_id;

		bool fullscreen = 0;
		unsigned short screen_height = 256;

		JTexture::BasicTexture icon;

		string Serialize() {
			JSerial::Template prj_tmpl = { JSerial::Type<string>("name"), 
										   JSerial::Array<string>("tmp textures"), 
										   JSerial::Type<string>("global res"), 
										   JSerial::Array<string>("scenes"), 
										   JSerial::Type<unsigned short>("scene id"), 
										   JSerial::Type<bool>("fullscreen"), 
										   JSerial::Type<unsigned short>("screen height"),
										   JSerial::Type<string>("icon") };

			JSerial::Write prj_wr(prj_tmpl);

			prj_wr.WriteType(name);
			vector<string> serialized_tmp_textures(tmp_textures.size());
			for (int i = 0; i < tmp_textures.size(); i++) {
				serialized_tmp_textures[i] = tmp_textures[i].Serialize();
			}

			prj_wr.WriteArray(serialized_tmp_textures);
			prj_wr.WriteType(global_res.Serialize());

			vector<string> serialized_scenes(scenes.size());
			for (int i = 0; i < scenes.size(); i++) {
				serialized_scenes[i] = scenes[i].Serialize();
			}

			prj_wr.WriteArray(serialized_scenes);
			prj_wr.WriteType(c_scene_id);
			prj_wr.WriteType(fullscreen);
			prj_wr.WriteType(screen_height);
			prj_wr.WriteType(icon.Serialize());

			return prj_wr.Result();
		}

		void Deserialize(string src) {
			JSerial::Template prj_tmpl = { JSerial::Type<string>("name"),
										   JSerial::Array<string>("tmp textures"),
										   JSerial::Type<string>("global res"),
										   JSerial::Array<string>("scenes"),
										   JSerial::Type<unsigned short>("scene id"),
										   JSerial::Type<bool>("fullscreen"),
										   JSerial::Type<unsigned short>("screen height"),
										   JSerial::Type<string>("icon") };

			JSerial::Read rd(prj_tmpl, src);

			name = rd.ReadType<string>("name");

			vector<string> ser_tmp_txts = rd.ReadArray<string>("tmp textures");
			tmp_textures.resize(ser_tmp_txts.size());
			for (int i = 0; i < ser_tmp_txts.size(); i++) {
				tmp_textures[i].Deserialize(ser_tmp_txts[i]);
			}

			global_res.Deserialize(rd.ReadType<string>("global res"));

			vector<string> ser_sc = rd.ReadArray<string>("scenes");
			scenes.resize(ser_sc.size());
			for (int i = 0; i < ser_sc.size(); i++) {
				scenes[i].Deserialize(ser_sc[i]);
			}
			hash_scenes();

			c_scene_id = rd.ReadType<unsigned short>("scene id");
			fullscreen = rd.ReadType<bool>("fullscreen");
			screen_height = rd.ReadType<unsigned short>("screen height");

			icon.Deserialize(rd.ReadType<string>("icon"));
		}

		gameobjs::Scene* get_curr_scene() {
			return &scenes[c_scene_id];
		}

		gameobjs::room* get_curr_room() {
			return &scenes[c_scene_id].rooms[scenes[c_scene_id].c_room_id];
		}

		resources* get_res(bool is_local) {
			return is_local ? &scenes[c_scene_id].local_res : &global_res;
		}

		void replace_tile_in_all_scenes(Tiles::Tile from, Tiles::Tile to) {
			unsigned short range[] = { (from.is_local || to.is_local) ? c_scene_id : 0,
									   (from.is_local || to.is_local) ? c_scene_id + 1 : scenes.size() };

			for (unsigned short sc_id = range[0]; sc_id < range[1]; sc_id++) {
				for (unsigned short r_id = 0; r_id < scenes[sc_id].rooms.size(); r_id++) {
					room* c_room = &scenes[sc_id].rooms[r_id];
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
					room* c_room = &scenes[sc_id].rooms[r_id];
					for (unsigned short l_id = 0; l_id < c_room->layers.size(); l_id++) {
						Layer* c_layer = &c_room->layers[l_id];
						c_layer->tmap.SwapReplace(from, to);
					}
				}
			}
		}

		/*void destroy() {
			for (Scene& i : scenes)
				i.destroy();
			scenes.clear();

			global_res.destroy();


			for (Texture::BasicTexture& i : tmp_textures)
				i.delete_texture();
			tmp_textures.clear();
		}*/
	};
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

