#pragma once
#include <Tilemap.h>
#include <Sprite.h>
#include <camera.h>
#include <Bits.h>
#include <String+.h>
#include <string>
#include <fcntl.h>
#include <io.h>
//#include <Sprite.h>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>



class Scene {
private:
	wstring convert_uft8_to_wstr(const string& str)
	{
		wstring_convert<codecvt_utf8_utf16<wchar_t> > myconv;
		return myconv.from_bytes(str);
	}
public:

	Camera* cams;
	Tilemap* tmaps;
	//Tileset* tsets;
	wstring name, path;
	unsigned short int nc = 0, ntm = 0, nts = 0;

	void load(wstring path) {
		setlocale(LC_ALL, "");
		int tmp1 = 0, tmp2 = 2;
		string tmptst;
		wstring tmp, tmps[3];
		this->path = path;
		ifstream file(path);
		getline(file, tmptst);
		tmp = convert_uft8_to_wstr(tmptst);
		nc = tmp[0] - 32, ntm = tmp[1] - 32;// , nts = Merge(tmp, 4);
		cams = new Camera[nc], tmaps = new Tilemap[ntm];// , tsets = new Tileset[nts];
		for (int i = 0; i < 3; i++) {
			tmp1 = tmp2, tmp2 = Find(tmp, tmp1, char(0));
			tmps[i] = cut(tmp, tmp1 + 1, tmp2);
		}
		name = tmps[0];
		tmp2 = 0, tmp1 = 0;
		for (int i = 0; i < nc; i++) {
			tmp1 = tmp2, tmp2 = Find(tmps[1], tmp1 + 1, char(1));
			tmp = cut(tmps[1], tmp1 + 1, tmp2);
			cams[i].x = wMerge(tmp), cams[i].y = wMerge(tmp, 3);
			cams[i].name = cut(tmp, 6, tmp.length());
		}
		tmp1 = 0, tmp2 = 0;
		for (int i = 0; i < ntm; i++) {
			tmp1 = tmp2, tmp2 = Find(tmps[2], tmp1, char(1));
			tmp = cut(tmps[2], tmp1 + 1, tmp2);
			tmaps[i].load(tmp);
		}
		file.close();
		/*
		tmp1 = 0, tmp2 = 0;
		for (int i = 0; i < nts; i++) {
			tmp1 = tmp2, tmp2 = find(tmps[2], tmp1, char(1));
			tmp = cut(tmps[2], tmp1 + 1, tmp2);
			//tsets[i].load(tmp, renderer, window);
		}*/
	}

	void sload(wstring str) {
		setlocale(LC_ALL, "");
		int tmp1 = 0, tmp2 = 2;
		wstring tmps[3];
		nc = str[0] - 32, ntm = str[1] - 32;
		cams = new Camera[nc], tmaps = new Tilemap[ntm];
		for (int i = 0; i < 3; i++) {
			tmp1 = tmp2, tmp2 = Find(str, tmp1, char(0));
			tmps[i] = cut(str, tmp1 + 1, tmp2);
		}
		name = tmps[0];
		tmp2 = 0, tmp1 = 0;
		for (int i = 0; i < nc; i++) {
			tmp1 = tmp2, tmp2 = Find(tmps[1], tmp1 + 1, char(1));
			str = cut(tmps[1], tmp1 + 1, tmp2);
			cams[i].x = wMerge(str), cams[i].y = wMerge(str, 4);
			cams[i].name = cut(str, 8, str.length());
		}
		tmp1 = 0, tmp2 = 0;
		for (int i = 0; i < ntm; i++) {
			tmp1 = tmp2, tmp2 = Find(tmps[2], tmp1, char(1));
			str = cut(tmps[2], tmp1 + 1, tmp2);
			tmaps[i].load(str);
		}
	}
	void export_tmap(wstring path, int id) {
		wofstream file(path);
		file.imbue(locale(locale(), new codecvt_utf8<wchar_t>()));
		file << tmaps[id].save();
		file.close();
	}

	void import_tmap(wstring path) {
		wstring line;
		wifstream file(path);
		getline(file, line);
		addtmap(L"", 0, 0);
		tmaps[ntm].load(line);
		file.close();
	}

	wstring save() {
		setlocale(LC_ALL, "");
		_setmode(_fileno(stdout), _O_U8TEXT);
		wstring assembled;
		assembled += wchar_t(nc + 32), assembled += wchar_t(ntm + 32), assembled += char(0);
		assembled += name += char(0);
		if (nc == 0) assembled += char(1);
		for (int i = 0; i < nc; i++)
			assembled += wchar_t(1), assembled += wSeparate(cams[i].x) += wSeparate(cams[i].y) += cams[i].name;
		assembled += wchar_t(1), assembled += wchar_t(0);
		if(ntm == 0) assembled += char(1);
		for (int i = 0; i < ntm; i++)
			assembled += wchar_t(1), assembled += tmaps[i].save();
		assembled += wchar_t(1), assembled += wchar_t(0);
		//wcout << assembled << endl;
		wfstream file(path, ios::out);
		file.imbue(locale(locale(), new codecvt_utf8<wchar_t>()));
		file << assembled;
		file.close();
		return assembled;
	}

	void addcam(int x, int y, wstring name) {
		Camera *tmp = new Camera[nc];
		for (int i = 0; i < nc; i++)
			tmp[i] = cams[i];
		cams = new Camera[nc + 1];
		for (int i = 0; i < nc; i++)
			cams[i] = tmp[i];
		cams[nc].x = x, cams[nc].y = y, cams[nc].name = name;
		nc += 1;
	}

	void addtmap(wstring name, int width, int heigh) {
		Tilemap* tmp = new Tilemap[ntm];
		for (int i = 0; i < ntm; i++)
			tmp[i] = tmaps[i];
		tmaps = new Tilemap[ntm + 1];
		for (int i = 0; i < ntm; i++)
			tmaps[i] = tmp[i];
		tmaps[ntm].cnew(name, width, heigh);
		ntm += 1;
	}

	void removecam(int pos) {
		Camera *tmp = new Camera[nc];
		for (int i = 0; i < nc; i++)
			tmp[i] = cams[i];
		nc -= 1;
		for (int i = pos; i < nc; i++)
			tmp[i] = tmp[i + 1];
		
		cams = new Camera[nc];
		for (int i = 0; i < nc; i++)
			cams[i] = tmp[i];
	}

	void removetmap(int pos) {
		Tilemap* tmp = new Tilemap[ntm];
		for (int i = 0; i < ntm; i++)
			tmp[i] = tmaps[i];
		ntm -= 1;
		for (int i = pos; i < ntm; i++)
			tmp[i] = tmp[i + 1];
		tmaps = new Tilemap[ntm];
		for (int i = 0; i < ntm; i++)
			tmaps[i] = tmp[i];
	}

	void rearrangecam(int from, int to) {
		Camera tmp = cams[from];
		if(from > to)
			for (int i = from; i > to; i--)
				cams[i] = cams[i - 1];
		else
			for (int i = from; i < to; i++)
				cams[i] = cams[i + 1];
		cams[to] = tmp;
	}

	void rearrangetmap(int from, int to) {
		Tilemap tmp = tmaps[from];
		if (from > to)
			for (int i = from; i > to; i--)
				tmaps[i] = tmaps[i - 1];
		else
			for (int i = from; i < to; i++)
				tmaps[i] = tmaps[i + 1];
		tmaps[to] = tmp;
	}

	void generate() {
		string path, asembled, tmp;
		int x, y;
		cout << "enter path" << endl;
		cin >> path;
		cout << "enter params" << endl;
		cin >> nc >> ntm >> nts;
		asembled += (Separate(nc) + Separate(ntm) + Separate(nts) + char(2));
		for (int i = 0; i < nc; i++) {
			cin >> x >> y;
			asembled = ((asembled + char(0)) + Separate(x)) + Separate(y);
		}
		asembled = (asembled + char(0)) + char(2);
		for (int i = 0; i < ntm; i++) {
			cin >> tmp;
			asembled = (asembled + char(0)) + tmp;
		}
		asembled = (asembled + char(0)) + char(2);
		for (int i = 0; i < nts; i++) {
			cin >> tmp;
			asembled = (asembled + char(0)) + tmp;
		}
		asembled = (asembled + char(0)) + char(2);
		cout << asembled;
		fstream file(path, ios::out);
		file << asembled;
		file.close();
	}
};