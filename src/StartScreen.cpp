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
	m_main_menu.addOption(MenuOptionKey::Play, "Play");
	m_main_menu.addOption(MenuOptionKey::Options, "Options");
	m_main_menu.addOption(MenuOptionKey::Exit, "Exit");
	m_main_menu.addOption(MenuOptionKey::Play, "Test1");
	m_main_menu.addOption(MenuOptionKey::Play, "Test2");
	m_main_menu.addOption(MenuOptionKey::Play, "Test3");
	m_main_menu.addOption(MenuOptionKey::Play, "Test4");
	m_main_menu.addOption(MenuOptionKey::Play, "Test5");
	m_main_menu.addOption(MenuOptionKey::Play, "Test6");
	m_main_menu.addOption(MenuOptionKey::Play, "Test7");
	m_main_menu.addOption(MenuOptionKey::Play, "Test8");
	m_main_menu.addOption(MenuOptionKey::Play, "Test9");
	m_main_menu.addOption(MenuOptionKey::Play, "Test10");
	m_main_menu.addOption(MenuOptionKey::Play, "Test11");
	m_main_menu.addOption(MenuOptionKey::Play, "Test12");
	m_main_menu.addOption(MenuOptionKey::Options, "Options");
	m_main_menu.addOption(MenuOptionKey::Options, "Options");
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
						switch (const auto [key, id] = *selected;
							key)
						{
							case MenuOptionKey::Play:
								m_main_menu.setOptionText(
									MenuOptionKey::Play,
									"Poop"
								);
								break;
							case MenuOptionKey::Options:
								m_main_menu.delOption(
									MenuOptionKey::Options,
									"Options"
								);
								break;
							case MenuOptionKey::Exit:
								window.close();
								LOG_DEBUG("Window closed");
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