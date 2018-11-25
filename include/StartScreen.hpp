#pragma once
#include <SFML/Graphics.hpp>
#include "../include/Menu.hpp"

namespace rp
{

class StartScreen
{
public:
	enum class StartMenuKey {
		Play,
		Continue,
		Settings,
		Quit
	};

	StartScreen();
	void Update(sf::RenderWindow& window, sf::Event& event);
	bool IsFileLoaded() const;

private:
	Menu<StartMenuKey> m_main_menu;
	Menu<StartMenuKey> m_inv_menu;
	Menu<StartMenuKey> m_cfg_menu;

	int m_file;
};

}