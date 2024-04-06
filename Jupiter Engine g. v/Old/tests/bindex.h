#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

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

int Merge(u16string line, int pos = 0) {
	return (line[pos] << 16) + line[pos + 1];
}

long long MergeLL(u16string line, int pos = 0) {
	return (line[pos] << 48) + (line[pos + 1] << 32) + (line[pos + 2] << 16) + line[pos + 3];
}

float MergeF(u16string line, int pos = 0) {
	return Merge(line, pos + 1) * pow(10, line[pos] * -1);
}

double MergeD(u16string line, int pos = 0) {
	return MergeLL(line, pos + 1) * pow(10, line[pos] * -1);
}


u16string Separate(int num) {
	return u16string{ char16_t(num >> 16), char16_t(num) };
}

u16string SeparateF(float num) {
	int i = 0;
	float a = num;
	for (i; a - int(a) != 0; i++) a *= 10;
	return u16string{ char16_t(i) } + Separate(a);
}

vector<int> size_table = { 1, 2, 4, 3, 5}; //short = 1, int = 2, ll = 4, float = 3, double = 5

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
		u16string a = { 10 }, b;
		file << a.c_str();
		file.close();
		file.open(filesystem::path(u"bruhman.bruh"), ios::binary|ios::in);
		char16_t* buff = new char16_t[1];
		file.seekg(0);
		file.read(buff, 1);
		b = buff;
		delete[] buff;
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
			cursor_pos += size_table[bindex_int];
			write_str(a);
			cursor_pos += a.size();
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
		active_templates.clear();
		active_templates.push_back(main_template);
		cursor_pos = 0;
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
		if (active_templates.size() >= level)
			return active_templates[active_templates.size() - level].types[active_templates[active_templates.size() - level].item_id];
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
		if (get_current_type() == bindex_array) {
			if (active_templates.back().item_id < active_templates.back().types.size() - 1)
				tmp += addr_size;
			cursor_pos = tmp;
			return Merge(read_str(tmp, size_table[bindex_int]));
		}
	}

	int get_int_value(int pos) {
		return Merge(read_str(pos, size_table[bindex_int]));
	}

	int get_adress_by_cursor_pos() {
		return Merge(read_str(cursor_pos, addr_size));
	}
};