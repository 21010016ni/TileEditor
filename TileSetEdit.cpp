#include "TileSetEdit.hpp"
#include <iomanip>
#include <sstream>
#include <DxLib.h>
#include "UI.hpp"
#include "Config.hpp"
#include "convert_string.hpp"

Draw TileSetEdit::display(0, 0, 0);
Point<int> TileSetEdit::select(-1, -1);
int TileSetEdit::wait = 30;
Point<int> TileSetEdit::size(48, 48);

TileSetEdit::Tile::Tile(const std::u8string& file) :file(file), type(TileType::normal), pattern(1)
{
	GetGraphSize(Handle::get(file, Handle::type::graph), &num.x, &num.y);
	num /= size;
	data.resize(num.y * num.x);
}

TileSetEdit::Tile::Tile(const std::u8string& file, BitFlag type, unsigned char pattern) :file(file), type(type | TileType::anime), pattern(pattern)
{
	GetGraphSize(Handle::get(file, Handle::type::graph), &num.x, &num.y);
	num /= size;
	if(type(1))
		num.y /= pattern;
	else
		num.x /= pattern;
	data.resize(num.y * num.x);
}

void TileSetEdit::SelectTile(const Point<int>& pos)
{
	select = (pos - display.pos) / size;
	int y = 0;
	for(const auto& i : tile)
	{
		if(select.y >= y && select.y < y + i.num.y)
		{
			if(select.x < 0 || select.x >= i.num.x)
				select = {-1,-1};
			break;
		}
		y += i.num.y;
	}
}

void TileSetEdit::load(const std::u8string& file)
{
	tile.emplace_back(file);
}

void TileSetEdit::draw()
{
	int y = 0;
	for(const auto& i : tile)
	{
		display.graph(0, y * size.y, Handle::get(i.file, Handle::type::graph), true);
		if(select.y >= y && select.y < y + i.num.y)
		{
			//display.blend(blendmode::sub, 128);
			display.box(select * size, size, 0xffffff, false);
			//display.blend();
			UI::draw.string(0, 5, i.file, 0xffffff);
			UI::draw.string(0, 30, u8"タイプ:", 0xffffff);
			if(i.type(0))
			{
				UI::draw.string(60, 30, u8"アニメ", 0xffffff);
				if(i.type(1))
					UI::draw.string(0, 55, u8"垂直", 0xffffff);
				else
					UI::draw.string(0, 55, u8"水平", 0xffffff);
				if(i.type(2))
					UI::draw.string(60, 55, u8"Step", 0xffffff);
				else
					UI::draw.string(60, 55, u8"Walk", 0xffffff);
			}
			else
			{
				UI::draw.string(60, 30, u8"通常", 0xffffff);
			}
			UI::draw.string(0, 80, ext::vto<char8_t>((select.y - y) * i.num.x + select.x), 0xffffff);
			std::stringstream ss;
			ss << std::setfill('0') << std::hex << std::setw(3) << i.data[(select.y - y) * i.num.x + select.x].tag;
			UI::draw.string(40, 80, ext::convert<char8_t>(ss.str()), 0xffffff);
			ss.str("");
			ss << std::setfill('0') << std::hex << std::setw(3) << i.data[(select.y - y) * i.num.x + select.x].flag;
			UI::draw.string(70, 80, ext::convert<char8_t>(ss.str()), 0xffffff);
		}
		y += i.num.y;
	}
}
