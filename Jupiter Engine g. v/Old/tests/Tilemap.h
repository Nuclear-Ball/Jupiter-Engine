#pragma once
#include <iostream>
#include <string>
//#include <bitset>
#include <fstream>
#include <vector>
//#include <Bits.h>
//#include <camera.h>

using namespace std;

struct tile {
	short tile, tset = 0;
	int x = 0, y = 0;
	bool loaded = 0;
};
/*
class Tilemap {
private:
	int width = 0, heigh = 0; 
	tile *reload_map;

public:
	pos2d offset;
	int tileSize;
	tile* map;
	int gwidth() {
		return width;
	}

	int gheigh() {
		return heigh;
	}

	tile gTile(int x, int y, int tile_value = -2, int tset_value = -1) {
		if (tile_value > -2)
			map[y * width + x].tile = tile_value;
		if (tset_value > -1)
			map[y * width + x].tset = tset_value;
		return map[y * width + x];
	}

	void cnew(int width, int heigh, int offset_x = 0, int offset_y = 0, int tile_size = 16, int stile = -1, int from_tileset = 0) {
		offset.x = offset_x, offset.y = offset_y, tileSize = tile_size;
		this->width = width, this->heigh = heigh;
		map = new tile[width * heigh];
		for (int i = 0; i < heigh * width; i++)
			map[i].tile = stile, map[i].tset = from_tileset;
		reload_map = map;
	}

	wstring save() {
		wstring line;
		line += wchar_t(width + 32), line += wchar_t(heigh + 32), line += wchar_t(offset.x + 32), line += wchar_t(offset.y + 32), line += wchar_t(tileSize + 32);
		for (int i = 0; i < width * heigh; i++)
			line += wchar_t(map[i].tile + 32), line += wchar_t(map[i].tset + 32);
		return line;
	}

	void reload() {
		map = reload_map;
	}

	void clear() {
		for (int line = 0; line < heigh; line++)
			for (int coll = 0; coll < width; coll++)
				gTile(coll, line, 0, 0);
	}

	void fill(int id, tile a) {
		for (int line = 0; line < heigh; line++)
			for (int coll = 0; coll < width; coll++)
				gTile(coll, line, a.tile, a.tset);
	}

	void replace_tileset(int from, int to) {
		for (int i = 0; i < width * heigh; i++) 
			if (map[i].tset == from)
				map[i].tset = to;
	}

	void replace_tile(int from, int to) {
		for (int i = 0; i < width * heigh; i++)
			if (map[i].tile == from)
				map[i].tile = to;
	}

	void replace_tile_and_set(int tfrom, int tto, int sfrom, int sto) {
		for (int i = 0; i < width * heigh; i++)
			if (map[i].tile == tfrom && map[i].tset == sfrom)
				map[i].tile = tto, map[i].tset == sto;
	}

	void resize(int x, int y) {
		tile* resized = new tile[(width + x) * (heigh + y)];
		for (int line = 0; line < (heigh + y); line++)
			for (int coll = 0; coll < (width + x); coll++)
				resized[line * (width + x) + coll].tile = 0, resized[line * (width + x) + coll].tset = 0;
		for (int line = 0; line < heigh; line++)
			for (int coll = 0; coll < width; coll++)
				resized[line * (width + x) + coll] = gTile(coll, line);
		delete[] map;
		map = resized;
		width += x, heigh += y;
	}

};
*/

/*
int isinside(cam2d camera, Tilemap map, int screen_width, int screen_heigh, int tile_size) {
	if ((camera.pos.x + screen_width < map.gwidth() * tile_size && camera.pos.x >= 0) && (camera.pos.y + screen_heigh < map.gheigh() * tile_size && camera.pos.y >= 0))
		return 0;
	else {
		if ((camera.pos.x + screen_width > map.gwidth() * tile_size && camera.pos.x < 0) && (camera.pos.y + screen_heigh > map.gheigh() * tile_size && camera.pos.y < 0))
			return 1;
		if ((camera.pos.x + screen_width < map.gwidth() * tile_size && camera.pos.x < 0) && (camera.pos.y + screen_heigh < map.gheigh() * tile_size && camera.pos.y < 0))
			return 2;
		if ((camera.pos.x + screen_width > map.gwidth() * tile_size && camera.pos.x >= 0) && (camera.pos.y + screen_heigh < map.gheigh() * tile_size && camera.pos.y < 0))
			return 3;
		if ((camera.pos.x + screen_width > map.gwidth() * tile_size && camera.pos.x >= 0) && (camera.pos.y + screen_heigh > map.gheigh() * tile_size && camera.pos.y >= 0))
			return 4;
		if ((camera.pos.x + screen_width < map.gwidth() * tile_size && camera.pos.x < 0) && (camera.pos.y + screen_heigh > map.gheigh() * tile_size && camera.pos.y >= 0))
			return 5;
		if ((camera.pos.x + screen_width < map.gwidth() * tile_size && camera.pos.x >= 0) && (camera.pos.y + screen_heigh < map.gheigh() * tile_size && camera.pos.y < 0))
			return 6;
		if ((camera.pos.x + screen_width > map.gwidth() * tile_size && camera.pos.x >= 0) && (camera.pos.y + screen_heigh < map.gheigh() * tile_size && camera.pos.y >= 0))
			return 7;
		if ((camera.pos.x + screen_width < map.gwidth() * tile_size && camera.pos.x >= 0) && (camera.pos.y + screen_heigh > map.gheigh() * tile_size && camera.pos.y >= 0))
			return 8;
		if ((camera.pos.x + screen_width < map.gwidth() * tile_size && camera.pos.x < 0) && (camera.pos.y + screen_heigh < map.gheigh() * tile_size && camera.pos.y >= 0))
			return 9;
		return 1;
	}
}*/

class vecmap {
private:
	int width = 0, height = 0;
	vector<vector<tile>> reload_map;

public:

	void load_tcl(u16string path) {

	}

	vector<int> vec1d() {
		for (int i = 0; i < width; i++) {

		}
	}

	pos2d offset;
	int tileSize;
	vector<vector<tile>> map;
	int gwidth() {
		return width;
	}

	int gheigh() {
		return height;
	}

	tile gTile(int x, int y, int tile_value = -2, int tset_value = -1) {
		tile tmp;
		tmp.tile = -1;
		if (0 <= x && 0 <= y && x < width && y < height) {
			if (tile_value > -2)
				map[x][y].tile = tile_value;
			if (tset_value > -1)
				map[x][y].tset = tset_value;
			return map[x][y];
		}
		return tmp;
	}

	void cnew(int w, int h, int st = -1, int ts = 0, int offset_x = 0, int offset_y = 0, int tile_size = 16) {
		offset.x = offset_x, offset.y = offset_y, tileSize = tile_size;
		width = w, height = h;
		tile tmpt;
		tmpt.tile = st;
		tmpt.tset = ts;
		vector<vector<tile>> tmp(width, vector<tile>(height, tmpt));
		map = tmp;
	}

	/*wstring save() {
		wstring line;
		line += wchar_t(width + 32), line += wchar_t(heigh + 32), line += wchar_t(offset.x + 32), line += wchar_t(offset.y + 32), line += wchar_t(tileSize + 32);
		for (int i = 0; i < width * heigh; i++)
			line += wchar_t(map[i].tile + 32), line += wchar_t(map[i].tset + 32);
		return line;
	}*/

	void reload() {
		map = reload_map;
	}
	/*
	void clear() {
		for (int line = 0; line < heigh; line++)
			for (int coll = 0; coll < width; coll++)
				gTile(coll, line, 0, 0);
	}

	void fill(int id, tile a) {
		for (int line = 0; line < heigh; line++)
			for (int coll = 0; coll < width; coll++)
				gTile(coll, line, a.tile, a.tset);
	}
	*/
	//void replace_tileset(int from, int to) {
		//for (int i = 0; i < width * heigh; i++)
			//if (map[i].tset == from)
				//map[i].tset = to;
	//}

	//void replace_tile(int from, int to) {
		//for (int i = 0; i < width * heigh; i++)
			//if (map[i].tile == from)
				//map[i].tile = to;
	//}

	//void replace_tile_and_set(int tfrom, int tto, int sfrom, int sto) {
		//for (int i = 0; i < width * heigh; i++)
			//if (map[i].tile == tfrom && map[i].tset == sfrom)
				//map[i].tile = tto, map[i].tset == sto;
	//}

	void resize(int x, int y) {
		map.resize(x, vector<tile>(y));
	}

};