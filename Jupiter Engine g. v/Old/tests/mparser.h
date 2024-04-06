#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Bits.h>
#include <Tilemap.h>
//#include <Sprite.h>
#include <objects.h>
#include <string>
//#include <codecvt>
#include <String+.h>

using namespace std;



struct adress {
	long long beg, end;
};

//Scene sc;

class QuickParser {
private:
	const int types = 9;
	short* amount = new short[types];
	adress* t_adress = new adress[types];
public:
	wfstream file;
	short &tmaps = amount[0], &cams = amount[1], &objs = amount[2], &points = amount[3], &sprites = amount[4], &scripts = amount[5], &tsets = amount[6], &prefabs = amount[7], &mdls = amount[8];
	adress& tma = t_adress[0], & cma = t_adress[1], & oba = t_adress[2], & pta = t_adress[3], & spa = t_adress[4], & sca = t_adress[5], & tsa = t_adress[6], & pra = t_adress[7], & mda = t_adress[8];
	void init(wstring path) {
		file.open(path);
		for (int i = 0; i < types; i++) {
			file.seekg(i), amount[i] = file.get(), t_adress[i].beg = types;
			for (int a = 0; a < i; a++)
				t_adress[i].beg += amount[a] * 5;
			t_adress[i].end = t_adress[i].beg += amount[i] * 5;
		}
	}

	adress getadress(adress start, short item) {
		wstring a;
		adress b;
		for (int i = 0; i < 10; i++)
			file.seekg(start.beg + item * 10 + i), a += file.get();
		b.beg = wMergeLL(a), b.end = wMergeLL(a, 5);
		return b;
	}

	short getvalue(adress adr) {
		short a;
		file.seekg(adr.beg), a = file.get();
		return a;
	}

	tile load_tile_from_tmap(int tmap_id, int x, int y, int width) {
		adress adr = getadress(tma, tmap_id);
		wstring a;
		tile t;
		adr.beg += (x * 2 + y * 2 * width), t.tile = getvalue(adr),adr.beg += 1, t.tset = getvalue(adr);
		t.loaded = 1;
		return t;
	}
};

wstring save_scene(Scene sc) {
	wstring a;
	long long tmp = 4, tmp1, tmp2;
	a += wchar_t(sc.Layers.size()), a += wchar_t(sc.tf.size()), a += wchar_t(sc.tsets.size()), a += wchar_t(sc.tsets.size());
	tmp += wchar_t(sc.Layers.size() * 5 * 2), tmp += wchar_t(sc.tf.size() * 5 * 2), tmp += wchar_t(sc.tsets.size() * 5 * 2), tmp += wchar_t(sc.cams.size() * 5 * 2), tmp -= 1;
	for (int i = 0; i < sc.Layers.size(); i++) {
		tmp++;
		a += wSeparateLL(tmp);
		tmp += sc.Layers[i].tmap.gheigh() * sc.Layers[i].tmap.gwidth() * 2 + 1;
		a += wSeparateLL(tmp);
	}
	for (int i = 0; i < sc.tf.size(); i++) {
		tmp++;
		a += wSeparateLL(tmp);
		tmp1 = tmp;
		tmp += sc.tf[i].textures.size() * 5 * 2 + 1;
		for (int b = 0; b < sc.tf[i].textures.size(); b++) {
			tmp += sc.tf[i].textures[b].dat.size() + 2;
			//cout << "done";
		}
		a += wSeparateLL(tmp);
	}
	for (int i = 0; i < sc.tsets.size(); i++) {
		tmp++;
		a += wSeparateLL(tmp);
		tmp2 = tmp;
		tmp += sc.tsets[i].tiles.size() * 10 + 1;
		tmp += sc.tsets[i].tiles.size() * 16;
		a += wSeparateLL(tmp);
	}
	for (int i = 0; i < sc.cams.size(); i++) {
		tmp++;
		a += wSeparateLL(tmp);
		tmp += 3;
		a += wSeparateLL(tmp);
	}

	for (int i = 0; i < sc.Layers.size(); i++) {
		a += wchar_t(sc.Layers[i].tmap.gwidth()), a += wchar_t(sc.Layers[i].tmap.gheigh());
		for (int x = 0; x < sc.Layers[i].tmap.gwidth(); x++) {
			for (int y = 0; y < sc.Layers[i].tmap.gheigh(); y++) {
				a += wchar_t(sc.Layers[i].tmap.map[x][y].tile), a += wchar_t(sc.Layers[i].tmap.map[x][y].tset);
			}
		}
	}

	for (int i = 0; i < sc.tf.size(); i++) {
		tmp1 += sc.tf[i].textures.size() * 10;
	}
	for (int i = 0; i < sc.tf.size(); i++) {
		a += wchar_t(sc.tf[i].textures.size());
		for (int b = 0; b < sc.tf[i].textures.size(); b++) {
			tmp1++;
			a += wSeparateLL(tmp1);
			tmp1 += sc.tf[i].textures[b].dat.length() + 2;
			a += wSeparateLL(tmp1);
		}
	}
	for (int i = 0; i < sc.tf.size(); i++) {
		for (int b = 0; b < sc.tf[i].textures.size(); b++) {
			a += wchar_t(sc.tf[i].textures[b].size.x), a += wchar_t(sc.tf[i].textures[b].size.y);
			a += sc.tf[i].textures[b].dat;
		}
	}

	//for (int i = 0; i < sc.tf.size(); i++) {
		//for (int b = 0; b < sc.tf[i].textures.size(); b++) {
			//a += stws(sc.tf[i].textures[b].dat);
		//}
	//}
	for (int i = 0; i < sc.tsets.size(); i++) {
		tmp2 += sc.tsets[i].tiles.size() * 10;//2 + 3 * 4 + 2;
	}
	for (int i = 0; i < sc.tsets.size(); i++) {
		a += wchar_t(sc.tsets[i].tiles.size());
		for (int b = 0; b < sc.tsets[i].tiles.size(); b++) {		
			tmp2++;
			a += wSeparateLL(tmp);
			tmp2 += 16;
			a += wSeparateLL(tmp);
		}
	}
	for (int i = 0; i < sc.tsets.size(); i++) {
		for (int b = 0; b < sc.tsets[i].tiles.size(); b++) {
			a += sc.tsets[i].tiles[b].folder, a += sc.tsets[i].tiles[b].id;
		}
	}

	for (int i = 0; i < sc.cams.size(); i++) {
		a += wSeparate(sc.cams[i].pos.x), a += wSeparate(sc.cams[i].pos.y), a += wSeparate(sc.cams[i].zoom);
	}

	//for (int i = 0; i < sc.tf.size(); i++) {
		//for (int b = 0; b < sc.tf[i].textures.size(); b++) {
			//a += stws(sc.tf[i].textures[b].dat);
		//}
	//}
	/*
	for (int i = 0; i < sc.tsets.size(); i++) {
		for (int b = 0; b < sc.tsets[i].amount_of_tiles; b++) {
			a += wchar_t(sc.tsets[i].tiles[b].folder), a += wchar_t(sc.tsets[i].tiles[b].id), a += wchar_t(sc.tsets[i].tiles[b].center.x), a += wchar_t(sc.tsets[i].tiles[b].center.y), a += wchar_t(sc.tsets[i].tiles[b].rotation), a += wchar_t(sc.tsets[i].tiles[b].scalex), a += wchar_t(sc.tsets[i].tiles[b].scaley);//wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(sc.tf[i].textures[b].dat);
		}
	}*/
	return a;
}

short w_getvalue(wstring str, adress adr) {
	short a;
	a = str[adr.beg];// , a = file.get();
	return a;
}

adress w_getadress(wstring str, long long start, short item) {
	wstring a;
	adress b;
	for (int i = 0; i < 10; i++)
		a += str[start + (item * 10) + i];
	b.beg = wMergeLL(a), b.end = wMergeLL(a, 5);
	return b;
}

tile w_load_tile_from_tmap(wstring str, adress tma, int tmap_id, int x, int y, int heigh) {
	adress adr = tma;
	wstring a;
	tile t;
	adr.beg += (x * 2 * heigh + y * 2) + 2, t.tile = w_getvalue(str, adr), adr.beg += 1, t.tset = w_getvalue(str, adr);
	t.loaded = 1;
	return t;
}

struct ao {
	int Layers, tfolders, tsets, cams, textures, tiles;
};

texture load_scene(wstring data) {
	Scene sc;
	//vecmap tm;
	ao aoo;
	aoo.Layers = data[0], aoo.tfolders = data[1], aoo.tsets = data[2], aoo.cams = data[3];
	adress tmp, tmp1;
	//aoo.Layers * 10 + aoo.tfolders * 10 + aoo.tsets * 10 + aoo.cams * 10 + 4;
	//tmp2.beg = aoo.Layers * 10 + aoo.tfolders * 10 + aoo.tsets * 10 + aoo.cams * 10 + 4 + 1;
	//cout << w_getvalue(data, tmp) << " " << w_getvalue(data, tmp1);
	//tmp2.beg = tmp.beg + 2;
	//cout << "passed";
	for (int i = 0; i < aoo.Layers; i++) {
		sc.add(layer);
		tmp = w_getadress(data, 4, i);
		tmp1.beg = tmp.beg + 1;
		sc.Layers[i].tmap.cnew(w_getvalue(data, tmp), w_getvalue(data, tmp1));
		//tm.cnew(w_getvalue(data, tmp), w_getvalue(data, tmp1));
		cout << sc.Layers[i].tmap.gheigh() << " " << sc.Layers[i].tmap.gwidth() << endl;
		for (int x = 0; x < sc.Layers[i].tmap.gwidth(); x++) {			
			for (int y = 0; y < sc.Layers[i].tmap.gheigh(); y++) {
				sc.Layers[i].tmap.gTile(x, y, w_load_tile_from_tmap(data, tmp, i, x, y, sc.Layers[i].tmap.gheigh()).tile, w_load_tile_from_tmap(data, tmp, i, x, y, sc.Layers[i].tmap.gheigh()).tset).tile;
				//tm.gTile(x, y, w_load_tile_from_tmap(data, tmp, i, x, y, tm.gwidth()).tile, w_load_tile_from_tmap(data, tmp, i, x, y, tm.gwidth()).tset).tile;

			}
		}
	}
	wstring tmpstr;
	tmp = w_getadress(data, 4, aoo.Layers);
	aoo.textures = w_getvalue(data, tmp);
	tmp.beg += 1;
	tmp1 = w_getadress(data, tmp.beg, 0);
	for (int i = 0; i < aoo.tfolders; i++) {
		sc.add(tFolder);
		tmp = tmp1, tmp.beg -= 1;
		
		for (int b = 0; b < aoo.textures; b++) {
			tmp1.beg++;
			tmpstr = cut(data, tmp1.beg - 1, tmp1.end);
			sc.tf[i].add();
			sc.tf[i].textures[b].loadfromstr(tmpstr);
		}
	}
	return sc.tf[0].textures[0];
}