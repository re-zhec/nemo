#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "../State.hpp"

namespace nemo
{

class FreezeState : public State
{
public:
	virtual std::unique_ptr<State> 
	handleEvent(const sf::Event& event) 
	override;
	
	virtual void
	update(sf::RenderWindow& window) 
	override;
};

}