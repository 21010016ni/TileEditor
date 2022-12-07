#include "Game.hpp"
#include <filesystem>
#include "Config.hpp"
#include "Input.hpp"
//#include "Particle.hpp"
#include "convert_string.hpp"

#include "UI.hpp"
#include "TileSetEdit.hpp"
#include "Handle.hpp"

Draw display(0, 0, 0);

static TCHAR buf[256] = {0};
Point<int> mouse_click;
Point<int> mouse_prev;

enum class Appstate
{
	non_file,
	file_read_error,
	file_loaded
};

Appstate appstate = Appstate::non_file;

void Game::preset()
{
	//Particle::set<Dust>(20);
}

Game::Message Game::update()
{
	if (Keyboard::push(VK_ESCAPE))
	{
		return Message::quit;
	}
	switch(appstate)
	{
	case Appstate::file_loaded:
		if(Mouse::click(MOUSE_INPUT_1))
		{
			mouse_click = Mouse::pos();
		}
		else if(Mouse::releace(MOUSE_INPUT_1))
		{
			if(mouse_click == Mouse::pos())
			{
				TileSetEdit::SelectTile(Mouse::pos());
			}
		}
		else if(Mouse::button(MOUSE_INPUT_1) && Mouse::type())
		{
			TileSetEdit::display.pos += Mouse::pos() - mouse_prev;
		}
		break;
	}
	while (GetDragFileNum() > 0)
	{
		GetDragFilePath(buf);
		std::u8string path = (char8_t*)buf;
		if(path.find(std::filesystem::current_path().u8string()) == std::string::npos)
		{
			appstate = Appstate::file_read_error;
		}
		else
		{
			TileSetEdit::load(path.substr(std::filesystem::current_path().u8string().size() + 1, std::string::npos));
			appstate = Appstate::file_loaded;
		}
	}
	mouse_prev = Mouse::pos();

	//Particle::update();
	return Message::none;
}

void Game::draw()
{
	switch(appstate)
	{
	case Appstate::non_file:
		UI::draw.string(common::width / 2, common::height / 2, u8"ファイルをドロップしてください", 0xffffff, ref::center | ref::middle);
		break;
	case Appstate::file_read_error:
		UI::draw.string(common::width / 2, common::height / 2 - 20, u8"ファイルの読み込みに失敗しました", 0xffffff, ref::center | ref::middle);
		UI::draw.string(common::width / 2, common::height / 2, u8"もう一度ファイルをドロップしてください", 0xffffff, ref::center | ref::middle);
		UI::draw.string(common::width / 2, common::height / 2 + 20, u8"（このアプリ以下のディレクトリにある画像ファイルのみ読み込めます）", 0xffffff, ref::center | ref::middle);
		break;
	case Appstate::file_loaded:
		TileSetEdit::draw();
		break;
	}
	//Particle::draw();
}

