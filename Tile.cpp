#include "Tile.hpp"
#include <stdexcept>
#include <DxLib.h>
#include "Handle.hpp"
#include "BinIO.hpp"
#include "convert_string.hpp"

Point<int> Tile::size(1, 1);

Tile::Norm::Norm(const char8_t* graph, unsigned short tnum)
{
	path = graph;
	GetGraphSize(Handle::get(path, Handle::type::graph), &num.x, &num.y);
	num /= size;

	// dataÇÃèâä˙âª
	data.resize(tnum);
}

Tile::Ret Tile::Norm::get(int id, int t)const
{
	if (data.size() <= id)
		throw std::out_of_range("out of range");
	return { Handle::get(path, Handle::type::graph),num(id) * size,data[id] };
}

Tile::Anim::Anim(const char8_t* graph, unsigned short tnum, unsigned char type, int pattern) :pattern(pattern), type(type)
{
	path = graph;
	GetGraphSize(Handle::get(path, Handle::type::graph), &num.x, &num.y);
	num /= size;
	if (type & Mode::horizontal)
		num.y /= pattern;
	else
		num.x /= pattern;

	// dataÇÃèâä˙âª
	data.resize(tnum);
}

Tile::Ret Tile::Anim::get(int id, int t)const
{
	if (data.size() <= id)
		throw std::out_of_range("out of range");
	auto buf = num(id);
	if (type & Mode::step)
	{
		t %= (pattern - 1) * 2;
		if (t >= pattern)
			t = (pattern - 1) * 2 - t;
	}
	else
		t %= pattern;
	if (type & Mode::horizontal)
		buf.y += num.y * t;
	else
		buf.x += num.x * t;
	return { Handle::get(path, Handle::type::graph),buf * size,data[id] };
}

void Tile::load(const char* file)
{
	BinIO ifs(file, std::ios_base::in);
	try
	{
		auto num = ifs.read<unsigned char>();
		for (unsigned char i = 0; i < num; ++i)
		{
			auto type = ifs.read<unsigned char>();
			auto pathLength = ifs.read<unsigned char>();
			std::string path;
			path.resize(pathLength);
			ifs.fst.read(path.data(), pathLength);
			auto tileNum = ifs.read<unsigned short>();
			if (type & 1)
			{
				// ÉAÉjÉÅ
				auto pattern = ifs.read<unsigned char>();
				data.emplace_back(new Anim(ext::to<char8_t>(path), tileNum, type >> 1, pattern));
			}
			else
			{
				// í èÌ
				data.emplace_back(new Norm(ext::to<char8_t>(path), tileNum));
			}
			for (unsigned short i = 0; i < tileNum; ++i)
			{
				auto tag = ifs.read<unsigned char>();
				auto flag = ifs.read<unsigned char>();
				data.back()->data.emplace_back(tag, flag);
			}
		}
	}
	catch (std::runtime_error e)
	{
		throw e;
	}
}

Tile::Ret Tile::get(int id, int t)
{
	for (const auto& i : data)
		if (id < static_cast<int>(i->data.size()))
			return i->get(id, t);
		else
			id -= static_cast<int>(i->data.size());
	throw std::out_of_range("out of range");
}

