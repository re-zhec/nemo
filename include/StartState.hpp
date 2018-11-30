#pragma once

#include <SFML/Graphics.hpp>

#include "GameState.hpp"
#include "Menu.hpp"

namespace rp
{

class StartState : public GameState
{
public:
	StartState();
	virtual void handleEvent(const sf::Event& event) & override;
	virtual void update(sf::RenderWindow& window) & override;

private:
	enum MainMenuKey {
		Play,
		Continue,
		Settings,
		Quit,
		Default
	};

	Menu m_main_menu;
};

}