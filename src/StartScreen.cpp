#include "../include/logger.hpp"
#include "../include/Menu.hpp"
#include "../include/StartScreen.hpp"

namespace rp
{

StartScreen::StartScreen()
	// : m_main_menu("xml/start_menu.xml")
	: m_main_menu("xml/menu/start.xml")
	, m_inv_menu({40.f, 40.f}, {600.f, 150.f}, 2, 4)
	, m_cfg_menu({0.f, 0.f}, {1280.f, 720.f}, 4, 8)
	, m_file(0)
{
	m_main_menu
		.add(StartMenuKey::Play, "Play")
		.add(StartMenuKey::Continue, "Continue")
		.add(StartMenuKey::Settings, "Settings")
		.add(StartMenuKey::Quit, "Quit");

	m_inv_menu.add(StartMenuKey::Play, "Play");
}

void StartScreen::Update(sf::RenderWindow& window, sf::Event& event)
{
	switch (event.type) 
	{
		case sf::Event::KeyPressed:
			// key pressed
			switch (event.key.code) {
				case sf::Keyboard::Backspace:
					LOG_DEBUG("backspace key pressed");
					// go back
					break;

				case sf::Keyboard::W:
					LOG_DEBUG("'w' key pressed");
					m_main_menu.moveUp();
					break;
				
				case sf::Keyboard::S:
					LOG_DEBUG("'s' key pressed");
					m_main_menu.moveDown();
					break;
				
				case sf::Keyboard::A:
					LOG_DEBUG("'a' key pressed");
					m_main_menu.moveLeft();
					break;
				
				case sf::Keyboard::D:
					LOG_DEBUG("'d' key pressed");
					m_main_menu.moveRight();
					break;
				
				case sf::Keyboard::Enter:
					LOG_DEBUG("enter key pressed");
					
					if (const auto selected = m_main_menu.cursorAt()) {
						switch (*selected) {
							case StartMenuKey::Play:
								m_main_menu.setOptionText(*selected, "Poop");
								break;

							case StartMenuKey::Continue:
								m_file = 1;
								break;

							case StartMenuKey::Settings:
								m_main_menu.remove(*selected);
								break;

							case StartMenuKey::Quit:
								window.close();
								break;
							
							default:
								m_main_menu.remove(*selected);
								break;
						}
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

	sf::RectangleShape screen({1280.f, 720.f});
	screen.setFillColor(sf::Color::White);
	window.draw(screen);
	m_main_menu.draw(window);
}

bool StartScreen::IsFileLoaded() const
{
	return m_file != 0;
}

}