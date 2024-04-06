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

using namespace std;

class Project{
public:
	int ns = 0;
	string name, path, fullpath;
	Scene *scenes;
	void loadproj(string path) {
		//setlocale(LC_ALL, "Russian");
		int tmp1 = 0, tmp2 = 0;
		string line;
		this->path = path;
		//dir = cut(this->path, 0, find(this->path, 0, '/') + 1);
		fstream file(path, ios::in);
		getline(file, line);
		//cout << line << endl;
		ns = Merge(line, 0);
		name = cut(line, 2, Find(line, 2, char(0)));
		scenes = new Scene[ns];
		tmp2 = Find(line, tmp1, char(0));
		for (int i = 0; i < ns; i++) {
			tmp1 = tmp2, tmp2 = Find(line, tmp1, char(0));
			//cout << cut(line, tmp1 + 1, tmp2) << endl;
			if(cut(line, tmp1 + 1, tmp2).at(0) == char(1))
				scenes[i].load(GetPath() + cut(line, tmp1 + 2, tmp2)), scenes[i].path = GetPath() + cut(line, tmp1 + 2, tmp2);
			else scenes[i].path = cut(line, tmp1 + 1, tmp2), scenes[i].load(scenes[i].path);
			//cout << cut(line, tmp1 + 1, tmp2);
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

	void addscene(string name) {
		//setlocale(LC_ALL, "Russian");
		string tmp1 = path + "/" + name + char(92);
		Scene* tmp = new Scene[ns];
		for (int i = 0; i < ns; i++)
			tmp[i] = scenes[i];
		scenes = new Scene[ns + 1];
		for (int i = 0; i < ns; i++)
			scenes[i] = tmp[i];
		scenes[ns].name = name, scenes[ns].path = tmp1 + name + ".bin";
		ns += 1;
		mkdir(tmp1.c_str());
		//cams[ns].x = x, cams[ns].y = y, cams[ns].name = name;
	}
	void save() {
		//setlocale(LC_ALL, "Russian");
		fstream file(path + "/" + name + ".bin", ios::out);
		file << Separate(ns);
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