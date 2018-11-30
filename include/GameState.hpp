#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "KeyControls.hpp"

namespace rp
{

class GameState
{
public:
	virtual void handleEvent(const sf::Event& event) & {};
	virtual void update(sf::RenderWindow& window) & {};

protected:
	KeyControls controls_;
};

}