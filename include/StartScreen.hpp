#pragma once
#include <SFML/Graphics.hpp>
#include "../include/Menu.hpp"

namespace fe
{

class StartScreen
{
public:
	enum class MainMenuKey {
		Play,
		Continue,
		Settings,
		Quit
	};

	StartScreen();
	void Refresh(sf::RenderWindow& window, sf::Event& event);
	bool IsFileLoaded() const;

private:
	Menu<MainMenuKey> m_main_menu;
	Menu<MainMenuKey> m_file_menu;
	Menu<MainMenuKey> m_cfg_menu;

	int m_file;
};

}