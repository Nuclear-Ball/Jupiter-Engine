#pragma once
//#include <iostream>
//#include <vector>
#include "pos2d.h"
#include "jserial_modular.hpp"

namespace JE {
	namespace Tiles {
		struct Tile {
			unsigned short tile_value;
			unsigned short tset_value;
			bool is_local;

			bool operator==(const Tile& other) const {
				return (tile_value == other.tile_value) && (tset_value == other.tset_value) && (is_local == other.is_local);
			}
		};


		class Tilemap {
		private:
			Tile* quarters[4] = { reinterpret_cast<Tile*>(malloc(0)), reinterpret_cast<Tile*>(malloc(0)), reinterpret_cast<Tile*>(malloc(0)), reinterpret_cast<Tile*>(malloc(0)) };
			pos2ds pos_size = { 0, 0 }, neg_size = { 0, 0 };

			inline void ResizeYP(short y) {
				quarters[0] = reinterpret_cast<Tile*>(realloc(quarters[0], y * pos_size.x * sizeof(Tile)));
				quarters[1] = reinterpret_cast<Tile*>(realloc(quarters[1], y * neg_size.x * sizeof(Tile)));

				memset(&quarters[0][pos_size.x * pos_size.y], 0, pos_size.x * (y - pos_size.y) * sizeof(Tile));
				memset(&quarters[1][neg_size.x * pos_size.y], 0, neg_size.x * (y - pos_size.y) * sizeof(Tile));
				pos_size.y = y;
			}

			inline void ResizeYN(short y) {
				const unsigned short cy = -(y + 1);

				quarters[2] = reinterpret_cast<Tile*>(realloc(quarters[2], cy * pos_size.x * sizeof(Tile))); //Увеличить размер
				quarters[3] = reinterpret_cast<Tile*>(realloc(quarters[3], cy * neg_size.x * sizeof(Tile)));

				memset(quarters[2] + pos_size.x * neg_size.y, 0, pos_size.x * (cy - neg_size.y) * sizeof(Tile)); //Заполнить нулями
				memset(quarters[3] + neg_size.x * neg_size.y, 0, neg_size.x * (cy - neg_size.y) * sizeof(Tile));

				neg_size.y = cy;
			}

			void ResizeXP(short x) {
				quarters[0] = reinterpret_cast<Tile*>(realloc(quarters[0], pos_size.y * x * sizeof(Tile)));
				quarters[2] = reinterpret_cast<Tile*>(realloc(quarters[2], neg_size.y * x * sizeof(Tile)));

				for (short i = pos_size.y - 1; i >= 0; i--)
					memmove((quarters[0] + i * x), (quarters[0] + i * pos_size.x), pos_size.x * sizeof(Tile)),
					memset((quarters[0] + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(Tile));
				for (short i = neg_size.y - 1; i >= 0; i--)
					memmove((quarters[2] + i * x), (quarters[2] + i * pos_size.x), pos_size.x * sizeof(Tile)),
					memset((quarters[2] + i * x + pos_size.x), 0, (x - pos_size.x) * sizeof(Tile));

				pos_size.x = x;
			}

			void ResizeXN(short x) {
				short cx = -(x + 1);

				quarters[1] = reinterpret_cast<Tile*>(realloc(quarters[1], pos_size.y * cx * sizeof(Tile)));
				quarters[3] = reinterpret_cast<Tile*>(realloc(quarters[3], neg_size.y * cx * sizeof(Tile)));

				for (short i = pos_size.y - 1; i >= 0; i--) {
					memmove((quarters[1] + i * cx), (quarters[1] + i * neg_size.x), neg_size.x * sizeof(Tile));
					memset((quarters[1] + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(Tile));
				}

				for (short i = neg_size.y - 1; i >= 0; i--) {
					memmove((quarters[3] + i * cx), (quarters[3] + i * neg_size.x), neg_size.x * sizeof(Tile));
					memset((quarters[3] + i * cx + neg_size.x), 0, (cx - neg_size.x) * sizeof(Tile));
				}

				neg_size.x = cx;
			}

		public:
			static constexpr Tile empty = { 0, 0, 0 };
			pos2ds offset = { 0, 0 };
			pos2dus grid_size = { 16, 16 };

			Tilemap() {}

			Tilemap(const Tilemap& cpy) {
				const pos2ds quarter_sizes[4] = { cpy.pos_size, { cpy.neg_size.x, cpy.pos_size.y }, { cpy.pos_size.x, cpy.neg_size.y }, cpy.neg_size };

				for (unsigned char i = 0; i < 4; i++) {
					quarters[i] = reinterpret_cast<Tile*>(realloc(quarters[i], quarter_sizes[i].x * quarter_sizes[i].y * sizeof(Tile)));
					memcpy(quarters[i], cpy.quarters[i], quarter_sizes[i].x * quarter_sizes[i].y * sizeof(Tile));
				}

				pos_size = cpy.pos_size;
				neg_size = cpy.neg_size;
				grid_size = cpy.grid_size;
				offset = cpy.offset;
			}
			Tilemap& operator=(const Tilemap& cpy) {
				const pos2ds quarter_sizes[4] = { cpy.pos_size, { cpy.neg_size.x, cpy.pos_size.y }, { cpy.pos_size.x, cpy.neg_size.y }, cpy.neg_size };

				for (unsigned char i = 0; i < 4; i++) {
					quarters[i] = reinterpret_cast<Tile*>(realloc(quarters[i], quarter_sizes[i].x * quarter_sizes[i].y * sizeof(Tile)));
					memcpy(quarters[i], cpy.quarters[i], quarter_sizes[i].x * quarter_sizes[i].y * sizeof(Tile));
				}

				pos_size = cpy.pos_size;
				neg_size = cpy.neg_size;
				grid_size = cpy.grid_size;
				offset = cpy.offset;

				return *this;
			}

			Tilemap(Tilemap&& cpy) noexcept {
				for (unsigned char i = 0; i < 4; i++)
					std::swap(quarters[i], cpy.quarters[i]);

				std::swap(pos_size, cpy.pos_size);
				std::swap(neg_size, cpy.neg_size);
				grid_size = cpy.grid_size;
				offset = cpy.offset;
			}
			Tilemap& operator=(Tilemap&& cpy) noexcept {
				for (unsigned char i = 0; i < 4; i++)
					std::swap(quarters[i], cpy.quarters[i]);

				pos_size = cpy.pos_size;
				neg_size = cpy.neg_size;
				cpy.pos_size = { 0, 0 };
				cpy.neg_size = { 0, 0 };

				//std::cout << "moved!\n";

				grid_size = cpy.grid_size;
				offset = cpy.offset;

				return *this;
			}

			inline ~Tilemap() {
				for (unsigned char i = 0; i < 4; i++)
					free(quarters[i]);
			}


			void Clear() {
				pos_size = { 0, 0 };
				neg_size = { 0, 0 };

				for (unsigned char i = 0; i < 4; i++)
					quarters[i] = reinterpret_cast<Tile*>(realloc(quarters[i], 0));
			}

			void Replace(const Tile& from, const Tile& to) {
				const pos2ds quarter_sizes[4] = { pos_size, { neg_size.x, pos_size.y }, { pos_size.x, neg_size.y }, neg_size };

				for (unsigned char i = 0; i < 4; i++) {
					Tile* const quarter = quarters[i];

					for (int j = 0; j < (quarter_sizes[i].x * quarter_sizes[i].y); j++) {
						if (quarter[j] == from)
							quarter[j] = to;
					}
				}
			}

			void SwapReplace(const Tile& a, const Tile& b) {
				const pos2ds quarter_sizes[4] = { pos_size, { neg_size.x, pos_size.y }, { pos_size.x, neg_size.y }, neg_size };

				for (unsigned char i = 0; i < 4; i++) {
					Tile* const quarter = quarters[i];

					for (int j = 0; j < (quarter_sizes[i].x * quarter_sizes[i].y); j++) {
						if (quarter[j] == a)
							quarter[j] = b;
						else if (quarter[j] == b)
							quarter[j] = a;
					}
				}
			}

			void ShiftX(short a) {
				Tile** const pos_shift_maps[2] = { &quarters[(a < 0)], &quarters[2 + (a < 0)] };
				Tile** const neg_shift_maps[2] = { &quarters[(a >= 0)], &quarters[2 + (a >= 0)] };

				const short c_a = std::abs(a);//a * (-1 * (a < 0)) + (a * (a >= 0));

				pos2ds* const c_pos_size = a >= 0 ? &pos_size : &neg_size;
				pos2ds* const c_neg_size = a >= 0 ? &neg_size : &pos_size;

				*pos_shift_maps[0] = reinterpret_cast<Tile*>(realloc(*pos_shift_maps[0], (c_pos_size->x + c_a) * pos_size.y * sizeof(Tile)));
				*pos_shift_maps[1] = reinterpret_cast<Tile*>(realloc(*pos_shift_maps[1], (c_pos_size->x + c_a) * neg_size.y * sizeof(Tile)));

				for (short i = pos_size.y - 1; i >= 0; i--)
					memmove((*pos_shift_maps[0] + i * (c_pos_size->x + c_a)) + c_a, (*pos_shift_maps[0] + i * c_pos_size->x), c_pos_size->x * sizeof(Tile));

				for (short i = neg_size.y - 1; i >= 0; i--)
					memmove((*pos_shift_maps[1] + i * (c_pos_size->x + c_a)) + c_a, (*pos_shift_maps[1] + i * c_pos_size->x), c_pos_size->x * sizeof(Tile));

				if (c_neg_size->x >= c_a) {
					for (short i = 0; i < pos_size.y; i++) {
						memcpy(*pos_shift_maps[0] + i * (c_pos_size->x + c_a), (*neg_shift_maps[0] + i * c_neg_size->x), c_a * sizeof(Tile));
						memmove(*neg_shift_maps[0] + i * (c_neg_size->x - c_a), *neg_shift_maps[0] + i * (c_neg_size->x) + c_a, (c_neg_size->x - c_a) * sizeof(Tile));
					}

					for (short i = 0; i < neg_size.y; i++) {
						memcpy((*pos_shift_maps[1] + i * (c_pos_size->x + c_a)), (*neg_shift_maps[1] + i * c_neg_size->x), c_a * sizeof(Tile));
						memmove(*neg_shift_maps[1] + i * (c_neg_size->x - c_a), *neg_shift_maps[1] + i * (c_neg_size->x) + c_a, (c_neg_size->x - c_a) * sizeof(Tile));
					}

					*neg_shift_maps[0] = reinterpret_cast<Tile*>(realloc(*neg_shift_maps[0], pos_size.y * (c_neg_size->x - c_a) * sizeof(Tile)));
					*neg_shift_maps[1] = reinterpret_cast<Tile*>(realloc(*neg_shift_maps[1], neg_size.y * (c_neg_size->x - c_a) * sizeof(Tile)));
					c_neg_size->x -= c_a;
				}
				else {
					for (short i = pos_size.y - 1; i >= 0; i--)
						memset(*pos_shift_maps[0] + i * (c_pos_size->x + c_a), 0, c_a * sizeof(Tile));
					for (short i = neg_size.y - 1; i >= 0; i--)
						memset(*pos_shift_maps[1] + i * (c_pos_size->x + c_a), 0, c_a * sizeof(Tile));
				}
				c_pos_size->x += c_a;
			}

			void ShiftY(short a) {
				Tile** const pos_shift_maps[] = { &quarters[(a < 0) << 1], &quarters[1 + ((a < 0) << 1)] };
				Tile** const neg_shift_maps[] = { &quarters[(a >= 0) << 1], &quarters[1 + ((a >= 0) << 1)] };

				const short c_a = std::abs(a);//a * (-1 * (a < 0)) + (a * (a >= 0));

				pos2ds* const converted_pos_size = a >= 0 ? &pos_size : &neg_size;
				pos2ds* const converted_neg_size = a >= 0 ? &neg_size : &pos_size;

				*pos_shift_maps[0] = reinterpret_cast<Tile*>(realloc(*pos_shift_maps[0], pos_size.x * (converted_pos_size->y + c_a) * sizeof(Tile)));
				*pos_shift_maps[1] = reinterpret_cast<Tile*>(realloc(*pos_shift_maps[1], neg_size.x * (converted_pos_size->y + c_a) * sizeof(Tile)));
				memmove(*pos_shift_maps[0] + pos_size.x * c_a, *pos_shift_maps[0], pos_size.x * converted_pos_size->y * sizeof(Tile));
				memmove(*pos_shift_maps[1] + neg_size.x * c_a, *pos_shift_maps[1], neg_size.x * converted_pos_size->y * sizeof(Tile));

				if (converted_neg_size->y >= c_a) {
					memmove(*pos_shift_maps[0], *neg_shift_maps[0], pos_size.x * c_a * sizeof(Tile));
					memmove(*pos_shift_maps[1], *neg_shift_maps[1], neg_size.x * c_a * sizeof(Tile));

					memmove(*neg_shift_maps[0], *neg_shift_maps[0] + pos_size.x * c_a, pos_size.x * (converted_neg_size->y - c_a) * sizeof(Tile));
					memmove(*neg_shift_maps[1], *neg_shift_maps[1] + neg_size.x * c_a, neg_size.x * (converted_neg_size->y - c_a) * sizeof(Tile));

					*neg_shift_maps[0] = reinterpret_cast<Tile*>(realloc(*neg_shift_maps[0], pos_size.x * (converted_neg_size->y - c_a) * sizeof(Tile)));
					*neg_shift_maps[1] = reinterpret_cast<Tile*>(realloc(*neg_shift_maps[1], neg_size.x * (converted_neg_size->y - c_a) * sizeof(Tile)));

					converted_neg_size->y -= c_a;
				}
				else {
					memset(*pos_shift_maps[0], 0, pos_size.x * c_a * sizeof(Tile));
					memset(*pos_shift_maps[1], 0, neg_size.x * c_a * sizeof(Tile));
				}

				converted_pos_size->y += c_a;
			}

			std::string Serialize() {
				std::string dat;
				dat += JSerial::Bits::Separate<pos2ds>(pos_size);
				dat += JSerial::Bits::Separate<pos2ds>(neg_size);
				dat += JSerial::Bits::Separate<pos2dus>(grid_size);
				dat += JSerial::Bits::Separate<pos2ds>(offset);

				const pos2ds quarter_sizes[4] = { pos_size, { neg_size.x, pos_size.y }, { pos_size.x, neg_size.y }, neg_size };

				for (unsigned char i = 0; i < 4; i++)
					dat += std::string(reinterpret_cast<char*>(quarters[i]), quarter_sizes[i].x * quarter_sizes[i].y * sizeof(Tile));
				return dat;
			}

			void Deserialize(const std::string& dat) {
				pos_size = JSerial::Bits::Merge<pos2ds>(dat.substr(0, sizeof(pos2ds)));
				neg_size = JSerial::Bits::Merge<pos2ds>(dat.substr(sizeof(pos2ds), sizeof(pos2ds)));
				grid_size = JSerial::Bits::Merge<pos2dus>(dat.substr(sizeof(pos2ds) * 2, sizeof(pos2dus)));
				offset = JSerial::Bits::Merge<pos2ds>(dat.substr(sizeof(pos2ds) * 3, sizeof(pos2ds)));

				const pos2ds quarter_sizes[4] = { pos_size, { neg_size.x, pos_size.y }, { pos_size.x, neg_size.y }, neg_size };

				unsigned int cursor_pos = 4 * sizeof(pos2ds);

				for (unsigned char i = 0; i < 4; i++) {
					const unsigned int total_quarter_size = quarter_sizes[i].x * quarter_sizes[i].y * sizeof(Tile);
					quarters[i] = reinterpret_cast<Tile*>(realloc(quarters[i], total_quarter_size));
					memcpy(quarters[i], dat.data() + cursor_pos, total_quarter_size);
					cursor_pos += total_quarter_size;
				}
			}

			inline pos2ds GetPosSize()
				{ return pos_size; }

			inline pos2ds GetNegSize()
				{ return neg_size; }

			inline Tile Lookup(pos2ds pos) {
				const pos2ds converted_quarter_size = { pos_size.x * (pos.x >= 0) + neg_size.x * !(pos.x >= 0),
														pos_size.y * (pos.y >= 0) + neg_size.y * !(pos.y >= 0) };

				const pos2ds converted_pos = { pos.x * (pos.x >= 0) + -(pos.x + 1) * !(pos.x >= 0),
											   pos.y * (pos.y >= 0) + -(pos.y + 1) * !(pos.y >= 0) };

				return (converted_pos.x < converted_quarter_size.x&& converted_pos.y < converted_quarter_size.y) ? quarters[((pos.y < 0) << 1) + (pos.x < 0)][converted_pos.y * converted_quarter_size.x + converted_pos.x] : empty;
			}

			Tile& operator[](pos2ds pos) {
				const pos2ds converted_quarter_size = { pos_size.x * (pos.x >= 0) + neg_size.x * !(pos.x >= 0),
														pos_size.y * (pos.y >= 0) + neg_size.y * !(pos.y >= 0) };

				const pos2ds converted_pos = { pos.x * (pos.x >= 0) + -(pos.x + 1) * !(pos.x >= 0),
											   pos.y * (pos.y >= 0) + -(pos.y + 1) * !(pos.y >= 0) };

				if (converted_pos.x >= converted_quarter_size.x)
					pos.x >= 0 ? ResizeXP(pos.x + 1) : ResizeXN(pos.x - 1);
				if (converted_pos.y >= converted_quarter_size.y)
					pos.y >= 0 ? ResizeYP(pos.y + 1) : ResizeYN(pos.y - 1);

				return quarters[((pos.y < 0) << 1) + (pos.x < 0)][converted_pos.y * converted_quarter_size.x + converted_pos.x];
			}
		};
	}
}