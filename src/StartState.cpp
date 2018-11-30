#include <SFML/Graphics.hpp>

#include "../include/GameState.hpp"
#include "../include/StartState.hpp"
#include "../include/Menu.hpp"

namespace rp
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

StartState::StartState()
	: m_main_menu("json/menus/start.json")
{
	m_main_menu
		.add(MainMenuKey::Play, "Play")
		.add(MainMenuKey::Continue, "Continue")
		.add(MainMenuKey::Settings, "Settings")
		.add(MainMenuKey::Quit, "Quit");
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void StartState::handleEvent(const sf::Event& event) &
{
	switch (event.type) 
	{
		case sf::Event::KeyPressed:
			// key pressed
			switch (event.key.code) {
				case sf::Keyboard::Backspace:
					// BOOST_LOG_TRIVIAL(debug) << "Pressed backspace key.";
					// go back
					break;

				case sf::Keyboard::W:
					// BOOST_LOG_TRIVIAL(debug) << "Pressed 'W' key.";
					m_main_menu.moveUp();
					break;
				
				case sf::Keyboard::S:
					// BOOST_LOG_TRIVIAL(debug) << "Pressed 'S' key.";
					m_main_menu.moveDown();
					break;
				
				case sf::Keyboard::A:
					// BOOST_LOG_TRIVIAL(debug) << "Pressed 'A' key.";
					m_main_menu.moveLeft();
					break;
				
				case sf::Keyboard::D:
					// BOOST_LOG_TRIVIAL(debug) << "Pressed 'D' key.";
					m_main_menu.moveRight();
					break;
				
				case sf::Keyboard::Enter:
					// BOOST_LOG_TRIVIAL(debug) << "Pressed Enter key.";
					
					switch (const auto selected = m_main_menu.cursorAt();
						selected.value_or(MainMenuKey::Default))
					{
						case MainMenuKey::Play:
							m_main_menu.changeOptionText(*selected, "Poop");
							break;

						case MainMenuKey::Continue:
							break;

						case MainMenuKey::Settings:
							m_main_menu.remove(*selected);
							break;

						case MainMenuKey::Quit:
							break;
						
						default:
							m_main_menu.remove(*selected);
							break;
					}
						
					// confirm selection
					break;
				
				default:
					break;
			}
			break;
		
		default:
			break;
	}
}

void StartState::update(sf::RenderWindow& window) &
{
	window.clear(sf::Color::White);
	m_main_menu.draw(window);
	window.display();
}

}