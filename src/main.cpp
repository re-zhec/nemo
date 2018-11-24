#include <SFML/Graphics.hpp>
#include "../include/Game.hpp"
#include "../include/StartScreen.hpp"
#include "../include/Menu.hpp"
#include "../include/logger.hpp"
#include "../include/PartyChar.hpp"
#include "../include/Item.hpp"
#include "../include/Weapon.hpp"
#include "../include/Inventory.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Fire Emblem");
	window.setFramerateLimit(30);
	window.setKeyRepeatEnabled(false);
	LOG_DEBUG("Window opened");

	rp::GameState state;
	rp::StartScreen start;

	// run the program as long as window is opened
	while (window.isOpen())
	{
		sf::Event event;

		// there are pending events
		if (window.pollEvent(event))
		{
			window.clear();
			// Regardless of what state the game is in, pause the game when the 
			// window loses focus and resume the game when the window regains 
			// focus
			if (event.type == sf::Event::LostFocus) {
				// pause game
				LOG_DEBUG("window lost focus");
			}
			
			else if (event.type == sf::Event::GainedFocus) {
				// resume game
				LOG_DEBUG("window gained focus");
			}

			else if (event.type == sf::Event::Closed) {
				// close window
				window.close();
				LOG_DEBUG("Window closed");
			}
			else {
				switch (state)
				{
					case rp::GameState::Start:
						start.Refresh(window, event);
						if (start.IsFileLoaded()) {
							state = rp::GameState::WorldMap;
						}
						break;

					case rp::GameState::WorldMap:
						window.close();
						break;

					case rp::GameState::LoadSave:
						break;

					default:
						break;
				}
			}
		}

		window.display();
	}

	return 0;
}