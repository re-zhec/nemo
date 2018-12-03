#pragma once

#include <stack>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "GameState.hpp"
#include "menu/Menu.hpp"

namespace sb
{

class TitleState : public GameState
{
public:
	virtual void 
	handleEvent(const sf::Event& event) 
	& override;
	
	virtual void 
	update(sf::RenderWindow& window) 
	& override;

private:
	std::stack<Menu> menus_;
};
}