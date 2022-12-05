#include "Game.hpp"
#include "Config.hpp"
#include "Input.hpp"
//#include "Particle.hpp"
#include "convert_string.hpp"

#include "UI.hpp"

#include <iostream>

Draw display(0, 0, 0);

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
	while (GetDragFileNum() > 0)
	{
		std::u8string buf;
		buf.resize(256);
		GetDragFilePath((TCHAR*)buf.data());
		printfDx("%s\n", ext::to<char>(buf));
	}

	//Particle::update();
	return Message::none;
}

void Game::draw()
{
	UI::draw.string(common::width / 2, common::height / 2, u8"ファイルをドロップしてください", 0xffffff, ref::center | ref::middle);
	//Particle::draw();
}

