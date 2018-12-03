#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "GameState.hpp"
#include "menu/Menu.hpp"

namespace sb
{

class MenuState : public GameState
{
public:
	MenuState();
	
	virtual void 
	handleEvent(const sf::Event& event) 
	& override;
	
	virtual void 
	update(sf::RenderWindow& window) 
	& override;

private:
	enum MenuChoice {
		Play,
		Continue,
		Settings,
		Quit
	};

	Menu menu_;
};

}