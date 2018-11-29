#include "../include/Logger.hpp"
#include "../include/Menu.hpp"
#include "../include/StartScreen.hpp"

namespace rp
{

StartScreen::StartScreen()
	: m_main_menu("json/menu/start.json")
	// : m_main_menu({40.f, 40.f}, {600.f, 150.f}, 2, 4)
	, m_inv_menu({40.f, 40.f}, {600.f, 150.f}, 2, 4)
	, m_cfg_menu({0.f, 0.f}, {1280.f, 720.f}, 4, 8)
	, m_file(0)
{
	m_main_menu
		.add(MainMenuKey::Play, "Play")
		.add(MainMenuKey::Continue, "Continue")
		.add(MainMenuKey::Settings, "Settings")
		.add(MainMenuKey::Quit, "Quit");

	m_inv_menu.add(MainMenuKey::Play, "Play");
}

void StartScreen::Update(sf::RenderWindow& window, sf::Event& event)
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
							m_file = 1;
							break;

						case MainMenuKey::Settings:
							m_main_menu.remove(*selected);
							break;

						case MainMenuKey::Quit:
							window.close();
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

	window.clear(sf::Color::White);
	m_main_menu.draw(window);
}

bool StartScreen::IsFileLoaded() const
{
	return m_file != 0;
}

}