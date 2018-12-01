#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "state/GameState.hpp"
#include "state/StartState.hpp"
#include "menu/Menu.hpp"
#include "menu/MenuCommand.hpp"

namespace sb
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

StartState::StartState()
	: main_menu_("json/menus/start.json")
{
	main_menu_
		.add(MainMenuKey::Play, "Play")
		.add(MainMenuKey::Continue, "Continue")
		.add(MainMenuKey::Settings, "Settings")
		.add(MainMenuKey::Quit, "Quit");
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
StartState::handleEvent(const sf::Event& event) &
{
	switch (event.type) 
	{
		case sf::Event::KeyPressed:
			std::cout << event.key.code << std::endl;
			if (const auto action = controls_.convert( Key{event.key.code} ); 
				action.has_value())
			{
				switch (*action) {
					case KeyAction::Cancel:
						// BOOST_LOG_TRIVIAL(debug) << "Pressed cancel key";
						break;

					case KeyAction::Up:
						// BOOST_LOG_TRIVIAL(debug) << "Pressed up key";
						command_ = std::make_unique<MenuUpCommand>();
						command_->execute(main_menu_);
						break;

					case KeyAction::Down:
						// BOOST_LOG_TRIVIAL(debug) << "Pressed down key.";
						command_ = std::make_unique<MenuDownCommand>();
						command_->execute(main_menu_);
						break;

					case KeyAction::Left:
						// BOOST_LOG_TRIVIAL(debug) << "Pressed left key.";
						command_ = std::make_unique<MenuLeftCommand>();
						command_->execute(main_menu_);
						break;

					case KeyAction::Right:
						// BOOST_LOG_TRIVIAL(debug) << "Pressed right key.";
						command_ = std::make_unique<MenuRightCommand>();
						command_->execute(main_menu_);
						break;
				}
			}
			break;
		
		default:
			break;
	}
}

void StartState::update(sf::RenderWindow& window) &
{
	window.clear(sf::Color::White);
	main_menu_.draw(window);
	window.display();
}

}