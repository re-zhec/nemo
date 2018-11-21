#pragma once
#include <SFML/Graphics.hpp>
#include "../include/Menu.hpp"

namespace fe
{

class StartScreen
{
public:
	StartScreen();
	void Refresh(sf::RenderWindow& window, sf::Event& event);
	bool IsFileLoaded() const;

private:
	Menu m_main_menu;
	Menu m_file_menu;
	Menu m_cfg_menu;

	int m_file;
};

}