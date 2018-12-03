#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "state/GameState.hpp"
#include "state/MenuState.hpp"
#include "menu/Menu.hpp"
#include "utility/type/MenuOptionID.hpp"

namespace sb
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuState::MenuState()
	: menu_("data/menus/start.json")
{
	menu_
		.add( MenuOptionID(MenuChoice::Play),     "Play"     )
		.add( MenuOptionID(MenuChoice::Continue), "Continue" )
		.add( MenuOptionID(MenuChoice::Settings), "Settings" )
		.add( MenuOptionID(MenuChoice::Quit),     "Quit"     );
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
MenuState::handleEvent(const sf::Event& event) 
&
{
	switch (event.type) 
	{
		case sf::Event::KeyPressed:
			if (const auto 
				action = controls_.convert( Key(event.key.code) ); 
				action.has_value())
			{
				switch (action.value()) {
					case KeyAction::Cancel:
						// BOOST_LOG_TRIVIAL(debug) << "Pressed cancel key";
						break;

					case KeyAction::Up:
						// BOOST_LOG_TRIVIAL(debug) << "Pressed up key";
						menu_.moveUp();
						break;

					case KeyAction::Down:
						// BOOST_LOG_TRIVIAL(debug) << "Pressed down key.";
						menu_.moveDown();
						break;

					case KeyAction::Left:
						// BOOST_LOG_TRIVIAL(debug) << "Pressed left key.";
						menu_.moveLeft();
						break;

					case KeyAction::Right:
						// BOOST_LOG_TRIVIAL(debug) << "Pressed right key.";
						menu_.moveRight();
						break;

					case KeyAction::Select:
						menu_.cursorAt();
						break;

					default:
						break;
				}
			}
			break;
		
		default:
			break;
	}
}

void MenuState::update(sf::RenderWindow& window) &
{
	window.clear(sf::Color::White);
	menu_.draw(window);
	window.display();
}

}