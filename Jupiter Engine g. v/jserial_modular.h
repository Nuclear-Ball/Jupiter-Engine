#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <string.h>
#include <string>
#include <fstream>
#include <codecvt>


inline void j_err(std::string msg) {
	std::cerr << msg; std::abort();
}

namespace Bits {
	template<typename T> std::string Separate(T dat) {
		return std::string(reinterpret_cast<char const*>(&dat), sizeof(T));
	}

	template<typename T> T Merge(std::string dat) {
		T a;
		memcpy(&a, dat.c_str(), sizeof(T));
		return a;
	}
}

namespace JsUtilities {
	template<typename T> std::string MemblockToString(T* memory, size_t size) {
		return std::string(reinterpret_cast<char const*>(memory), sizeof(T) * size);
	}

	void OverrideString(std::string* s1, std::string s2, size_t pos) {
		memcpy(&s1->operator[](pos), s2.data(), s2.size());
	}

	std::string ReadFile(std::string path) {

		//Открыть файл
		std::ifstream file(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(path), std::ios::binary);

		//Курсор в конец файла, чтобы узнать размер
		file.seekg(0, std::ios::end);
		
		//Позиция курсора - размер файла
		size_t filesize = file.tellg();

		//Вернуть курсор в начало
		file.seekg(0, std::ios::beg);

		//Если файл открыт некоректно (это длинное число является -1, но в виде unsigned long long)
		if (filesize == 18446744073709551615) {
			std::cerr << "ERROR: file was not successfully opened\n";
			return "";
		}

		//Создать строку под размер файла
		std::string res(filesize, ' ');

		//Прочитать файл
		file.read(const_cast<char*>(res.data()), filesize);
		file.close();

		return res;
	}

	void SaveFile(std::string path, std::string data) {
		//std::experimental::filesystem::create_directories(std::experimental::filesystem::path(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(path)));
		std::ofstream file(std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(path), std::ios::binary);
		file.write(data.c_str(), data.size());
		file.close();
	}
}

namespace JSerial {

	//Структура типа данных, требуемая для правильной работы сериализатора
	struct JsType {
		std::string key;
		int size;
		bool is_array;
		std::string type_name;
	};

	template<typename T> JsType Type(std::string key) {
		if (!std::is_pod<T>() && (typeid(T) != typeid(std::string)))
			j_err("JSERIAL ERROR: unsupported type: only strings and PODs are supported");
		return { key, sizeof(T), false, typeid(T).name() };
	}

	template<typename T> JsType Array(std::string key) {
		if (!std::is_pod<T>() && (typeid(T) != typeid(std::string)))
			j_err("JSERIAL ERROR: unsupported type: only strings and PODs are supported");
		return { key, sizeof(T), true, typeid(T).name() };
	}

	class Template {
	public:
		std::vector<JsType> types;
		std::unordered_map<std::string, size_t> typemap;

		Template() {}

		Template(std::initializer_list<JsType> lst) {
			types.resize(lst.size());
			for (int i = 0; i < lst.size(); i++) {
				types[i] = lst.begin()[i];
				typemap[types[i].key] = i + 1;
			}
		}

		template<typename T>
		void AddType(std::string key) {
			//Проверка на поддерживаемость типа(Поддерживаются только простые типы и строки)
			if(!std::is_pod<T>() && (typeid(T) != typeid(std::string)))
				j_err("unsupported type");
			types.push_back({ key, sizeof(T), false, typeid(T).name() });
			typemap[key] = types.size();
		}

		template<typename T>
		void AddArray(std::string key) {
			//Проверка на поддерживаемость типа(Поддерживаются только простые типы и строки)
			if (!std::is_pod<T>() && (typeid(T) != typeid(std::string)))
				j_err("unsupported type");
			types.push_back({ key, sizeof(T), true, typeid(T).name() });
			typemap[key] = types.size();
		}
	};


	template<typename aT = size_t> class Write {
	public:
		Template main_template;
		std::string data;

		Write() {
			if (!std::is_unsigned<aT>() || !std::is_arithmetic<aT>())
				j_err("JSERIAL ERROR: invalid adress type: must be an unsigned integer");
		}

		Write(Template templ) {
			if (!std::is_unsigned<aT>() || !std::is_arithmetic<aT>())
				j_err("JSERIAL ERROR: invalid adress type: must be an unsigned integer");
			main_template = templ;
		}

		template<typename T> void WriteType(T dat) {
			//Текущий тип
			JsType curr_type = main_template.types[item_id];

			//Если требуемый тип совпадает с текущим из шаблона
			if (typeid(T).name() == curr_type.type_name && !curr_type.is_array) {

				//Записать тип
				data += Bits::Separate<T>(dat);

				//Переместить курсор и след. элемент
				cursor_pos += curr_type.size;
				item_id++;
			}
			else j_err(std::string("type mismatch: " + std::string(typeid(T).name()) + " instead of " + curr_type.type_name + (curr_type.is_array ? " array" : "") + ", the key of the type is: " + curr_type.key));
		}

		//Специализация для строки
		template<> void WriteType(std::string dat) {
			//Текущий тип
			JsType curr_type = main_template.types[item_id];

			//Если требуемый тип совпадает с текущим типом из шаблона
			if (curr_type.type_name == typeid(std::string).name() && !curr_type.is_array) {

				//Выделить место под адрес след. элемента в заголовке
				ReserveAdresses(1);

				//Записать размер строки
				data += Bits::Separate<aT>(dat.size());
				cursor_pos += sizeof(aT);

				//Сама строка
				data += dat;
				cursor_pos += dat.size();

				//Записать адрес
				WriteJmpt();

				//След. элемент
				item_id++;
			}
			else j_err(std::string("type mismatch: string instead of " + curr_type.type_name + (curr_type.is_array ? " array" : "") + ", the key of the type is: " + curr_type.key));
		}

		template<typename T> void WriteArray(std::vector<T> dat) {
			//Текущий тип
			JsType curr_type = main_template.types[item_id];

			//Если требуемый тип совпадает с текущим типом из шаблона
			if (typeid(T).name() == curr_type.type_name && curr_type.is_array) {

				//Выделить место под адрес след. элемента в заголовке
				ReserveAdresses(1);

				//Записать размер массива
				data += Bits::Separate<aT>(dat.size());
				cursor_pos += sizeof(aT);

				//Выделить место под данные массива
				data.resize(data.size() + dat.size() * curr_type.size);

				//Записать данные массива
				memcpy(&data[cursor_pos], dat.data(), dat.size() * curr_type.size);
				cursor_pos += curr_type.size * dat.size();

				//Записать адрес след. элемента
				WriteJmpt();

				//След. элемент
				item_id++;
			}
			else j_err(std::string("type mismatch: " + std::string(typeid(T).name()) + " array instead of " + curr_type.type_name + (curr_type.is_array ? " array" : "") + ", the key of the type is: " + curr_type.key));
		}

		//Специализация для строкового массива
		template<> void WriteArray(std::vector<std::string> dat) {
			//Текущий тип
			JsType curr_type = main_template.types[item_id];

			//Если требуемый тип совпадает с текущим типом из шаблона
			if (curr_type.type_name == typeid(std::string).name() && curr_type.is_array) {

				//Выделить место под адрес след. элемента в заголовке
				ReserveAdresses(1);

				//Записать количество элементов массива
				data += Bits::Separate<aT>(dat.size());
				cursor_pos += sizeof(aT);

				//Зарезервировать адреса под таблицу быстрых переходов
				ReserveAdresses(dat.size());

				//Пройтись по всем элементам и записать их
				for (std::string& i : dat) {
					//Записать адрес элемента в таблицу адресов
					WriteJmpt();

					//Записать элемент
					data += Bits::Separate<aT>(i.size()) + i;
					cursor_pos += sizeof(aT) + i.size();
				}

				//Записать адрес следующего элемента
				WriteJmpt();

				//След. элемент
				item_id++;
			}
			else j_err(std::string("type mismatch: string array instead of " + curr_type.type_name + (curr_type.is_array ? " array" : "") + ", the key of the type is: " + curr_type.key));
		}

		//Возврат в начальную конфигурацию с сохранением шаблона
		void reset() {
			cursor_pos = 0;
			item_id = 0;
			data.clear();
		}

	private:
		aT cursor_pos = 0;
		aT item_id = 0;
		std::vector<aT> tmp_cursor_pos;

		//Функция для записи в таблицу быстрых переходов позиции курсора
		void WriteJmpt() {
			JsUtilities::OverrideString(&data, Bits::Separate<aT>(cursor_pos), tmp_cursor_pos.back());
			tmp_cursor_pos.pop_back();
		}

		void ReserveAdresses(aT amount) {
			data.resize(data.size() + sizeof(aT) * amount);
			for (aT i = amount; i > 0; i--) tmp_cursor_pos.push_back(cursor_pos + (i - 1) * sizeof(aT));
			cursor_pos += amount * sizeof(aT);
		}
	};


	template<typename aT = size_t> class Read {
	public:
		Template main_template;

		Read() {
			if (!std::is_unsigned<aT>() || !std::is_arithmetic<aT>())
				j_err("JSERIAL ERROR: invalid adress type: must be an unsigned integer");
		}

		Read(std::string data, Template templ) {
			if (!std::is_unsigned<aT>() || !std::is_arithmetic<aT>())
				j_err("JSERIAL ERROR: invalid adress type: must be an unsigned integer");
			init(data, templ);
		}

		void init(std::string data, Template templ) {
			main_template = templ;
			this->data = data;
			aT max = 0 - 1;
			if (data.size() >= max)
				j_err("JSERIAL ERROR: source string size is larger than a max value of adress type");

			cached_type_positions.resize(main_template.types.size());
			aT cursor_pos = 0;

			//Кэшировать расположение типов в строке для более быстрого доступа
			for (int i = 0; i < main_template.types.size(); i++) {
				//Записать кэшированную позицию с пред. итерации
				cached_type_positions[i] = cursor_pos;

				//Текущий тип в итерации
				JsType curr_type = main_template.types[i];

				//Если тип является простой структурой данных, то можно просто прибавить размер типа
				if (!curr_type.is_array && curr_type.type_name != typeid(std::string).name())
					cursor_pos += curr_type.size;

				//Если тип либо массив, либо строка, то значение адреса след. элемента будет находится в начале заголовка массива/строки
				else
					cursor_pos = Bits::Merge<aT>(this->data.substr(cursor_pos, sizeof(aT)));
			}
		}

		template<typename T> T ReadType(std::string key) {
			aT curr_type_id = GetTypeIdFromKey(key);
			JsType curr_type = main_template.types[curr_type_id];

			//Если требуемый тип соответствует типу из шаблона, то прочитать и вернуть
			if (curr_type.type_name == typeid(T).name() && !curr_type.is_array)
				return Bits::Merge<T>(data.substr(cached_type_positions[curr_type_id], curr_type.size));

			//Иначе вывести ошибку
			else j_err(std::string("JSERIAL ERROR: type mismatch: " + std::string(typeid(T).name()) + " instead of " + curr_type.type_name + (curr_type.is_array ? " array" : "") + ", the key of the type is: " + curr_type.key + ". Serializer type is READ"));
		}

		template<> std::string ReadType(std::string key) {
			aT curr_type_id = GetTypeIdFromKey(key);
			JsType curr_type = main_template.types[curr_type_id];

			if (curr_type.type_name == typeid(std::string).name() && !curr_type.is_array) {

				//Начальная позиция области со строкой
				aT start_pos = cached_type_positions[curr_type_id] + sizeof(aT);

				//Сама строка
				return data.substr(start_pos + sizeof(aT), Bits::Merge<aT>(data.substr(start_pos, sizeof(aT))));
			}
			else j_err(std::string("JSERIAL ERROR: type mismatch: string instead of " + curr_type.type_name + (curr_type.is_array ? " array" : "") + ", the key of the type is: " + curr_type.key + ". Serializer type is READ"));
		}

		template<typename T> std::vector<T> ReadArray(std::string key) {
			aT curr_type_id = GetTypeIdFromKey(key);
			JsType curr_type = main_template.types[curr_type_id];

			//Если требуемый тип соответствует типу из шаблона
			if (curr_type.type_name == typeid(T).name() && curr_type.is_array) {
				

				//Начальная позиция массива
				aT start_pos = cached_type_positions[curr_type_id] + sizeof(aT);

				//Узнать размер массива из заголовка
				aT arr_size = Bits::Merge<aT>(data.substr(start_pos, sizeof(aT)));
				start_pos += sizeof(aT);

				//Создать массив под размеры данных
				std::vector<T> res(arr_size);

				//Копировать данные в память массива
				memcpy(res.data(), &data[start_pos], arr_size * curr_type.size);

				return res;
			}
			//Иначе вывести ошибку
			else j_err(std::string("JSERIAL ERROR: type mismatch: " + std::string(typeid(T).name()) + " array instead of " + curr_type.type_name + (curr_type.is_array ? " array" : "") + ", the key of the type is: " + curr_type.key + ". Serializer type is READ"));
		}

		template<> std::vector<std::string> ReadArray(std::string key) {
			aT curr_type_id = GetTypeIdFromKey(key);
			JsType curr_type = main_template.types[curr_type_id];

			//Если требуемый тип соответствует типу из шаблона
			if (curr_type.type_name == typeid(std::string).name() && curr_type.is_array) {
				

				//Начальная позиция области со строкой
				aT start_pos = cached_type_positions[curr_type_id] + sizeof(aT);

				//Узнать размер массива
				aT arr_size = Bits::Merge<aT>(data.substr(start_pos, sizeof(aT)));
				start_pos += sizeof(aT);

				std::vector<std::string> res(arr_size);

				//Пропустить таблицу быстрых переходов
				start_pos += arr_size * sizeof(aT);

				//Пройтись по всем строкам
				for (aT i = 0; i < arr_size; i++) {

					//Размер строки
					aT str_size = Bits::Merge<aT>(data.substr(start_pos, sizeof(aT)));
					start_pos += sizeof(aT);

					//Добавить строку в переменную результата
					res[i] = data.substr(start_pos, str_size);
					start_pos += str_size;
				}
				return res;
			}
			else j_err(std::string("JSERIAL ERROR: type mismatch: string array instead of " + curr_type.type_name + (curr_type.is_array ? " array" : "") + ", the key of the type is: " + curr_type.key + ". Serializer type is READ"));
		}

		aT GetArraySize(std::string key) {
			aT curr_type_id = GetTypeIdFromKey(key);
			JsType curr_type = main_template.types[curr_type_id];

			//Если тип - массив
			if (curr_type.is_array)
				//Вернуть значение из заголовка массива
				return Bits::Merge<aT>(data.substr(cached_type_positions[curr_type_id] + sizeof(aT), sizeof(aT)));
			else j_err("JSERIAL ERROR: type " + char(34) + key + char(34) + " is not an array");
		}

		template<typename T> T GetElementOfArray(std::string key, aT id) {
			aT curr_type_id = GetTypeIdFromKey(key);
			JsType curr_type = main_template.types[curr_type_id];

			//Если тип массив и совпадает с типом из шаблона
			if (curr_type.type_name == typeid(T).name() && curr_type.is_array) {

				//Стартовая позиция
				aT start_pos = cached_type_positions[curr_type_id] + sizeof(aT);

				//Узнаь размер массива
				aT arr_size = Bits::Merge<aT>(data.substr(start_pos, sizeof(aT)));

				//Проверка на то, находится ли элемент в границах массива
				if (id < arr_size)
					//Вернуть элемент                          Пропустить заголовок
					return Bits::Merge<T>(data.substr(start_pos + sizeof(aT) + curr_type.size * id, curr_type.size));
				else j_err(std::string("JSERIAL ERROR: trying to access element of an array out of bounds: " + std::to_string(id) + " when max size is " + std::to_string(arr_size)));
			}
			else j_err(std::string("JSERIAL ERROR: type mismatch: " + std::string(typeid(T).name()) + " array instead of " + curr_type.type_name + (curr_type.is_array ? " array" : "") + ", the key of the type is: " + curr_type.key + ". Serializer type is READ"));
		}

		template<> std::string GetElementOfArray(std::string key, aT id) {
			//Индекс текущего элемента
			aT curr_type_id = GetTypeIdFromKey(key);

			//Тип по ключу
			JsType curr_type = main_template.types[curr_type_id];

			if (curr_type.type_name == typeid(std::string).name() && curr_type.is_array) {
				aT start_pos = cached_type_positions[curr_type_id] + sizeof(aT);

				//Размер массива
				aT arr_size = Bits::Merge<aT>(data.substr(start_pos, sizeof(aT)));

				if (id < arr_size) {
					//Начало элемента в массиве. Получено через адреса в таблице переходов, распологающейся в заголовке после ячейки с количеством элементов массива
					aT string_start_pos = Bits::Merge<aT>(data.substr(start_pos + sizeof(aT) * (id + 1)));

					//Вернуть строку
					return data.substr(string_start_pos + sizeof(aT),  Bits::Merge<aT>(data.substr(string_start_pos, sizeof(aT))));
				}
				else j_err(std::string("JSERIAL ERROR: trying to access element of an array out of bounds: " + std::to_string(id) + " when max size is " + std::to_string(arr_size)));
			}
			else j_err(std::string("JSERIAL ERROR: type mismatch: string array instead of " + curr_type.type_name + (curr_type.is_array ? " array" : "") + ", the key of the type is: " + curr_type.key + ". Serializer type is READ"));
		}
	private:

		//Главная задача этой фукнкции - проверка правильности ключа
		aT GetTypeIdFromKey(std::string key) {
			aT id = main_template.typemap[key];
			if (id)
				return id - 1;
			j_err("JSERIAL ERROR: type with that key doesn't exists: " + key);
		}

		std::string data;
		std::vector<aT> cached_type_positions;
	};
}