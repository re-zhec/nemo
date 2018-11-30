#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "../include/GameState.hpp"
#include "../include/StartState.hpp"
#include "../include/Menu.hpp"
#include "../include/MenuCommand.hpp"

namespace rp
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

			if (event.key.code == controls_.cancel_) {
				// BOOST_LOG_TRIVIAL(debug) << "Pressed cancel key";
			}

			if (event.key.code == controls_.up_) {
				// BOOST_LOG_TRIVIAL(debug) << "Pressed up key";
				command_ = std::make_unique<MenuUpCommand>();
				command_->execute(main_menu_);
			}

			if (event.key.code == controls_.down_) {
				// BOOST_LOG_TRIVIAL(debug) << "Pressed down key.";
				command_ = std::make_unique<MenuDownCommand>();
				command_->execute(main_menu_);
			}

			if (event.key.code == controls_.left_) {
				// BOOST_LOG_TRIVIAL(debug) << "Pressed left key.";
				command_ = std::make_unique<MenuLeftCommand>();
				command_->execute(main_menu_);
			}

			if (event.key.code == controls_.right_) {
				// BOOST_LOG_TRIVIAL(debug) << "Pressed left key.";
				command_ = std::make_unique<MenuRightCommand>();
				command_->execute(main_menu_);
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