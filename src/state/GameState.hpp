#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "key/KeyControls.hpp"

namespace sb
{

class GameState
{
public:
	virtual void 
	handleEvent(const sf::Event& event) 
	& = 0;
	
	virtual void 
	update(sf::RenderWindow& window) 
	& = 0;

protected:
	KeyControls controls_;
};

}