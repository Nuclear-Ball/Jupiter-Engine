#pragma once
#include <string>


using namespace std;

u16string stws(string s) {
	u16string tmp(s.begin(), s.end());
	return tmp;
}
/*
int Find(string data, int pos, char symbol, int mode = 0) {
	int i = 0;
	if (!mode)
		for (i = pos; i < data.length() - 1; i++)
			if (data[i + 1] == symbol)
				return i + 1;
	if (mode)
		for (i = data.length(); i > pos; i--)
			if (data[i - 1] == symbol)
				return i - 1;
	return i;
}

string cut(string data, int pos1, int pos2) {
	string tmp;
	for (int i = pos1; i < pos2; i++)
		tmp += data[i];
	return tmp;
}
*/
bool contains(string a, string b) {
	for (int i = 0; i < b.length(); i++)
		if (a[i] != b[i])
			return 0;
	return 1;
}

int Find(wstring data, int pos, wchar_t symbol, bool mode = 0) {
	int i = 0;
	if (!mode)
		for (i = pos; i < data.length() - 1; i++)
			if (data[i + 1] == symbol)
				return i + 1;
	if (mode)
		for (i = data.length(); i > pos; i--)
			if (data[i - 1] == symbol)
				return i - 1;
	return i;
}

u16string cut(u16string data, int pos1, int pos2) {
	return u16string(data.begin() + pos1, data.end() - (data.size() - pos2));
}

u16string paste(u16string data, u16string to_copy, int pos) {
	//wstring::iterator itr = data.begin() + pos1;
	//char16_t *buff = (char16_t*)data.c_str();
	//char16_t* buff2 = buff;
	u16string tmp1(data.begin(), data.end() - (data.size() - pos));
	tmp1 += to_copy += u16string(data.begin() + pos + to_copy.size(), data.end());
	//to_copy.copy(buff, to_copy.size(), pos1);
	//for (int i = pos1; i < pos2; i++)
		//tmp += data[i];
	//return buff;
	return tmp1;
}



wstring replace(wstring line, wchar_t from, wchar_t to) {
	wstring tmp = line;
	for (int i = 0; i < line.length(); i++)
		if (line[i] == from) tmp[i] = to;
	return tmp;
}



bool wcontains(wstring a, wstring b) {
	for (int i = 0; i < b.length(); i++)
		if (a[i] != b[i])
			return 0;
	return 1;
}