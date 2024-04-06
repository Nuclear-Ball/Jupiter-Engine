#pragma once
#include <pos2d.h>
#include <iostream>
//#include <string>
//#include <fstream>
#include <vector>
//#include <window.h>
#include <texture.h>

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
		pos2dus pos_size, neg_size;

		tile *mxpyp, *mxmyp, *mxpym, *mxmym;

		//vector<vector<tile>> map_xpyp, map_xmyp, map_xpym, map_xmym;

		Tilemap() {
			cout << "constructor called\n";
			mxpyp = new tile[0];
			mxmyp = new tile[0];
			mxpym = new tile[0];
			mxmym = new tile[0];
		}

		void finish() {
			cout << "destructor called\n";
			delete[] mxpyp;
			delete[] mxmyp;
			delete[] mxpym;
			delete[] mxmym;
		}

		void clear() {
			pos_size.x = 0, pos_size.y = 0, neg_size.x = 0, neg_size.y = 0;
			delete[] mxpyp;
			delete[] mxmyp;
			delete[] mxpym;
			delete[] mxmym;
			mxpyp = new tile[0];
			mxmyp = new tile[0];
			mxpym = new tile[0];
			mxmym = new tile[0];
		}

		pos2d offset;
		pos2dus grid_size = { 16, 16 };

		int gwidth() {
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
			tile* n_mxpyp = new tile[pos_size.x * pos_size.y];
			tile* n_mxpym = new tile[pos_size.x * neg_size.y];

			memcpy(n_mxpyp, mxpyp, pos_size.x * pos_size.y);
			memcpy(n_mxpym, mxpym, pos_size.x * neg_size.y);

			delete[] mxpyp;
			delete[] mxpym;
			mxpyp = new tile[x * pos_size.y];
			mxpym = new tile[x * neg_size.y];

			memcpy(mxpyp, n_mxpyp, pos_size.x * pos_size.y);
			memcpy(mxpym, n_mxpym, pos_size.x * neg_size.y);

			for (short i = pos_size.y - 1; i > 0; i--)
				memmove((mxpyp + i * x), (mxpyp + i * pos_size.x), pos_size.x * sizeof(tile)),
				memset((mxpyp + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(tile));
			for (short i = neg_size.y - 1; i > 0; i--)
				memmove((mxpym + i * x), (mxpym + i * pos_size.x), pos_size.x * sizeof(tile)),
				memset((mxpym + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(tile));

			pos_size.x = x;
		}

		void resize_xm(short x) {
			short cx = -(x + 1);
			tile* n_mxmyp = new tile[neg_size.x * pos_size.y];
			tile* n_mxmym = new tile[neg_size.x * neg_size.y];

			memcpy(n_mxmyp, mxpyp, neg_size.x * pos_size.y);
			memcpy(n_mxmym, mxpym, neg_size.x * neg_size.y);

			delete[] mxmyp;
			delete[] mxmym;
			mxmyp = new tile[cx * pos_size.y];
			mxmym = new tile[cx * neg_size.y];

			memcpy(mxmyp, n_mxmyp, neg_size.x * pos_size.y);
			memcpy(mxmym, n_mxmym, neg_size.x * neg_size.y);

			for (short i = pos_size.y - 1; i >= 0; i--)
				memmove((mxmyp + i * cx), (mxmyp + i * neg_size.x), neg_size.x * sizeof(tile)),
				memset((mxmyp + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(tile));
			for (short i = neg_size.y - 1; i >= 0; i--)
				memmove((mxmym + i * cx), (mxmym + i * neg_size.x), neg_size.x * sizeof(tile)),
				memset((mxmym + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(tile));

			neg_size.x = cx;
		}

		void resize_yp(short y) {

			tile* n_mxpyp = new tile[pos_size.x * pos_size.y];
			tile* n_mxmyp = new tile[neg_size.x * pos_size.y];

			memcpy(n_mxpyp, mxpyp, pos_size.x * pos_size.y);
			memcpy(n_mxmyp, mxmyp, neg_size.x * pos_size.y);

			delete[] mxpyp;
			delete[] mxmyp;
			mxpyp = new tile[pos_size.x * y];
			mxmyp = new tile[pos_size.x * y];

			memcpy(mxpyp, n_mxpyp, pos_size.x * pos_size.y);
			memcpy(mxmyp, n_mxmyp, neg_size.x * pos_size.y);

			memset(mxpyp + pos_size.x * pos_size.y, 0, pos_size.x * (y - pos_size.y) * sizeof(tile));
			memset(mxmyp + neg_size.x * pos_size.y, 0, neg_size.x * (y - pos_size.y) * sizeof(tile));
			pos_size.y = y;
		}

		void resize_ym(short y) {
			jus cy = -(y + 1);

			tile* n_mxpym = new tile[pos_size.x * neg_size.y];
			tile* n_mxmym = new tile[neg_size.x * neg_size.y];

			memcpy(n_mxpym, mxpym, pos_size.x * neg_size.y);
			memcpy(n_mxmym, mxmym, neg_size.x * neg_size.y);

			delete[] mxpym;
			delete[] mxmym;
			mxpyp = new tile[pos_size.x * cy];
			mxmyp = new tile[pos_size.x * cy];

			memcpy(mxpyp, n_mxpym, pos_size.x * neg_size.y);
			memcpy(mxmyp, n_mxmym, neg_size.x * neg_size.y);

			memset(mxpym + pos_size.x * neg_size.y, 0, pos_size.x * (cy - neg_size.y) * sizeof(tile));
			memset(mxmym + neg_size.x * neg_size.y, 0, neg_size.x * (cy - neg_size.y) * sizeof(tile));
			neg_size.y = -(y + 1);
		}

		tile lookup(pos2ds pos) {
			tile tmp;
			tmp.tset_value = 0;
			tile* quarters[4] = { mxpyp, mxpym, mxmyp, mxmym };

			short cw = pos_size.x * (pos.x >= 0) + neg_size.x * !(pos.x >= 0),
				ch = pos_size.y * (pos.y >= 0) + neg_size.y * !(pos.y >= 0);
			tile* curr_quarter = quarters[((pos.x < 0) << 1) + (pos.y < 0)];
			pos2ds converted_pos = { pos.x * (pos.x >= 0) + -(pos.x + 1) * !(pos.x >= 0),
								   pos.y * (pos.y >= 0) + -(pos.y + 1) * !(pos.y >= 0) };

			if (converted_pos.x < cw && converted_pos.y < ch) {
				return curr_quarter[converted_pos.y * cw + converted_pos.x];
			}
			return tmp;
		}

		void put_value_and_resize(pos2ds pos, jus tile_value, jus tset_value, bool is_local = 0) {
			tile tmp;
			tmp.tile_value = 0;
			
			bool xq = pos.x >= 0, yq = pos.y >= 0;

			tile* curr_quarter = xq ? (yq ? mxpyp : mxpym) : (yq ? mxmyp : mxmym);

			jus cw = pos_size.x * xq + neg_size.x * !xq, 
				ch = pos_size.y * yq + neg_size.y * !yq;

			pos2ds converted_pos = {pos.x * xq + -(pos.x + 1) * !xq, 
								   pos.y * yq + -(pos.y + 1) * !yq};

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

mxpyp = (tile*)realloc(mxpyp, y * pos_size.x * sizeof(tile));
			mxmyp = (tile*)realloc(mxmyp, y * neg_size.x * sizeof(tile));

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