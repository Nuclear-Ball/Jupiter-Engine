#pragma once
#include <iostream>
#include <Bits.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <string>



using namespace std;

enum BindexTypes {
	bindex_short,
	bindex_int,
	bindex_long_long,
	bindex_float,
	bindex_double,
	bindex_template,
	bindex_array,
	bindex_string,
	bindex_pod
};

u32string its(int a) {
	return u32string{ char32_t(a) };
}

vector<int> size_table = { 1, 1, 4, 1, 5}; //short = 1, int = 2, ll = 4, float = 3, double = 5

class BindexTemplate {
public:
	vector<BindexTypes> types;
	vector<BindexTypes> array_types;
	vector<int> array_links, template_links;
	vector<BindexTemplate> templates;
	bool fixed = 0;
	int arr_pos = 0, item_id = 0;

	void add_template_array(BindexTemplate templ) {
		types.push_back(bindex_array);
		array_types.push_back(bindex_template);
		array_links.push_back(types.size() - 1);
		templates.push_back(templ);
		template_links.push_back(types.size() - 1);
	}

	void add_array(BindexTypes type) {
		types.push_back(bindex_array);
		array_types.push_back(type);
		array_links.push_back(types.size() - 1);
	}

	void add_template(BindexTemplate type) {
		types.push_back(bindex_template);
		templates.push_back(type);
		template_links.push_back(types.size() - 1);
	}

	BindexTypes find_arr() {
		if (types[item_id] == bindex_array) {
			int i = 0;
			for (i; array_links[i] != item_id; i++);
			return array_types[i];
		}
	}

	BindexTypes find_arr(int id) {
		if (types[id] == bindex_array) {
			int i = 0;
			for (i; array_links[i] != id; i++);
			return array_types[i];
		}
	}

	BindexTemplate find_template(){
		if (types[item_id] == bindex_template || find_arr() == bindex_template) {
			int i = 0;
			for (i; template_links[i] != item_id; i++);
			return templates[i];
		}
	}

	void link_arrays() {
		for (int i = 0; i < types.size(); i++) {
			if (types[i] == bindex_array) array_links.push_back(i);
		}
	}

	void link_templates() {
		for (int i = 0; i < types.size(); i++) {
			if (types[i] == bindex_template) template_links.push_back(i);
		}
	}

	void check_for_fixed() {
		fixed = 1;
		for (int i : types)
			fixed *= !(i == bindex_string || i == bindex_array);
	}
};

struct active_template_array {
	int layer = 0, number_of_elements = 0, current_element = 0, cursor_pos = 0;
};

class BindexFile {
public:
	BindexTemplate main_template;
	void test() {
		file.open(filesystem::path(u"bruhman.bruh"), ios::binary | ios::in);


	}

	/*
	void test() {
		file.open(filesystem::path(u"bruhman.bruh"), ios::binary | ios::out);
		write_str(u"Тестовая строка");
		file.close();
		file.open(filesystem::path(u"bruhman.bruh"), ios::binary | ios::in);
		u16string a = read_str(0, 15);
		cout << "test";
	}*/

	void init_write(u32string path) {
		active_templates.push_back(main_template);
		file.open(filesystem::path(path), ios::binary|ios::out);
	}

	void write_string(u32string a) {
		if (get_current_type() == bindex_string) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				reserve_adresses(1);
			write_str(its(a.size()));
			write_str(a);
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				write_jmpt();
			check_and_switch();
		}
		else cout << "Error" << endl;
	}

	void write_int(int value) {
		if (get_current_type() == bindex_int) {
			write_str(its(value));
			check_and_switch();
		}
		else cout << "Error" << endl;
	}

	void write_int_array(vector<int> arr) {
		if (get_current_type() == bindex_array && get_current_array() == bindex_int) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				reserve_adresses(1);
			write_str(its(arr.size()));
			for (int i : arr) {
				write_str(its(i));
			}
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				write_jmpt();
			check_and_switch();
		}
		else cout << "Error" << endl;
	}

	void prepare_template_array_write(int amount) {
		if (active_templates.back().find_arr() == bindex_template) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				reserve_adresses(1);
			write_str(its(amount));
			reserve_adresses(amount);
			write_jmpt();
			active_templates.push_back(active_templates.back().find_template());
			act_templ_arrs.push_back({ int(active_templates.size()), amount, 0, 0 });
		}
		else cout << "Error" << endl;
	}

	//--------------------------------------------------------------------------Чтение файла--------------------------------------------

	u32string get_entire_file() {
		file.seekg(0, ios::end);
		int size = file.tellg();
		file.seekg(0, ios::beg);
		return read_str(0, size);
	}

	void bruh() {
		vector<u32string> a;
		a.resize(10);
		int b = file.tellg();
		for (int i = 0; i < 10; i++) {
			//file.seekg(i);
			a[i] = file.get();
		}
		
		b = file.tellg();
		
		//int a = get_adress_by_cursor_pos(), tmp = file.tellg();
		//a = get_adress_by_cursor_pos(), tmp = file.tellg();
		//a = get_adress_by_cursor_pos(), tmp = file.tellg();
		//a = get_adress_by_cursor_pos(), tmp = file.tellg();
		//a = get_adress_by_cursor_pos(), tmp = file.tellg();
	}

	void init_read(u32string path) {
		active_templates.push_back(main_template);
		file.open(filesystem::path(path), ios::binary | ios::in);
	}


	int read_int() {
		if (get_current_type() == bindex_int) {
			int tmp = file.tellg();
			u32string buff = read_str(file.tellg(), 5);
			int a = read_str(file.tellg(), size_table[bindex_int])[0];
			next();
			return a;
		}
	}

	vector<int> read_int_array() {
		vector<int> a;
		int debug_tmp = file.tellg();
		tmp_cursor_pos.push_back(file.tellg());
		int array_size = get_array_size();
		a.resize(array_size);
		file.seekg(size_table[bindex_int] + addr_size * gars_m(), ios::cur);
		for (int i = 0; i < array_size; i++) {
			a[i] = get_int_value(file.tellg());
			file.seekg(size_table[bindex_int], ios::cur);
		}
		file.seekg(tmp_cursor_pos.back());
		tmp_cursor_pos.pop_back();
		next();
		return a;
	}

	u32string read_string() {
		if (get_current_type() == bindex_string) {
			int debug_tmp = file.tellg();
			tmp_cursor_pos.push_back(file.tellg());
			int strsize = get_array_size();
			file.seekg(size_table[bindex_int] + addr_size * gars_m(), ios::cur);
			u32string a = read_str(file.tellg(), strsize);
			file.seekg(tmp_cursor_pos.back());
			tmp_cursor_pos.pop_back();
			next();
			return a;
		}
	}


	int enter_template_array() {
		if (get_current_array() == bindex_template) {
			int arrs = get_array_size(), debug_tmp = file.tellg();
			active_templates.push_back(active_templates.back().find_template());
			act_templ_arrs.push_back({ int(active_templates.size()), arrs, 0, int(file.tellg()) });
			file.seekg(size_table[bindex_int] + addr_size * gars_m(), ios::cur);
			acess_element_of_template_array(0);
			return arrs;
		}
	}

	void acess_element_of_template_array(int id) {
		if (act_templ_arrs.size()) {
			int debug_tmp = file.tellg();
			file.seekg(act_templ_arrs.back().cursor_pos);
			file.seekg(size_table[bindex_int] + id * addr_size, ios::cur);
			debug_tmp = file.tellg();
			file.seekg(get_adress_by_cursor_pos());
			debug_tmp = file.tellg();
			u32string debstr = read_str(file.tellg(), 5);
			while (act_templ_arrs.back().layer > active_templates.size()) {
				active_templates.pop_back(), tmp_cursor_pos.pop_back();
			}
			act_templ_arrs.back().current_element = id;
		}
	}

	void leave_template_arr() {
		if (act_templ_arrs.size()) {
			file.seekg(act_templ_arrs.back().cursor_pos);
			while (act_templ_arrs.back().layer > active_templates.size()) {
				active_templates.pop_back(), tmp_cursor_pos.pop_back();
			}
			active_templates.pop_back();
			act_templ_arrs.pop_back();
		}
		next();
	}


	void enter_template() {
		if (get_current_type() == bindex_template) {
			int tmp = file.tellg();
			tmp_cursor_pos.push_back(tmp);
			if (active_templates.back().item_id < active_templates.back().types.size() - 1) file.seekg(addr_size, ios::cur);
			active_templates.push_back(active_templates.back().find_template());
		}
	}

	void leave_template() {
		if (get_current_type(2) == bindex_template)
			file.seekg(tmp_cursor_pos.back()), active_templates.pop_back(), tmp_cursor_pos.pop_back(), next();
	}


	void next() {
		int debug_tmp = file.tellg();
		if (active_templates.back().item_id < active_templates.back().types.size() - 1) {
			if (get_current_type() != bindex_array && get_current_type() != bindex_template && get_current_type() != bindex_pod && get_current_type() != bindex_string)
				file.seekg(size_table[get_current_type()], ios::cur);
			else file.seekg(get_adress_by_cursor_pos());
			active_templates.back().item_id++;
		}
		else {
			if (is_template_array()) {
				if (act_templ_arrs.back().current_element < act_templ_arrs.back().number_of_elements) {
					if (get_current_type() != bindex_array && get_current_type() != bindex_template && get_current_type() != bindex_pod && get_current_type() != bindex_string)
						file.seekg(size_table[get_current_type()], ios::cur);
					else file.seekg(get_adress_by_cursor_pos());
					active_templates.back().item_id = 0, act_templ_arrs.back().current_element++;
				}
				else leave_template_arr();
			}
			else leave_template();
		}
	}

	void go_to(int id) {
		int debug_tmp = file.tellg();
		if (id <= active_templates.back().types.size() - 1) {
			for (int i = 0; i < id; i++) {
				BindexTypes ct = get_current_type();
				if (ct != bindex_array && ct != bindex_template && ct != bindex_pod && ct != bindex_string)
					file.seekg(size_table[ct], ios::cur);
				else {
					file.seekg(get_adress_by_cursor_pos());
				}
				active_templates.back().item_id++;
			}
		}
	}





private:
	//u32string str;
	int cursor_pos = 0;
	const int addr_size = 1; //int
	vector<active_template_array> act_templ_arrs;
	vector<int> tmp_cursor_pos;
	vector<BindexTemplate> active_templates;
	basic_fstream<char32_t> file;

	BindexTypes get_current_type(int level = 1) {
		if (active_templates.size() >= level) {
			return active_templates[active_templates.size() - level].types[active_templates[active_templates.size() - level].item_id];
		}
	}

	BindexTypes get_current_array(int level = 1) {
		if (active_templates.size() >= level)
			return active_templates.back().find_arr();
	}

	void write_adress(int pos, int adress) {
		int tmp = file.tellg();
		file.seekg(pos);
		write_str(its(adress));
		file.seekg(tmp);
	}

	void write_jmpt() {
		write_adress(tmp_cursor_pos[tmp_cursor_pos.size() - 1], file.tellg());
		tmp_cursor_pos.pop_back();
	}

	void check_and_switch() {
		if (active_templates.back().item_id < active_templates.back().types.size() - 1) {
			active_templates.back().item_id++;
			if (get_current_type() == bindex_template)
				prepare_template_write();
		}
		else {
			if (active_templates.size() == 1) end_write();
			if (get_current_type(2) == bindex_template) end_template_write();
			if (act_templ_arrs.size() && act_templ_arrs.back().layer == active_templates.size()) {
				act_templ_arrs.back().current_element++, active_templates.back().item_id = 0;
				if (act_templ_arrs.back().current_element < act_templ_arrs.back().number_of_elements) write_jmpt();
				else end_template_array_write();
			}
		}
	}

	void reserve_adresses(int amount) {
		int tmp = file.tellg();
		for (int i = amount; i > 0; i--) write_str(U" "), tmp_cursor_pos.push_back(tmp + (i - 1) * addr_size);
	}

	void write_str(u32string str) {
		file.write(str.c_str(), str.size());
	}

	void prepare_template_write() {
		if (active_templates.back().item_id < active_templates.back().types.size() - 1)
			reserve_adresses(1);
		active_templates.push_back(active_templates.back().find_template());
	}

	void end_template_write() {
		if (active_templates[active_templates.size() - 2].item_id < active_templates[active_templates.size() - 2].types.size() - 1)
			write_jmpt();
		active_templates.pop_back();
		check_and_switch();

	}

	void end_template_array_write() {
		active_templates.pop_back();
		act_templ_arrs.pop_back();
		if (active_templates.back().item_id < active_templates.back().types.size() - 1)
			write_jmpt();
		check_and_switch();
	}

	void end_write() {
		file.close();
		active_templates.clear();
		active_templates.shrink_to_fit();
		act_templ_arrs.clear();
		act_templ_arrs.shrink_to_fit();
		tmp_cursor_pos.clear();
		tmp_cursor_pos.shrink_to_fit();
		cout << "endwrite";
	}

	//--------------------------------------------------------------------------Чтение файла--------------------------------------------

	u32string read_str(int pos, int lengh) {
		int tmp = file.tellg();
		file.seekg(pos, ios::beg);
		char32_t* buff = new char32_t[lengh];
		file.read(buff, lengh);
		u32string a(buff, lengh);
		delete[] buff;
		file.seekg(tmp);
		return a;
	}

	bool is_template_array() {
		if (act_templ_arrs.size()) {
			return act_templ_arrs.back().layer == active_templates.size();
		}
	}

	int get_array_size() {
		int tmp = file.tellg();
		if (get_current_type() == bindex_array || get_current_type() == bindex_string) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				tmp += addr_size;
			u32string a = read_str(tmp, size_table[bindex_int]);
			return a[0];
		}
	}

	bool gars_m() {
		return (get_current_type() == bindex_array || get_current_type() == bindex_string) && (active_templates.back().item_id < active_templates.back().types.size() - 1);
	}

	int get_int_value(int pos) {
		return read_str(pos, size_table[bindex_int])[0];
	}

	int get_adress_by_cursor_pos() {
		return read_str(file.tellg(), addr_size)[0];
	}
};

/*
u32string get_entire_file() {
		file.seekg(0, ios::end);
		int size = file.tellg();
		file.seekg(0, ios::beg);
		return read_str(0, size);
	}

	void bruh() {
		vector<u32string> a;
		a.resize(10);
		int b = file.tellg();
		for (int i = 0; i < 10; i++) {
			//file.seekg(i);
			a[i] = read_str(15, 5);
		}

		b = file.tellg();

		//int a = get_adress_by_cursor_pos(), tmp = file.tellg();
		//a = get_adress_by_cursor_pos(), tmp = file.tellg();
		//a = get_adress_by_cursor_pos(), tmp = file.tellg();
		//a = get_adress_by_cursor_pos(), tmp = file.tellg();
		//a = get_adress_by_cursor_pos(), tmp = file.tellg();
	}

	void init_read(u32string path) {
		active_templates.push_back(main_template);
		file.open(filesystem::path(path), ios::binary | ios::in);
	}


	int read_int() {
		if (get_current_type() == bindex_int) {
			int tmp = file.tellg();
			u32string buff = read_str(file.tellg(), 5);
			int a = read_str(file.tellg(), size_table[bindex_int])[0];
			next();
			return a;
		}
	}

	vector<int> read_int_array() {
		vector<int> a;
		int debug_tmp = file.tellg();
		tmp_cursor_pos.push_back(file.tellg());
		int array_size = get_array_size();
		a.resize(array_size);
		file.seekg(size_table[bindex_int] + addr_size * gars_m(), ios::cur);
		for (int i = 0; i < array_size; i++) {
			a[i] = get_int_value(file.tellg());
			file.seekg(size_table[bindex_int], ios::cur);
		}
		file.seekg(tmp_cursor_pos.back());
		tmp_cursor_pos.pop_back();
		next();
		return a;
	}

	u32string read_string() {
		if (get_current_type() == bindex_string) {
			int debug_tmp = file.tellg();
			tmp_cursor_pos.push_back(file.tellg());
			int strsize = get_array_size();
			file.seekg(size_table[bindex_int] + addr_size * gars_m(), ios::cur);
			u32string a = read_str(file.tellg(), strsize);
			file.seekg(tmp_cursor_pos.back());
			tmp_cursor_pos.pop_back();
			next();
			return a;
		}
	}


	int enter_template_array() {
		if (get_current_array() == bindex_template) {
			int arrs = get_array_size(), debug_tmp = file.tellg();
			active_templates.push_back(active_templates.back().find_template());
			act_templ_arrs.push_back({ int(active_templates.size()), arrs, 0, int(file.tellg()) });
			file.seekg(size_table[bindex_int] + addr_size * gars_m(), ios::cur);
			acess_element_of_template_array(0);
			return arrs;
		}
	}

	void acess_element_of_template_array(int id) {
		if (act_templ_arrs.size()) {
			int debug_tmp = file.tellg();
			file.seekg(act_templ_arrs.back().cursor_pos);
			file.seekg(size_table[bindex_int] + id * addr_size, ios::cur);
			debug_tmp = file.tellg();
			file.seekg(get_adress_by_cursor_pos());
			debug_tmp = file.tellg();
			u32string debstr = read_str(file.tellg(), 5);
			while (act_templ_arrs.back().layer > active_templates.size()) {
				active_templates.pop_back(), tmp_cursor_pos.pop_back();
			}
			act_templ_arrs.back().current_element = id;
		}
	}

	void leave_template_arr() {
		if (act_templ_arrs.size()) {
			file.seekg(act_templ_arrs.back().cursor_pos);
			while (act_templ_arrs.back().layer > active_templates.size()) {
				active_templates.pop_back(), tmp_cursor_pos.pop_back();
			}
			active_templates.pop_back();
			act_templ_arrs.pop_back();
		}
		next();
	}


	void enter_template() {
		if (get_current_type() == bindex_template) {
			int tmp = file.tellg();
			tmp_cursor_pos.push_back(tmp);
			if (active_templates.back().item_id < active_templates.back().types.size() - 1) file.seekg(addr_size, ios::cur);
			active_templates.push_back(active_templates.back().find_template());
		}
	}

	void leave_template() {
		if (get_current_type(2) == bindex_template)
			file.seekg(tmp_cursor_pos.back()), active_templates.pop_back(), tmp_cursor_pos.pop_back(), next();
	}


	void next() {
		int debug_tmp = file.tellg();
		if (active_templates.back().item_id < active_templates.back().types.size() - 1) {
			if (get_current_type() != bindex_array && get_current_type() != bindex_template && get_current_type() != bindex_pod && get_current_type() != bindex_string)
				file.seekg(size_table[get_current_type()], ios::cur);
			else file.seekg(get_adress_by_cursor_pos());
			active_templates.back().item_id++;
		}
		else {
			if (is_template_array()) {
				if (act_templ_arrs.back().current_element < act_templ_arrs.back().number_of_elements) {
					if (get_current_type() != bindex_array && get_current_type() != bindex_template && get_current_type() != bindex_pod && get_current_type() != bindex_string)
						file.seekg(size_table[get_current_type()], ios::cur);
					else file.seekg(get_adress_by_cursor_pos());
					active_templates.back().item_id = 0, act_templ_arrs.back().current_element++;
				}
				else leave_template_arr();
			}
			else leave_template();
		}
	}

	void go_to(int id) {
		int debug_tmp = file.tellg();
		if (id <= active_templates.back().types.size() - 1) {
			for (int i = 0; i < id; i++) {
				BindexTypes ct = get_current_type();
				if (ct != bindex_array && ct != bindex_template && ct != bindex_pod && ct != bindex_string)
					file.seekg(size_table[ct], ios::cur);
				else {
					file.seekg(get_adress_by_cursor_pos());
				}
				active_templates.back().item_id++;
			}
		}
	}





private:
	//u32string str;
	int cursor_pos = 0;
	const int addr_size = 1; //int
	vector<active_template_array> act_templ_arrs;
	vector<int> tmp_cursor_pos;
	vector<BindexTemplate> active_templates;
	basic_fstream<char32_t> file;

	BindexTypes get_current_type(int level = 1) {
		if (active_templates.size() >= level) {
			return active_templates[active_templates.size() - level].types[active_templates[active_templates.size() - level].item_id];
		}
	}

	BindexTypes get_current_array(int level = 1) {
		if (active_templates.size() >= level)
			return active_templates.back().find_arr();
	}

	void write_adress(int pos, int adress) {
		int tmp = file.tellg();
		file.seekg(pos);
		write_str(its(adress));
		file.seekg(tmp);
	}

	void write_jmpt() {
		write_adress(tmp_cursor_pos[tmp_cursor_pos.size() - 1], file.tellg());
		tmp_cursor_pos.pop_back();
	}

	void check_and_switch() {
		if (active_templates.back().item_id < active_templates.back().types.size() - 1) {
			active_templates.back().item_id++;
			if (get_current_type() == bindex_template)
				prepare_template_write();
		}
		else {
			if (active_templates.size() == 1) end_write();
			if (get_current_type(2) == bindex_template) end_template_write();
			if (act_templ_arrs.size() && act_templ_arrs.back().layer == active_templates.size()) {
				act_templ_arrs.back().current_element++, active_templates.back().item_id = 0;
				if (act_templ_arrs.back().current_element < act_templ_arrs.back().number_of_elements) write_jmpt();
				else end_template_array_write();
			}
		}
	}

	void reserve_adresses(int amount) {
		int tmp = file.tellg();
		for (int i = amount; i > 0; i--) write_str(U" "), tmp_cursor_pos.push_back(tmp + (i - 1) * addr_size);
	}

	void write_str(u32string str) {
		file.write(str.c_str(), str.size());
	}

	void prepare_template_write() {
		if (active_templates.back().item_id < active_templates.back().types.size() - 1)
			reserve_adresses(1);
		active_templates.push_back(active_templates.back().find_template());
	}

	void end_template_write() {
		if (active_templates[active_templates.size() - 2].item_id < active_templates[active_templates.size() - 2].types.size() - 1)
			write_jmpt();
		active_templates.pop_back();
		check_and_switch();

	}

	void end_template_array_write() {
		active_templates.pop_back();
		act_templ_arrs.pop_back();
		if (active_templates.back().item_id < active_templates.back().types.size() - 1)
			write_jmpt();
		check_and_switch();
	}

	void end_write() {
		file.close();
		active_templates.clear();
		active_templates.shrink_to_fit();
		act_templ_arrs.clear();
		act_templ_arrs.shrink_to_fit();
		tmp_cursor_pos.clear();
		tmp_cursor_pos.shrink_to_fit();
		cout << "endwrite";
	}

	//--------------------------------------------------------------------------Чтение файла--------------------------------------------

	u32string read_str(int pos, int lengh) {
		int tmp = file.tellg();
		file.seekg(pos, ios::beg);
		char32_t* buff = new char32_t[lengh];
		file.read(buff, lengh);
		u32string a(buff, lengh);
		delete[] buff;
		file.seekg(tmp);
		return a;
	}

	bool is_template_array() {
		if (act_templ_arrs.size()) {
			return act_templ_arrs.back().layer == active_templates.size();
		}
	}

	int get_array_size() {
		int tmp = file.tellg();
		if (get_current_type() == bindex_array || get_current_type() == bindex_string) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				tmp += addr_size;
			u32string a = read_str(tmp, size_table[bindex_int]);
			return a[0];
		}
	}

	bool gars_m() {
		return (get_current_type() == bindex_array || get_current_type() == bindex_string) && (active_templates.back().item_id < active_templates.back().types.size() - 1);
	}

	int get_int_value(int pos) {
		return read_str(pos, size_table[bindex_int])[0];
	}

	int get_adress_by_cursor_pos() {
		return read_str(file.tellg(), addr_size)[0];
	}

class BindexFile {
public:
	BindexTemplate main_template;

	void test() {
		file.open(filesystem::path(u"bruhman.bruh"), ios::binary | ios::out);
		write_str(u"Тестовая строка");
		file.close();
		file.open(filesystem::path(u"bruhman.bruh"), ios::binary | ios::in);
		u16string a = read_str(0, 15);
		cout << "test";
	}

	void init_write(u16string path) {
		active_templates.push_back(main_template);
		file.open(filesystem::path(path), ios::binary|ios::out);
	}

	void write_string(u16string a) {
		if (get_current_type() == bindex_string) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				reserve_adresses(1);
			write_str(Separate(a.size()));
			write_str(a);
			cursor_pos = file.tellg();
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				write_jmpt();
			check_and_switch();
		}
	}

	void write_int(int value) {
		if (get_current_type() == bindex_int) {
			write_str(Separate(value));
			cursor_pos += size_table[bindex_int];
			check_and_switch();
		}
		else cout << "Error" << endl;
	}

	void write_int_array(vector<int> arr) {
		if (get_current_type() == bindex_array && get_current_array() == bindex_int) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				reserve_adresses(1);
			write_str(Separate(arr.size()));
			cursor_pos += size_table[bindex_int];
			for (int i : arr) {
				write_str(Separate(i));
				cursor_pos += size_table[bindex_int];
			}
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				write_adress(tmp_cursor_pos[tmp_cursor_pos.size() - 1], cursor_pos), tmp_cursor_pos.pop_back();
			check_and_switch();
		}
		else cout << "Error" << endl;
	}

	void prepare_template_array_write(int amount) {
		if (active_templates.back().find_arr() == bindex_template) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				reserve_adresses(1);
			write_str(Separate(amount)), cursor_pos += size_table[bindex_int];
			reserve_adresses(amount);
			write_jmpt();
			active_templates.push_back(active_templates.back().find_template());
			act_templ_arrs.push_back({ int(active_templates.size()), amount, 0, 0 });
			//active_template_arrays.push_back(amount), active_template_arrays.push_back(active_templates.size());
		}
	}

	//--------------------------------------------------------------------------Чтение файла--------------------------------------------

	void init_read(u16string path) {
		cursor_pos = 0;
		active_templates.push_back(main_template);
		file.open(filesystem::path(path), ios::binary | ios::in);
	}

	void next() {
		if (active_templates.back().item_id < active_templates.back().types.size() - 1) {
			if (get_current_type() != bindex_array && get_current_type() != bindex_template && get_current_type() != bindex_pod && get_current_type() != bindex_string)
				cursor_pos += size_table[get_current_type()];
			else cursor_pos = get_adress_by_cursor_pos();
			active_templates.back().item_id++;
		}
		else {
			if (is_template_array()) {
				if (act_templ_arrs.back().current_element < act_templ_arrs.back().number_of_elements) {
					if (get_current_type() != bindex_array && get_current_type() != bindex_template)
						cursor_pos += size_table[get_current_type()];
					else cursor_pos = get_adress_by_cursor_pos();
					active_templates.back().item_id = 0, act_templ_arrs.back().current_element++;
				}
				else leave_template_arr();
			}
			else leave_template();
		}
	}

	void init_read() {
		cursor_pos = 0;
		active_templates.clear();
		active_templates.push_back(main_template);
	}



	int enter_template_array() {
		if (get_current_array() == bindex_template) {
			int tmp = get_array_size();
			active_templates.push_back(active_templates.back().find_template());
			act_templ_arrs.push_back({ int(active_templates.size()), tmp, 0, cursor_pos });
			cursor_pos += size_table[bindex_int];
			acess_element_of_template_array(0);
			return tmp;
		}
	}

	void acess_element_of_template_array(int id) {
		if (act_templ_arrs.size()) {
			cursor_pos = act_templ_arrs.back().cursor_pos;
			cursor_pos += size_table[bindex_int];
			cursor_pos += (id * addr_size);
			cursor_pos = get_adress_by_cursor_pos();
			while (act_templ_arrs.back().layer > active_templates.size()) {
				active_templates.pop_back(), tmp_cursor_pos.pop_back();
			}
			act_templ_arrs.back().current_element = id;
		}
	}

	void leave_template_arr() {
		if (act_templ_arrs.size()) {
			cursor_pos = act_templ_arrs.back().cursor_pos;
			while (act_templ_arrs.back().layer > active_templates.size()) {
				active_templates.pop_back(), tmp_cursor_pos.pop_back();
			}
			active_templates.pop_back();
			act_templ_arrs.pop_back();
		}
		next();
	}

	void leave_template() {
		if (get_current_type(2) == bindex_template)
			cursor_pos = tmp_cursor_pos.back(), active_templates.pop_back(), tmp_cursor_pos.pop_back(), next();
	}

	void go_to(int id) {
		if (id <= active_templates.back().types.size() - 1) {
			for (int i = 0; i < id; i++) {
				BindexTypes tmp = get_current_type();
				if (get_current_type() != bindex_array && get_current_type() != bindex_template && get_current_type() != bindex_pod && get_current_type() != bindex_string)
					cursor_pos += size_table[get_current_type()];
				else {
					cursor_pos = get_adress_by_cursor_pos();
				}
				active_templates.back().item_id++;
			}
		}
	}

	void enter_template() {
		if (get_current_type() == bindex_template) {
			tmp_cursor_pos.push_back(cursor_pos);
			if(active_templates.back().item_id < active_templates.back().types.size() - 1) cursor_pos += addr_size;
			active_templates.push_back(active_templates.back().find_template());

		}
	}

	vector<int> read_int_array() {
		vector<int> a;
		tmp_cursor_pos.push_back(cursor_pos);
		int array_size = get_array_size();
		a.resize(array_size);
		cursor_pos += size_table[bindex_int];
		for (int i = 0; i < array_size; i++) {
			a[i] = get_int_value(cursor_pos);
			cursor_pos += size_table[bindex_int];
		}
		cursor_pos = tmp_cursor_pos.back();
		tmp_cursor_pos.pop_back();
		next();
		return a;
	}

	u16string read_string() {
		if (get_current_type() == bindex_string) {
			tmp_cursor_pos.push_back(cursor_pos);
			int strsize = get_array_size();
			cursor_pos += size_table[bindex_int];
			u16string a = read_str(cursor_pos, strsize);
			cursor_pos = tmp_cursor_pos.back();
			tmp_cursor_pos.pop_back();
			next();
			return a;
		}
	}

	int read_int() {
		if (get_current_type() == bindex_int) {
			int a = Merge(read_str(cursor_pos, size_table[bindex_int]));
			next();
			return a;
		}
	}

private:
	u16string str;
	int cursor_pos = 0;
	const int addr_size = 2; //int
	vector<active_template_array> act_templ_arrs;
	vector<int> tmp_cursor_pos;
	vector<BindexTemplate> active_templates;
	basic_fstream<char16_t> file;

	BindexTypes get_current_type(int level = 1) {
		if (active_templates.size() >= level) {
			return active_templates[active_templates.size() - level].types[active_templates[active_templates.size() - level].item_id];
		}
	}

	BindexTypes get_current_array(int level = 1) {
		if (active_templates.size() >= level)
			return active_templates.back().find_arr();
	}

	void write_adress(int pos, int adress) {
		file.seekg(pos);
		write_str(Separate(adress));
		file.seekg(cursor_pos);
		//str = paste(str, Separate(adress), pos);
	}

	void write_jmpt() {
		write_adress(tmp_cursor_pos[tmp_cursor_pos.size() - 1], cursor_pos);
		tmp_cursor_pos.pop_back();
	}

	void check_and_switch() {
		if (active_templates.back().item_id < active_templates.back().types.size() - 1) {
			active_templates.back().item_id++;
			if (get_current_type() == bindex_template)
				prepare_template_write();
		}
		else {
			if (active_templates.size() == 1) end_write();
			if (get_current_type(2) == bindex_template) end_template_write();
			if (act_templ_arrs.size() && act_templ_arrs.back().layer == active_templates.size()) {
				act_templ_arrs.back().current_element++, active_templates.back().item_id = 0;
				if (act_templ_arrs.back().current_element < act_templ_arrs.back().number_of_elements) write_jmpt();
				else end_template_array_write();
			}
		}
	}

	void reserve_adresses(int amount) {
		for (int i = amount; i > 0; i--) write_str(u"  "), tmp_cursor_pos.push_back(cursor_pos + (i - 1) * addr_size);
		cursor_pos += amount * addr_size;
	}

	void write_str(u16string strg) {
		file.write(strg.c_str(), strg.size());
	}

	void prepare_template_write() {
		if (active_templates.back().item_id < active_templates.back().types.size() - 1)
			reserve_adresses(1);
		active_templates.push_back(active_templates.back().find_template());
	}

	void end_template_write() {
		if (active_templates[active_templates.size() - 2].item_id < active_templates[active_templates.size() - 2].types.size() - 1)
			write_jmpt();
		active_templates.pop_back();
		check_and_switch();

	}

	void end_template_array_write() {
		active_templates.pop_back();
		act_templ_arrs.pop_back();
		if (active_templates.back().item_id < active_templates.back().types.size() - 1)
			write_jmpt();
		check_and_switch();
	}

	void end_write() {
		file.close();
		active_templates.clear();
		active_templates.shrink_to_fit();
		act_templ_arrs.clear();
		act_templ_arrs.shrink_to_fit();
		tmp_cursor_pos.clear();
		tmp_cursor_pos.shrink_to_fit();
		cout << "endwrite";
	}

	//--------------------------------------------------------------------------Чтение файла--------------------------------------------

	u16string read_str(int pos, int lengh) {
		file.seekg(pos);
		char16_t* buff = new char16_t[lengh];
		file.read(buff, lengh);
		u16string a(buff, lengh);
		delete[] buff;
		file.seekg(cursor_pos);
		return a;
	}

	bool is_template_array() {
		if (act_templ_arrs.size()) {
			return act_templ_arrs.back().layer == active_templates.size();
		}
	}

	int get_array_size() {
		int tmp = cursor_pos;
		if (get_current_type() == bindex_array || get_current_type() == bindex_string) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				tmp += addr_size;
			cursor_pos = tmp;
			u16string a = read_str(tmp, size_table[bindex_int]);
			return Merge(a);
		}
	}

	int get_int_value(int pos) {
		return Merge(read_str(pos, size_table[bindex_int]));
	}

	int get_adress_by_cursor_pos() {
		return Merge(read_str(cursor_pos, addr_size));
	}
};

 class BindexFile {
public:
	BindexTemplate main_template;
	u16string str;
	int cursor_pos = 0;
	const int addr_size = 2; //int
	vector<active_template_array> act_templ_arrs;
	vector<int> tmp_cursor_pos;
	vector<BindexTemplate> active_templates;
	
	BindexTypes get_current_type(int level = 1) {
		if(active_templates.size() >= level)
			return active_templates[active_templates.size() - level].types[active_templates[active_templates.size() - level].item_id];
	}

	BindexTypes get_current_array(int level = 1) {
		if (active_templates.size() >= level)
			return active_templates.back().find_arr();
	}

	void init_write() {
		active_templates.push_back(main_template);
	}

	void write_adress(int pos, int adress) {
		str = paste(str, Separate(adress), pos);
	}

	void write_jmpt() {
		write_adress(tmp_cursor_pos[tmp_cursor_pos.size() - 1], cursor_pos);
		tmp_cursor_pos.pop_back();
	}

	void check_and_switch() {
		if (active_templates.back().item_id < active_templates.back().types.size() - 1) {
			active_templates.back().item_id++;
			if (get_current_type() == bindex_template)
				prepare_template_write();
		}
		else {
			if (get_current_type(2) == bindex_template) end_template_write();
			if (act_templ_arrs.size() && act_templ_arrs.back().layer == active_templates.size()) {
				act_templ_arrs.back().current_element++, active_templates.back().item_id = 0;
				if (act_templ_arrs.back().current_element < act_templ_arrs.back().number_of_elements) write_jmpt();
				else end_template_array_write();
			}
		}

	}

	void reserve_adresses(int amount) {
		for (int i = amount; i > 0; i--) str += u"  ", tmp_cursor_pos.push_back(cursor_pos + (i - 1) * addr_size);
		cursor_pos += amount * addr_size;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void write_string(u16string a) {
		if (get_current_type() == bindex_string) {
			if(active_templates.back().item_id < active_templates.back().types.size() - 1)
				str += u"  ", tmp_cursor_pos.push_back(cursor_pos);
			str += Separate(a.size());
			cursor_pos += size_table[bindex_int];
			str += a;
			cursor_pos += a.size();
			write_jmpt();
			check_and_switch();
		}
	}

	void write_int(int value) {
		if (get_current_type() == bindex_int) {
			str += Separate(value);
			cursor_pos += size_table[bindex_int];
			check_and_switch();
		}
		else cout << "Error" << endl;
	}

	void write_int_array(vector<int> arr) {
		if (get_current_type() == bindex_array && get_current_array() == bindex_int) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				str += u"  ", tmp_cursor_pos.push_back(cursor_pos), cursor_pos += addr_size;
			str += Separate(arr.size());
			cursor_pos += size_table[bindex_int];
			for (int i : arr) {
				str += Separate(i);
				cursor_pos += size_table[bindex_int];
			}
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				write_adress(tmp_cursor_pos[tmp_cursor_pos.size() - 1], cursor_pos), tmp_cursor_pos.pop_back();
			check_and_switch();
		}
		else cout << "Error" << endl;
	}


	void prepare_template_write() {
		if (active_templates.back().item_id < active_templates.back().types.size() - 1)
			str += u"  ", tmp_cursor_pos.push_back(cursor_pos), cursor_pos += addr_size;
		active_templates.push_back(active_templates.back().find_template());
	}

	void end_template_write() {
		if (active_templates[active_templates.size() - 2].item_id < active_templates[active_templates.size() - 2].types.size() - 1)
			write_jmpt();
		active_templates.pop_back();
		check_and_switch();

	}


	void prepare_template_array_write(int amount) {
		if (active_templates.back().find_arr() == bindex_template) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				tmp_cursor_pos.push_back(cursor_pos), str += u"  ", cursor_pos += addr_size;
			str += Separate(amount), cursor_pos += size_table[bindex_int];
			reserve_adresses(amount);
			write_jmpt();
			active_templates.push_back(active_templates.back().find_template());
			act_templ_arrs.push_back({ int(active_templates.size()), amount, 0, 0 });
			//active_template_arrays.push_back(amount), active_template_arrays.push_back(active_templates.size());
		}
	}

	void end_template_array_write() {
		active_templates.pop_back();
		act_templ_arrs.pop_back();
		if(active_templates.back().item_id < active_templates.back().types.size() - 1)
			write_jmpt();
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool is_template_array() {
		if (act_templ_arrs.size()) {
			return act_templ_arrs.back().layer == active_templates.size();
		}
	}

	void next() {
		if (active_templates.back().item_id < active_templates.back().types.size() - 1) {
			if (get_current_type() != bindex_array && get_current_type() != bindex_template && get_current_type() != bindex_pod && get_current_type() != bindex_string)
				cursor_pos += size_table[get_current_type()];
			else cursor_pos = get_adress_by_cursor_pos();
			active_templates.back().item_id++;
		}
		else {
			if (is_template_array()) {
				if (act_templ_arrs.back().current_element < act_templ_arrs.back().number_of_elements) {
					if (get_current_type() != bindex_array && get_current_type() != bindex_template)
						cursor_pos += size_table[get_current_type()];
					else cursor_pos = get_adress_by_cursor_pos();
					active_templates.back().item_id = 0, act_templ_arrs.back().current_element++;
				}
				else leave_template_arr();
			}
			else leave_template();
		}
	}

	void init_read() {
		active_templates.clear();
		active_templates.push_back(main_template);
		cursor_pos = 0;
	}

	int get_array_size() {
		int tmp = cursor_pos;
		if (get_current_type() == bindex_array) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				tmp += addr_size;
			cursor_pos = tmp;
			return Merge(cut(str, tmp, tmp + size_table[bindex_int]));
		}
	}

	int get_int_value(int pos) {
		return Merge(cut(str, pos, pos + size_table[bindex_int]));
	}

	int get_adress_by_cursor_pos() {
		return Merge(cut(str, cursor_pos, cursor_pos + addr_size));
	}

	void enter_template_array() {
		if (get_current_array() == bindex_template) {
			int tmp = get_array_size();
			active_templates.push_back(active_templates.back().find_template());
			act_templ_arrs.push_back({ int(active_templates.size()), tmp, 0, cursor_pos });
			cursor_pos += size_table[bindex_int];
			acess_element_of_template_array(0);
		}
	}

	void acess_element_of_template_array(int id) {
		if (act_templ_arrs.size()) {
			cursor_pos = act_templ_arrs.back().cursor_pos;
			cursor_pos += size_table[bindex_int];
			cursor_pos += (id * addr_size);
			cursor_pos = get_adress_by_cursor_pos();
			while (act_templ_arrs.back().layer > active_templates.size()) {
				active_templates.pop_back(), tmp_cursor_pos.pop_back();
			}
			act_templ_arrs.back().current_element = id;
		}
	}

	void leave_template_arr() {
		if (act_templ_arrs.size()) {
			cursor_pos = act_templ_arrs.back().cursor_pos;
			while (act_templ_arrs.back().layer > active_templates.size()) {
				active_templates.pop_back(), tmp_cursor_pos.pop_back();
			}
			active_templates.pop_back();
			act_templ_arrs.pop_back();
		}
		next();
	}

	void leave_template() {
		if (get_current_type(2) == bindex_template)
			cursor_pos = tmp_cursor_pos.back(), active_templates.pop_back(), tmp_cursor_pos.pop_back();
		next();
	}

	void go_to(int id) {
		if (id <= active_templates.back().types.size() - 1) {
			for (int i = 0; i < id; i++) {
				
				BindexTypes tmp = get_current_type();
				if (get_current_type() != bindex_array && get_current_type() != bindex_template && get_current_type() != bindex_pod)
					cursor_pos += size_table[get_current_type()];
				else {
					cursor_pos = get_adress_by_cursor_pos();
				}
				active_templates.back().item_id++;
			}
		}
	}

	void enter_template() {
		if (get_current_type() == bindex_template) {
			tmp_cursor_pos.push_back(cursor_pos);
			if(active_templates.back().item_id < active_templates.back().types.size() - 1) cursor_pos += addr_size;
			active_templates.push_back(active_templates.back().find_template());
			
		}
	}

	vector<int> read_int_array() {
		vector<int> a;
		tmp_cursor_pos.push_back(cursor_pos);
		int array_size = get_array_size();
		a.resize(array_size);
		cursor_pos += size_table[bindex_int];
		for (int i = 0; i < array_size; i++) {
			a[i] = get_int_value(cursor_pos);
			cursor_pos += size_table[bindex_int];
		}
		cursor_pos = tmp_cursor_pos.back();
		tmp_cursor_pos.pop_back();
		next();
		return a;
	}

	u16string read_string() {
		if (get_current_type() == bindex_string) {
			tmp_cursor_pos.push_back(cursor_pos);
			int strsize = get_array_size();
			cursor_pos += size_table[bindex_int];
			u16string a = cut(str, cursor_pos, cursor_pos + strsize);
			next();
			return a;
		}
	}

	int read_int() {
		if (get_current_type() == bindex_int) {
			int a = Merge(cut(str, cursor_pos, cursor_pos + size_table[bindex_int]));
			next();
			return a; 
		}
	}
};


 class BindexFile {
public:
	u16string str;
	vector<BindexTemplate> templates;
	vector<int> amount_of_objects_of_template;
	int id_of_curr_template = 0;
	int cursor_pos = 0;
	int curr_object = 0;
	const int addr_size = 2; //int

	int calculate_template_size(int template_id) { //fixed
		int a = 0;
		for (int i : templates[template_id].types)
			a += size_table[i];
		return a;
	}

	int calculate_position_in_template(int template_id, int item_id) {
		int a = 0;
		for (int i = 0; i < item_id; i++)
			a += size_table[templates[template_id].types[i]];
		return a;
	}

	int get_adress_of_adress(int template_id, int item_id) {
		if (template_id < templates.size() && item_id < amount_of_objects_of_template[template_id]) {
			int a = 0;
			for (int i = 0; i < template_id; i++) {
				if (!templates[i].fixed) a += amount_of_objects_of_template[i];
				else                     a++;
			}
			if (!templates[template_id].fixed) a += item_id;
			a *= 2;
			return a;
		}
	}

	int get_adress(int template_id, int item_id) {
		int a = get_adress_of_adress(template_id, item_id);
		u16string b = { str[a], str[a + 1] };
		return Merge(b) + templates[template_id].fixed * calculate_template_size(template_id)* item_id;
	}

	int get_int() {
		int a = get_adress(id_of_curr_template, curr_object) + calculate_position_in_template(id_of_curr_template, templates[id_of_curr_template].item_id);
		u16string b = { str[a], str[a + 1] };
		check_and_switch_template();
		return Merge(b);
	}

	void go_to() {

	}

	bool check_and_switch_template() {
		int a = 0;

		if (templates[id_of_curr_template].item_id < templates[id_of_curr_template].types.size() - 1)
			templates[id_of_curr_template].item_id++;
		else {
			templates[id_of_curr_template].arr_counter = 0;
			if (curr_object < amount_of_objects_of_template[id_of_curr_template] - 1)
				templates[id_of_curr_template].item_id = 0, curr_object++, a = !templates[id_of_curr_template].fixed;
			else if (id_of_curr_template < templates.size() - 1)
				templates[id_of_curr_template].item_id = 0, id_of_curr_template++, templates[id_of_curr_template].item_id = 0, curr_object = 0, a = 1;
		}
		return a;
	}

	int get_int(int template_id, int item_id, int in_template_id = 0) {
		id_of_curr_template = template_id, curr_object = item_id, templates[id_of_curr_template].item_id = in_template_id;
		int a = get_adress(template_id, item_id) + calculate_position_in_template(template_id, item_id);
		u16string b = {str[a], str[a + 1]};
		check_and_switch_template();
		return Merge(b);
	}

	void reserve_adresses() {
		for (int i = 0; i < templates.size(); i++) {
			if (!templates[i].fixed) {
				cursor_pos += amount_of_objects_of_template[i] * addr_size;
				for (int a = 0; a < amount_of_objects_of_template[i]; a++)
					str += u"  ";
			}
			else str += u"  ", cursor_pos += addr_size;
		}
		write_adress(id_of_curr_template, curr_object, cursor_pos);
	}

	void write_adress(int template_id, int item_id, int adress) {
		int a = get_adress_of_adress(template_id, item_id);
		str = paste(str, Separate(adress), a);
	}

	void write_adress(int pos, int adress) {
	//	int a = get_adress_of_adress(template_id, item_id);
		str = paste(str, Separate(adress), pos);
	}

	void write_int(int value) {
		if (templates[id_of_curr_template].types[templates[id_of_curr_template].item_id] == bindex_int) {
			str += Separate(value), cursor_pos += addr_size;
			if (check_and_switch_template()) write_adress(id_of_curr_template, curr_object, cursor_pos);
		}
		else cout << "Error";
	}

	void write_int_array(vector<int> arr) {
		BindexTypes a = templates[id_of_curr_template].find_arr(templates[id_of_curr_template].item_id);
		if (templates[id_of_curr_template].types[templates[id_of_curr_template].item_id] == bindex_array && templates[id_of_curr_template].find_arr(templates[id_of_curr_template].item_id) == bindex_int) {
			str += u"  ";
			str += Separate(arr.size());
			int tmp_cursor_pos = cursor_pos;
			cursor_pos += 4;
			for (int i : arr) {
				str += Separate(i);
				cursor_pos += 2;
			}
			write_adress(tmp_cursor_pos, cursor_pos);
			templates[id_of_curr_template].arr_counter++;
			if (check_and_switch_template()) write_adress(id_of_curr_template, curr_object, cursor_pos);
		}
	}
	template<class t>
	void write_pod_array(vector<t> pod){

	}

	void insert_template(BindexTemplate template_, int amount_of_objs) {
		templates.push_back(template_), amount_of_objects_of_template.push_back(amount_of_objs);
		templates[templates.size() - 1].check_for_fixed();
	}
};




 	bool fixed_size = false;
	vector<BindexTypes> vars;
	bool next_item() {
		if (curr_item < vars.size()) {
			curr_item++;
			return true;
		}
		return false;
	}
	int get_curr_item() {
		return curr_item;
	}
private:
	int curr_item = 0;
* 
	u16string line;
	void write_adress(int template_id, int item_id, int cursor_pos) {
		int tmp;
		for (int i = 0; i < template_id - 1; i++) {
			tmp += ao_data[i];
		}
		tmp += item_id;
		tmp *= 5;
		line = paste(line, u"placeholder for u16 ll value", tmp);
	}

	void reserve_indexes() {
		for (int i = 0; i < templates.size(); i++) {
			for (int a = 0; a < ao_data[i]; a++) {
				line += u"     ";
				cursor_pos += 5;
			}
		}

	}
	void set_template(BindexTemplate a, int size, int pos = -1) {
		for (int i = 0; i < a.vars.size(); i++) {
			if (a.vars[i] == bindex_string && a.fixed_size || a.vars[i] == bindex_string) {
				cerr << "Error: invalid template(template has fixed size, but also has non fixed types inside). Try fixing this by changing <name_of_your_template>.fixed_size = false";
				vector<int> ErrArr; ErrArr[3683] = 0;
			}
		}
		if (pos = -1)
			templates.push_back(a);
		else templates[pos] = a;
	}

	void next_template() {
		curr_template++;
		if (templates[curr_template].fixed_size)
			write_adress(curr_template, 0, cursor_pos);

	}
	void writeint(int num) {
		if(templates[curr_template].vars[templates[curr_template].get_curr_item()] == bindex_int)
			if(!templates[curr_template].fixed_size)
				cursor_pos += 2, line += u16Separate(num);
		else {
			vector<int> ErrArr; ErrArr[3683] = 0;
		}
			int curr_template = 0;
	vector<BindexTemplate> templates;
	vector<int> ao_data;
	int cursor_pos = 0, adress_start = 0;
	}*/