#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "../MenuState.hpp"
#include "menu/Menu.hpp"
#include "menu/MenuHandler.hpp"

namespace nemo
{

class TitleState : public MenuState
{
public:
	TitleState();

	virtual std::unique_ptr<State> 
	handleEvent(const sf::Event& event);
	
	virtual void
	update(sf::RenderWindow& window) 
	override;

private:
	enum TitleOption {
		Play,
		Continue,
		Settings,
		Quit
	};
};

}