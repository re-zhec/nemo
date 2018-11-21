#include "../include/logger.hpp"
#include "../include/Menu.hpp"
#include "../include/StartScreen.hpp"

namespace fe
{

StartScreen::StartScreen()
	: m_main_menu({40.f, 40.f}, 600.f, 250.f, 2, 4)
	, m_file_menu({40.f, 40.f}, 200.f, 600.f, 1, 4)
	, m_cfg_menu({0.f, 0.f}, 1280.f, 720.f, 4, 8)
	, m_file(0)
{
	m_main_menu.addOption(MainMenuKey::Play, "Play");
	m_main_menu.addOption(MainMenuKey::Continue, "Continue");
	m_main_menu.addOption(MainMenuKey::Settings, "Settings");
	m_main_menu.addOption(MainMenuKey::Quit, "Quit");
}

void StartScreen::Refresh(sf::RenderWindow& window, sf::Event& event)
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
					
					if (const auto selected = m_main_menu.getHoveredOption()) {
						switch (*selected) {
							case MainMenuKey::Play:
								m_main_menu.setOptionText(*selected, "Poop");
								break;

							case MainMenuKey::Continue:
								m_file = 1;
								break;

							case MainMenuKey::Settings:
								m_main_menu.delOption(*selected);
								break;

							case MainMenuKey::Quit:
								window.close();
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

	m_main_menu.draw(window);
}

bool StartScreen::IsFileLoaded() const
{
	return m_file != 0;
}

}