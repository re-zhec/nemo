#include <optional>
#include <memory>
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "TitleState.hpp"
#include "state/State.hpp"
#include "menu/Menu.hpp"


namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

TitleState::TitleState()
	: MenuState(Menu("data/menus/start.json"))
{
	menu_.add(TitleOption::Play,   "Play"    )
		.add(TitleOption::Continue, "Continue")
		.add(TitleOption::Settings, "Settings")
		.add(TitleOption::Quit,     "Quit"    );
}

std::unique_ptr<State> 
TitleState::handleEvent(const sf::Event& event) 
{
	const auto action = controls_.convert(event);
	std::unique_ptr<State> new_state = nullptr;

	if (!action) {
		return nullptr;
	}

	if (const auto selected = menu_handler_.handleAction(menu_, *action);
		selected)
	{
		switch (*selected) {
			case TitleOption::Settings:
				new_state = nullptr;
				break;
			
			case TitleOption::Continue:
				new_state = nullptr;
				break;
		}
	}

	return new_state;
}
	
void 
TitleState::update(sf::RenderWindow& window) 
{
	window.clear(sf::Color::White);
	menu_.draw(window);
}

}