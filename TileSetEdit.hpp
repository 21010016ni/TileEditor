#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Point.hpp"
#include "BitFlag.hpp"
#include "Handle.hpp"
#include "Draw.hpp"

namespace TileType
{
	constexpr BitFlag
		normal(0),
		anime(1),
		horizontal(0),
		vertical(2),
		walk(0),
		step(4);
}

class TileSetEdit
{
public:
	struct Data
	{
		unsigned char tag;
		unsigned char flag;
	};
	struct Tile
	{
		std::u8string file;
		Point<int> num;
		BitFlag type;
		unsigned char pattern;
		std::vector<Data> data;

		Tile(const std::u8string& file);
		Tile(const std::u8string& file, BitFlag type, unsigned char pattern);
	};
	
	static inline int t = 0;
	static inline int count = 0;
	static Point<int> select;

	static int wait;

	static Draw display;

	static Point<int> size;
	static inline std::vector<Tile> tile;

	static void SelectTile(const Point<int>& pos);
	
	static void load(const std::u8string& file);
	static void draw();
};

