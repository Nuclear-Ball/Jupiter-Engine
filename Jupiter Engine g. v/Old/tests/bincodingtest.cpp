#pragma warning(disable : 4996)
#include <iostream>
//#include <filewrite.h>
#include <objects.h>

using namespace std;

int main() {
	je_core::Scene sc;
	sc.add(je_core::txtre, 10);
	sc.save_scene(u"bruhman.bruh");
	//je_core::je_file file;
	//vector<je_core::texture> txts;
	//file.openscene(u"bruhman.bruh");
	//txts = file.get_textures();
	//txts.resize(10);
	//for (int i = 0; i < 10; i++) {
		//txts[i].dat = "randstr";
	//}
	//file.file.open(filesystem::path(u"bruhman.bruh"), ios::out | ios::binary);
	//auto br = file.save_textures(txts);
	//file.savescene(br);
	cout << "end";
	//je_core::je_file file;
}



/*
 
	BindexTemplate templ;
	templ.types.push_back(bindex_string);
	BindexTemplate _scene, _rooms, _textures, _tsets, _prefabs, _layers;

	_textures.types = {bindex_int, bindex_int, bindex_string, bindex_string };
	_scene.add_template_array(_textures);
	BindexFile file;
	file.main_template = _scene;

	file.init_write(U"bruhman.bruh");
	file.prepare_template_array_write(4);
	for (int i = 0; i < 4; i++) {
		file.write_int(10);
		file.write_int(16);
		file.write_string(U"Имя   ");
		file.write_string(U"Загрузочная строка");
	}
	//u16string name, dat;
	//file.init_read(u"bruhman.bruh");
	//name = file.read_string();
	//dat = file.read_string();
	
	u32string name, dat, whole_file;
	file.init_read(U"bruhman.bruh");
	//whole_file = file.get_entire_file();
	file.bruh();
	//whole_file = file.get_entire_file();
	for (int i = 0; i < file.enter_template_array(); i++) {
		cout << file.read_int() << endl;
		cout << file.read_int() << endl;
		name = file.read_string();
		dat = file.read_string();
		cout << "end_iter" << endl;
	}
	//file.write_string(u"Теееставая строка");

	//file.init_read(u"bruhman.bruh");
	
	//u16string a = file.read_string();
	cout << "end";

Серьезность	Код	Описание	Проект	Файл	Строка	Столбец	Состояние подавления
Ошибка	C4996	'std::codecvt<char16_t,char,mbstate_t>::id': warning STL4020: std::codecvt<char16_t, char, mbstate_t>, std::codecvt<char32_t, char, mbstate_t>, std::codecvt_byname<char16_t, char, mbstate_t>, and std::codecvt_byname<char32_t, char, mbstate_t> are deprecated in C++20 and replaced by specializations with a second argument of type char8_t. You can define _SILENCE_CXX20_CODECVT_FACETS_DEPRECATION_WARNING or _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS to acknowledge that you have received this warning.	Jupiter Engine g. v	C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.30.30705\include\xlocale	426	40

	BindexTemplate templ1, templ2;

	templ1.types = {bindex_int, bindex_int};

	templ1.add_array(bindex_int);

	templ2.types = { bindex_int, bindex_int };
	templ1.add_template(templ2);
	templ1.add_template_array(templ2);
	BindexFile file;
	file.main_template = templ1;
	vector<int> a = { 1, 5, 9, 11, 25, 0, 8, 7, 55}, b;
	file.init_write();
	file.write_int(0);
	file.write_int(1);
	file.write_int_array(a);

	file.write_int(3);
	file.write_int(4);
	//cout << "working";
	file.prepare_template_array_write(4);
	for(int i = 0; i < 4; i++)
		file.write_int(i), file.write_int(i + 10);

	file.init_read();
	cout << file.read_int() << endl;
	cout << file.read_int() << endl;

	b = file.read_int_array();

	file.enter_template();
	for (int i = 0; i < 2; i++)
		cout << file.read_int() << endl;
	file.enter_template_array();
	for(int i = 0; i < 8; i++)
		cout << file.read_int() << endl;
	cout << "end";




	BindexTemplate templ1, templ2;
	//templ1.fixed = 1;
	templ1.types.push_back(bindex_int);
	templ1.types.push_back(bindex_int);
	templ1.types.push_back(bindex_int);
	templ1.add_array(bindex_int);
	vector<int> a = { 1, 5, 9, 11, 25, 0, 8, 7, 55 };
	//templ2.fixed = 1;
	templ2.types.push_back(bindex_int);
	templ2.types.push_back(bindex_int);

	BindexFile file;
	file.insert_template(templ1, 2);
	file.insert_template(templ2, 3);
	file.reserve_adresses();

	for (int i = 0; i < 2; i++)
		file.write_int(i), file.write_int(i), file.write_int(i), file.write_int_array(a);

	for (int i = 0; i < 3; i++)
		file.write_int(i), file.write_int(i);
	file.id_of_curr_template = 0, file.curr_object = 0;
	//cout << file.get_int();
	for (int i = 0; i < 2; i++) {
		for (int b = 0; b < 3; b++)
			cout << file.get_int() << " ";
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < 3; i++) {
		for (int b = 0; b < 2; b++)
			cout << file.get_int() << " ";
		cout << endl;
	}
 
 
 
 

#include <fstream>
#include <Bits.h>
#include <vector>
#include <String+.h>
#include <mparser.h>
#include <cstdint>
#include <cstring>

enum pattern_types {
	unsigned_short_,
	int_,
	long_int_,
	unsigned_,
	float_,
	string_,
	pattern_
};

struct pattern {
	vector<pattern_types> pattrn;
};

class Bincoding {
public:
	wstring str;
	vector<int> initData;
	int different_classes = 0;
	vector<int> type_size;
	vector<pattern_types> pattern;
	const int llsize = 5, intsize = 3, floatsize = 4;
	void initdata(vector<int> data) {
		//ao_initdata = data.size();
		initData = data;
		for (int i = 0; i < initData.size(); i++) {
			str += wchar_t(initData[i]);
		}
	}

	void addtype(int elements_amount) {
		type_size.push_back(elements_amount);
	}

	void writetypes() {
		for (int i = 0; i < type_size.size(); i++) {
			str += wchar_t(type_size[i]);
		}

		for (int i = 0; i < type_size.size(); i++) {
			str.resize(str.size() + type_size[i] * llsize * 2);
		}
	}

	void setaddr(adress addr, int type, int item_id) {
		type_size[type] + item_id * llsize * 2;
		for (int i = 0; i < llsize * 2; i += llsize) {
			str[type_size[type] + item_id * llsize * 2 + i] = wSeparateLL(0)[0];
		}
		
	}
	
};
*/
/*
class testclass {
public:
	string a = "", b, c;
};
*/
