#pragma once
#include <SFML/Graphics.hpp>


namespace rp
{

class GameState
{
public:
	virtual void handleEvent(const sf::Event& event) & {};
	virtual void update(sf::RenderWindow& window) & {};
};

}