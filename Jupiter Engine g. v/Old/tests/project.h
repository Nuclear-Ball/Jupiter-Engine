#pragma once
#pragma warning(disable : 4996)
#include <fstream>
#include <iostream>
#include <Bits.h>
#include <scene.h>
#include <String+.h>
#include <string>
#include <direct.h>
#include <systempath.h>
#include <codecvt>
#include <locale>
#include <fcntl.h>
#include <io.h>

using namespace std;

class Project {
public:
	int ns = 0;
	wstring name, path, fullpath;
	Scene* scenes;
	void loadproj(wstring path) {
		setlocale(LC_ALL, "");
		int tmp1 = 0, tmp2 = 0;
		wstring line;
		this->path = path;
		wifstream file(path);
		getline(file, line);
		ns = line[0] - 32;
		name = cut(line, 2, Find(line, 2, char(0)));
		scenes = new Scene[ns];
		tmp2 = Find(line, tmp1, char(0));
		for (int i = 0; i < ns; i++) {
			tmp1 = tmp2, tmp2 = Find(line, tmp1, char(0));
			if (cut(line, tmp1 + 1, tmp2).at(0) == char(1))
				scenes[i].load(GetPath() + cut(line, tmp1 + 2, tmp2)), scenes[i].path = GetPath() + cut(line, tmp1 + 2, tmp2);
			else scenes[i].path = cut(line, tmp1 + 1, tmp2), scenes[i].load(scenes[i].path);
		}
		file.close();
	}

	/*void createproj(string name, string path) {
		fstream file(path, ios::out);
		file << Separate(0);
		file << name << char(0);
		file.close();
		this->path = path;
	}*/

	void addscene(wstring name) {
		wstring tmp1 = path + L"/" + name + L"/";
		Scene* tmp = new Scene[ns];
		for (int i = 0; i < ns; i++)
			tmp[i] = scenes[i];
		scenes = new Scene[ns + 1];
		for (int i = 0; i < ns; i++)
			scenes[i] = tmp[i];
		scenes[ns].name = name, scenes[ns].path = tmp1 + L"scenedata.bin";
		ns += 1;
		_wmkdir(tmp1.c_str());
	}
	void save() {
		setlocale(LC_ALL, "");
		_setmode(_fileno(stdout), _O_U8TEXT);
		wofstream file(path + L"/scenedata.bin");
		file << wchar_t(ns + 32);
		file << name << char(0);
		for (int i = 0; i < ns; i++) {
			scenes[i].save();
			if (contains(path, GetPath()))
				file << char(1) << cut(scenes[i].path, GetPath().length(), scenes[i].path.length()) << char(0);
			else file << scenes[i].path << char(0);
		}
		file.close();
	}
};