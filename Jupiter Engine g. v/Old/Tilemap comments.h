#pragma once
#include <pos2d.h>
#include <iostream>
//#include <string>
//#include <fstream>
#include <vector>
//#include <window.h>
#include <texture.h>
#include <jserial.h>

using namespace std;

typedef unsigned short jus;

namespace tiles {

	struct tile {
		jus tile_value; 
		jus tset_value;
		bool is_local;
	};

	struct t_tile {
		texture::texture tile_texture;
	};

	struct tileset {
		string in_editor_name = u8"Без имени";
		vector<t_tile> tiles;
	};



	class Tilemap {
	public:
		pos2ds pos_size, neg_size;

		tile *mxpyp, *mxmyp, *mxpym, *mxmym;

		Tilemap() {
			//cout << "constructor called\n";
			mxpyp = (tile*)malloc(0);
			mxmyp = (tile*)malloc(0);
			mxpym = (tile*)malloc(0);
			mxmym = (tile*)malloc(0);
		}

		void finish() {
			cout << "destructor called\n";
			free(mxpyp);
			free(mxmyp);
			free(mxpym);
			free(mxmym);
		}

		void clear() {
			pos_size.x = 0, pos_size.y = 0, neg_size.x = 0, neg_size.y = 0;
			free(mxpyp);
			free(mxmyp);
			free(mxpym);
			free(mxmym);
			mxpyp = (tile*)malloc(0);
			mxmyp = (tile*)malloc(0);
			mxpym = (tile*)malloc(0);
			mxmym = (tile*)malloc(0);
		}

		pos2ds offset;
		pos2dus grid_size = { 16, 16 };

		short gwidth() {
			return pos_size.x;
		}

		short gheigh() {
			return pos_size.y;
		}

		short gnwidth() {
			return neg_size.x;
		}

		short gnheigh() {
			return neg_size.y;
		}

		void replace(tile from, tile to) {
			for (int i = 0; i < pos_size.x * pos_size.y; i++) {
				//if(!memcmp(&mxpyp[i], &from, sizeof(tile)))
				if((mxpyp[i].tile_value == from.tile_value) && (mxpyp[i].tset_value == from.tset_value) && (mxpyp[i].is_local == from.is_local))
					mxpyp[i] = to;
			}
		}

		void swap_replace(tile a, tile b) {
			for (int i = 0; i < pos_size.x * pos_size.y; i++) {
				tile* quarter = mxpyp;
				if ((quarter[i].tile_value == a.tile_value) && (quarter[i].tset_value == a.tset_value) && (quarter[i].is_local == a.is_local))
					quarter[i] = b;
				else if ((quarter[i].tile_value == b.tile_value) && (quarter[i].tset_value == b.tset_value) && (quarter[i].is_local == b.is_local))
					quarter[i] = a;
			}

			for (int i = 0; i < neg_size.x * pos_size.y; i++) {
				tile* quarter = mxmyp;
				if ((quarter[i].tile_value == a.tile_value) && (quarter[i].tset_value == a.tset_value) && (quarter[i].is_local == a.is_local))
					quarter[i] = b;
				else if ((quarter[i].tile_value == b.tile_value) && (quarter[i].tset_value == b.tset_value) && (quarter[i].is_local == b.is_local))
					quarter[i] = a;
			}

			for (int i = 0; i < pos_size.x * neg_size.y; i++) {
				tile* quarter = mxpym;
				if ((quarter[i].tile_value == a.tile_value) && (quarter[i].tset_value == a.tset_value) && (quarter[i].is_local == a.is_local))
					quarter[i] = b;
				else if ((quarter[i].tile_value == b.tile_value) && (quarter[i].tset_value == b.tset_value) && (quarter[i].is_local == b.is_local))
					quarter[i] = a;
			}
			
			for (int i = 0; i < neg_size.x * neg_size.y; i++) {
				tile* quarter = mxmym;
				if ((quarter[i].tile_value == a.tile_value) && (quarter[i].tset_value == a.tset_value) && (quarter[i].is_local == a.is_local))
					quarter[i] = b;
				else if ((quarter[i].tile_value == b.tile_value) && (quarter[i].tset_value == b.tset_value) && (quarter[i].is_local == b.is_local))
					quarter[i] = a;
			}
		}

		void shift_x(int a) {

		}

		void shift_y(int a) {

		}

		void resize_xp(short x) {
			mxpyp = (tile*)realloc(mxpyp, pos_size.y * x * sizeof(tile));
			mxpym = (tile*)realloc(mxpym, neg_size.y * x * sizeof(tile));

			for (short i = pos_size.y - 1; i >= 0; i--)
				memmove((mxpyp + i * x), (mxpyp + i * pos_size.x), pos_size.x * sizeof(tile)),
				memset((mxpyp + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(tile));
			for (short i = neg_size.y - 1; i >= 0; i--)
				memmove((mxpym + i * x), (mxpym + i * pos_size.x), pos_size.x * sizeof(tile)),
				memset((mxpym + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(tile));

			pos_size.x = x;
		}

		void resize_xm(short x) {
			short cx = -(x + 1);
			mxmyp = (tile*)realloc(mxmyp, pos_size.y * cx * sizeof(tile));
			mxmym = (tile*)realloc(mxmym, neg_size.y * cx * sizeof(tile));

			for (short i = pos_size.y - 1; i >= 0; i--)
				memmove((mxmyp + i * cx), (mxmyp + i * neg_size.x), neg_size.x * sizeof(tile)),
				memset((mxmyp + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(tile));
			for (short i = neg_size.y - 1; i >= 0; i--)
				memmove((mxmym + i * cx), (mxmym + i * neg_size.x), neg_size.x * sizeof(tile)),
				memset((mxmym + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(tile));

			neg_size.x = cx;
		}

		void resize_yp(short y) {
			mxpyp = (tile*)realloc(mxpyp, y * pos_size.x * sizeof(tile));
			mxmyp = (tile*)realloc(mxmyp, y * neg_size.x * sizeof(tile));
			memset(mxpyp + pos_size.x * pos_size.y, 0, pos_size.x * (y - pos_size.y) * sizeof(tile));
			memset(mxmyp + neg_size.x * pos_size.y, 0, neg_size.x * (y - pos_size.y) * sizeof(tile));
			pos_size.y = y;
		}

		void resize_ym(short y) {
			jus cy = -(y + 1);
			mxpym = (tile*)realloc(mxpym, cy * pos_size.x * sizeof(tile));
			mxmym = (tile*)realloc(mxmym, cy * neg_size.x * sizeof(tile));
			memset(mxpym + pos_size.x * neg_size.y, 0, pos_size.x * (cy - neg_size.y) * sizeof(tile));
			memset(mxmym + neg_size.x * neg_size.y, 0, neg_size.x * (cy - neg_size.y) * sizeof(tile));
			neg_size.y = -(y + 1);
		}

		tile lookup(pos2ds pos) {
			short converted_width = pos_size.x * (pos.x >= 0) + neg_size.x * !(pos.x >= 0),
				converted_height = pos_size.y * (pos.y >= 0) + neg_size.y * !(pos.y >= 0);

			tile* quarters[4] = { mxpyp, mxmyp, mxpym, mxmym };
			tile* curr_quarter = quarters[((pos.y < 0) << 1) + (pos.x < 0)];

			pos2ds converted_pos = { pos.x * (pos.x >= 0) + -(pos.x + 1) * !(pos.x >= 0),
								   pos.y * (pos.y >= 0) + -(pos.y + 1) * !(pos.y >= 0) };

			return (converted_pos.x < converted_width&& converted_pos.y < converted_height) ? curr_quarter[converted_pos.y * converted_width + converted_pos.x] : tile({ 0, 0, 0 });
		}

		void put_value_and_resize(pos2ds pos, tile tile_value) {
			tile tmp;
			tmp.tile_value = 0;
			
			bool xq = pos.x >= 0, yq = pos.y >= 0;

			tile* curr_quarter = xq ? (yq ? mxpyp : mxpym) : (yq ? mxmyp : mxmym);

			jus cw = pos_size.x * xq + neg_size.x * !xq, 
				ch = pos_size.y * yq + neg_size.y * !yq;

			pos2ds converted_pos = {pos.x * xq + -(pos.x + 1) * !xq, 
								   pos.y * yq + -(pos.y + 1) * !yq};
			if (tile_value.tset_value || (!tile_value.tset_value && tile_value.tile_value)) {
				if (converted_pos.x >= cw && tile_value.tile_value > -1)
					pos.x >= 0 ? resize_xp(pos.x + 1) : resize_xm(pos.x - 1);
				if (converted_pos.y >= ch && tile_value.tile_value > -1)
					pos.y >= 0 ? resize_yp(pos.y + 1) : resize_ym(pos.y - 1);
			}
			if (converted_pos.x < cw && converted_pos.y < ch) {
				curr_quarter[converted_pos.y * cw + converted_pos.x] = tile_value;
			}
		}

		string serialize() {
			string dat;
			dat += bits::Separate<pos2ds>(pos_size);
			dat += bits::Separate<pos2ds>(neg_size);
			dat += bits::Separate<pos2dus>(grid_size);
			dat += bits::Separate<pos2ds>(offset);
			string s_xpyp, s_xmyp, s_xpym, s_xmym;
			s_xpyp.resize(pos_size.x * pos_size.y * sizeof(tile));
			memcpy((void*)s_xpyp.data(), mxpyp, pos_size.x * pos_size.y * sizeof(tile));
			s_xmyp.resize(neg_size.x * pos_size.y * sizeof(tile));
			memcpy((void*)s_xmyp.data(), mxmyp, neg_size.x * pos_size.y * sizeof(tile));
			s_xpym.resize(pos_size.x * neg_size.y * sizeof(tile));
			memcpy((void*)s_xpym.data(), mxpym, pos_size.x * neg_size.y * sizeof(tile));
			s_xmym.resize(neg_size.x * neg_size.y * sizeof(tile));
			memcpy((void*)s_xmym.data(), mxmym, neg_size.x * neg_size.y * sizeof(tile));

			dat += s_xpyp;
			dat += s_xmyp;
			dat += s_xpym;
			dat += s_xmym;
			return dat;
		}

		void deserialize(string dat) {
			int cursor_pos = 0;
			pos_size = bits::Merge<pos2ds>(dat.substr(0, sizeof(pos2ds)));
			neg_size = bits::Merge<pos2ds>(dat.substr(sizeof(pos2ds), sizeof(pos2ds)));
			grid_size = bits::Merge<pos2dus>(dat.substr(sizeof(pos2ds) * 2, sizeof(pos2dus)));
			offset = bits::Merge<pos2ds>(dat.substr(sizeof(pos2ds) * 3, sizeof(pos2ds)));

			mxpyp = (tile*)realloc(mxpyp, pos_size.x * pos_size.y * sizeof(tile)); //выделение памяти под данные карты
			mxmyp = (tile*)realloc(mxmyp, neg_size.x * pos_size.y * sizeof(tile));
			mxpym = (tile*)realloc(mxpym, pos_size.x * neg_size.y * sizeof(tile));
			mxmym = (tile*)realloc(mxmym, neg_size.x * neg_size.y * sizeof(tile));

			memcpy(mxpyp, dat.data() + 4 * sizeof(pos2ds), pos_size.x * pos_size.y * sizeof(tile));
			cursor_pos += pos_size.x * pos_size.y * sizeof(tile);
			memcpy(mxmyp, dat.data() + 4 * sizeof(pos2ds) + cursor_pos, neg_size.x * pos_size.y * sizeof(tile));
			cursor_pos += neg_size.x * pos_size.y * sizeof(tile);
			memcpy(mxpym, dat.data() + 4 * sizeof(pos2ds) + cursor_pos, pos_size.x * neg_size.y * sizeof(tile));
			cursor_pos += pos_size.x * neg_size.y * sizeof(tile);
			memcpy(mxmym, dat.data() + 4 * sizeof(pos2ds) + cursor_pos, neg_size.x * neg_size.y * sizeof(tile));
			cursor_pos += neg_size.x * neg_size.y * sizeof(tile);
		}
	};

	class TestTilemap {
	public:
		pos2ds pos_size, neg_size;

		//tile* mxpyp, * mxmyp, * mxpym, * mxmym;

		vector<tile> map_xpyp, map_xmyp, map_xpym, map_xmym;

		TestTilemap() {
			cout << "constructor called\n";
			//mxpyp = (tile*)malloc(0);
			//mxmyp = (tile*)malloc(0);
			//mxpym = (tile*)malloc(0);
			//mxmym = (tile*)malloc(0);
		}

		void finish() {
			cout << "destructor called\n";
			//free(mxpyp);
			//free(mxmyp);
			//free(mxpym);
			//free(mxmym);
		}

		void clear() {
			pos_size.x = 0, pos_size.y = 0, neg_size.x = 0, neg_size.y = 0;
			map_xpyp.clear();
			map_xmyp.clear();
			map_xpym.clear();
			map_xmym.clear();
		}

		pos2d offset;
		pos2dus grid_size = { 16, 16 };

		short gwidth() {
			return pos_size.x;
		}

		int gheigh() {
			return pos_size.y;
		}

		int gnwidth() {
			return neg_size.x;
		}

		int gnheigh() {
			return neg_size.y;
		}

		void resize_xp(short x) {
			map_xpyp.resize(pos_size.y * x, {0, 1});
			map_xpym.resize(neg_size.y * x, {0, 1});
			//mxpyp = (tile*)realloc(mxpyp, pos_size.y * x * sizeof(tile));
			//mxpym = (tile*)realloc(mxpym, neg_size.y * x * sizeof(tile));

			for (short i = pos_size.y - 1; i >= 0; i--)
				memmove((map_xpyp.data() + i * x), (&map_xpyp[0] + i * pos_size.x), pos_size.x * sizeof(tile)),
				memset((map_xpyp.data() + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(tile));
			for (short i = neg_size.y - 1; i >= 0; i--)
				memmove((map_xpym.data() + i * x), (&map_xpym[0] + i * pos_size.x), pos_size.x * sizeof(tile)),
				memset((map_xpym.data() + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(tile));

			pos_size.x = x;
		}

		void resize_xm(short x) {
			short cx = -(x + 1);
			map_xmyp.resize(pos_size.y * cx, { 0, 1 });
			map_xmym.resize(neg_size.y * cx, { 0, 1 });
			//mxmyp = (tile*)realloc(mxmyp, pos_size.y * cx * sizeof(tile));
			//mxmym = (tile*)realloc(mxmym, neg_size.y * cx * sizeof(tile));

			for (short i = pos_size.y - 1; i >= 0; i--)
				memmove((map_xmyp.data() + i * cx), (map_xmyp.data() + i * neg_size.x), neg_size.x * sizeof(tile)),
				memset((map_xmyp.data() + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(tile));
			for (short i = neg_size.y - 1; i >= 0; i--)
				memmove((map_xmym.data() + i * cx), (map_xmym.data() + i * neg_size.x), neg_size.x * sizeof(tile)),
				memset((map_xmym.data() + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(tile));

			neg_size.x = cx;
		}

		void resize_yp(short y) {
			map_xpyp.resize(y * pos_size.x, { 0, 1 });
			map_xmyp.resize(y * neg_size.x, { 0, 1 });

			//mxpyp = (tile*)realloc(mxpyp, y * pos_size.x * sizeof(tile));
			//mxmyp = (tile*)realloc(mxmyp, y * neg_size.x * sizeof(tile));
			memset(map_xpyp.data() + pos_size.x * pos_size.y, 0, pos_size.x * (y - pos_size.y) * sizeof(tile));
			memset(map_xmyp.data() + neg_size.x * pos_size.y, 0, neg_size.x * (y - pos_size.y) * sizeof(tile));
			pos_size.y = y;
		}

		void resize_ym(short y) {
			jus cy = -(y + 1);
			map_xpym.resize(cy * pos_size.x, { 0, 1 });
			map_xmym.resize(cy * neg_size.x, { 0, 1 });

			//mxpym = (tile*)realloc(mxpym, cy * pos_size.x * sizeof(tile));
			//mxmym = (tile*)realloc(mxmym, cy * neg_size.x * sizeof(tile));
			memset(map_xpym.data() + pos_size.x * neg_size.y, 0, pos_size.x * (cy - neg_size.y) * sizeof(tile));
			memset(map_xmym.data() + neg_size.x * neg_size.y, 0, neg_size.x * (cy - neg_size.y) * sizeof(tile));
			neg_size.y = cy;
		}

		tile lookup(pos2ds pos) {
			tile tmp;
			tmp.tset_value = 0;
			vector<tile>* quarters[4] = { &map_xpyp, &map_xpym, &map_xmyp, &map_xmym };

			short cw = pos_size.x * (pos.x >= 0) + neg_size.x * !(pos.x >= 0),
				ch = pos_size.y * (pos.y >= 0) + neg_size.y * !(pos.y >= 0);
			vector<tile>* curr_quarter = quarters[((pos.x < 0) << 1) + (pos.y < 0)];
			pos2ds converted_pos = { pos.x * (pos.x >= 0) + -(pos.x + 1) * !(pos.x >= 0),
								   pos.y * (pos.y >= 0) + -(pos.y + 1) * !(pos.y >= 0) };

			if (converted_pos.x < cw && converted_pos.y < ch) {
				return curr_quarter->operator[](converted_pos.y * cw + converted_pos.x);
			}
			return tmp;
		}

		void put_value_and_resize(pos2ds pos, jus tile_value, jus tset_value, bool is_local = 0) {
			tile tmp;
			tmp.tile_value = 0;

			bool xq = pos.x >= 0, yq = pos.y >= 0;

			vector<tile>& curr_quarter = xq ? (yq ? map_xpyp : map_xpym) : (yq ? map_xmyp : map_xmym);

			jus cw = pos_size.x * xq + neg_size.x * !xq,
				ch = pos_size.y * yq + neg_size.y * !yq;

			pos2ds converted_pos = { pos.x * xq + -(pos.x + 1) * !xq,
								   pos.y * yq + -(pos.y + 1) * !yq };

			if (converted_pos.x >= cw && tile_value > -1)
				pos.x >= 0 ? resize_xp(pos.x + 1) : resize_xm(pos.x - 1);
			if (converted_pos.y >= ch && tile_value > -1)
				pos.y >= 0 ? resize_yp(pos.y + 1) : resize_ym(pos.y - 1);

			if (converted_pos.x < cw && converted_pos.y < ch) {
				curr_quarter[converted_pos.y * cw + converted_pos.x].tile_value = tile_value,
					curr_quarter[converted_pos.y * cw + converted_pos.x].tset_value = tset_value;
			}
		}
	};

	class ppTilemap {
	public:
		pos2dus pos_size, neg_size;

		vector<tile> map_xpyp, map_xmyp, map_xpym, map_xmym;

		void clear() {
			pos_size.x = 0, pos_size.y = 0, neg_size.x = 0, neg_size.y = 0;
			map_xpyp.clear();
			map_xmyp.clear();
			map_xpym.clear();
			map_xmym.clear();
		}

		pos2d offset;
		pos2dus grid_size = { 16, 16 };

		short gwidth() {
			return pos_size.x;
		}

		int gheigh() {
			return pos_size.y;
		}

		int gnwidth() {
			return neg_size.x;
		}

		int gnheigh() {
			return neg_size.y;
		}

		void resize_xp(short x) {
			map_xpyp.resize(pos_size.y * x, { 0, 1 });
			map_xpym.resize(neg_size.y * x, { 0, 1 });

			for (short i = pos_size.y - 1; i >= 0; i--)
				memmove((map_xpyp.data() + i * x), (map_xpyp.data() + i * pos_size.x), pos_size.x * sizeof(tile)),
				memset((map_xpyp.data() + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(tile));
			for (short i = neg_size.y - 1; i >= 0; i--)
				memmove((map_xpym.data() + i * x), (map_xpym.data() + i * pos_size.x), pos_size.x * sizeof(tile)),
				memset((map_xpym.data() + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(tile));

			pos_size.x = x;
		}

		void resize_xm(short x) {
			short cx = -(x + 1);
			map_xmyp.resize(pos_size.y * cx, { 0, 1 });
			map_xmym.resize(neg_size.y * cx, { 0, 1 });

			for (short i = pos_size.y - 1; i >= 0; i--)
				memmove((map_xmyp.data() + i * cx), (map_xmyp.data() + i * neg_size.x), neg_size.x * sizeof(tile)),
				memset((map_xmyp.data() + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(tile));
			for (short i = neg_size.y - 1; i >= 0; i--)
				memmove((map_xmym.data() + i * cx), (map_xmym.data() + i * neg_size.x), neg_size.x * sizeof(tile)),
				memset((map_xmym.data() + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(tile));

			neg_size.x = cx;
		}

		void resize_yp(short y) {
			map_xpyp.resize(y * pos_size.x, { 0, 1 });
			map_xmyp.resize(y * neg_size.x, { 0, 1 });

			memset(map_xpyp.data() + pos_size.x * pos_size.y, 0, pos_size.x * (y - pos_size.y) * sizeof(tile));
			memset(map_xmyp.data() + neg_size.x * pos_size.y, 0, neg_size.x * (y - pos_size.y) * sizeof(tile));
			pos_size.y = y;
		}

		void resize_ym(short y) {
			jus cy = -(y + 1);
			map_xpym.resize(cy * pos_size.x, { 0, 1 });
			map_xmym.resize(cy * neg_size.x, { 0, 1 });

			memset(map_xpym.data() + pos_size.x * neg_size.y, 0, pos_size.x * (cy - neg_size.y) * sizeof(tile));
			memset(map_xmym.data() + neg_size.x * neg_size.y, 0, neg_size.x * (cy - neg_size.y) * sizeof(tile));
			neg_size.y = cy;
		}

		tile lookup(pos2ds pos) {
			tile tmp;
			tmp.tset_value = 0;
			vector<tile>* quarters[4] = { &map_xpyp, &map_xpym, &map_xmyp, &map_xmym };

			short cw = pos_size.x * (pos.x >= 0) + neg_size.x * !(pos.x >= 0),
				ch = pos_size.y * (pos.y >= 0) + neg_size.y * !(pos.y >= 0);
			vector<tile>* curr_quarter = quarters[((pos.x < 0) << 1) + (pos.y < 0)];
			pos2ds converted_pos = { pos.x * (pos.x >= 0) + -(pos.x + 1) * !(pos.x >= 0),
								   pos.y * (pos.y >= 0) + -(pos.y + 1) * !(pos.y >= 0) };

			if (converted_pos.x < cw && converted_pos.y < ch) {
				return curr_quarter->operator[](converted_pos.y* cw + converted_pos.x);
			}
			return tmp;
		}

		void put_value_and_resize(pos2ds pos, jus tile_value, jus tset_value, bool is_local = 0) {
			tile tmp;
			tmp.tile_value = 0;

			bool xq = pos.x >= 0, yq = pos.y >= 0;

			vector<tile>& curr_quarter = xq ? (yq ? map_xpyp : map_xpym) : (yq ? map_xmyp : map_xmym);

			jus cw = pos_size.x * xq + neg_size.x * !xq,
				ch = pos_size.y * yq + neg_size.y * !yq;

			pos2ds converted_pos = { pos.x * xq + -(pos.x + 1) * !xq,
								   pos.y * yq + -(pos.y + 1) * !yq };

			if (converted_pos.x >= cw && tile_value > -1)
				pos.x >= 0 ? resize_xp(pos.x + 1) : resize_xm(pos.x - 1);
			if (converted_pos.y >= ch && tile_value > -1)
				pos.y >= 0 ? resize_yp(pos.y + 1) : resize_ym(pos.y - 1);

			if (converted_pos.x < cw && converted_pos.y < ch) {
				curr_quarter[converted_pos.y * cw + converted_pos.x].tile_value = tile_value,
					curr_quarter[converted_pos.y * cw + converted_pos.x].tset_value = tset_value;
			}
		}
	};

}



/*
 tile gTile(pos2d pos, int tile_value = -2, int tset_value = -1, bool is_local = 0) {
			tile tmp;
			tmp.tile_value = -1;
			
			bool xq = pos.x >= 0, yq = pos.y >= 0;
			vector<vector<tile>>* curr_quarter = xq ? (yq ? &map_xpyp : &map_xpym) : (yq ? &map_xmyp : &map_xmym);
			int cw = pos_size.x * xq + neg_size.x * !xq, ch = pos_size.y * yq + neg_size.y * !yq;
			pos2d converted_pos = {pos.x * xq + -(pos.x + 1) * !xq, pos.y * yq + -(pos.y + 1) * !yq};
			

			if (converted_pos.x >= cw && tile_value > -1)
				pos.x >= 0 ? resize_xp(pos.x + 1) : resize_xm(pos.x - 1);
			if (converted_pos.y >= ch && tile_value > -1)
				pos.y >= 0 ? resize_yp(pos.y + 1) : resize_ym(pos.y - 1);
			

			if (converted_pos.x < cw && converted_pos.y < ch) {
				if (tile_value > -2)
					(*curr_quarter)[converted_pos.x][converted_pos.y].tile_value = tile_value, (*curr_quarter)[converted_pos.x][converted_pos.y].tset_value = tset_value;
				return (*curr_quarter)[converted_pos.x][converted_pos.y];
			}
			return tmp;
		}
 

void resize_xp(int x) {
			map_xpyp.resize(x);
			for (int i = pos_size.x; i < x; ++i)
				map_xpyp[i].resize(pos_size.y, { -1, 0 });
			map_xpym.resize(x);
			for (int i = pos_size.x; i < x; ++i)
				map_xpym[i].resize(neg_size.y, { -1, 0 });
			pos_size.x = x;
		}

		void resize_xm(int x) {
			map_xmyp.resize(-(x + 1));
			for (int i = neg_size.x; i < -(x + 1); i++)
				map_xmyp[i].resize(pos_size.y, { -1, 0 });
			map_xmym.resize(-(x + 1));
			for (int i = neg_size.x; i < -(x + 1); i++)
				map_xmym[i].resize(neg_size.y, { -1, 0 });
			neg_size.x = -(x + 1);
		}

		void resize_yp(int y) {
			for (int i = 0; i < pos_size.x; ++i)
				map_xpyp[i].resize(y, { -1, 0 });
			for (int i = 0; i < neg_size.x; ++i)
				map_xmyp[i].resize(y, { -1, 0 });
			pos_size.y = y;
		}

		void resize_ym(int y) {
			for (int i = 0; i < pos_size.x; ++i)
				map_xpym[i].resize(-(y + 1), { -1, 0 });
			for (int i = 0; i < neg_size.x; ++i)
				map_xmym[i].resize(-(y + 1), { -1, 0 });
			neg_size.y = -(y + 1);
		}
*/