#include <SFML/Graphics.hpp>
#include "../include/Game.hpp"
#include "../include/StartScreen.hpp"
#include "../include/PauseScreen.hpp"
#include "../include/Menu.hpp"
#include "../include/Weapon.hpp"
#include "../include/Item.hpp"
#include "../include/logger.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Our Secret Base");
	window.setFramerateLimit(30);
	window.setKeyRepeatEnabled(false);
	LOG_DEBUG("Window opened");

	rp::Inventory inv(1000);
	std::shared_ptr<rp::Item> weap = std::make_shared<rp::Weapon>(rp::ItemID::Bolgano);
	inv.add(weap);
	inv.add(weap);
	inv.add(weap);

	rp::GameState state = rp::GameState::Start;
	rp::StartScreen start;
	rp::PauseScreen pause(std::make_shared<rp::Inventory>(inv));

	// run the program as long as window is opened
	while (window.isOpen())
	{
		sf::Event event;

		// there are pending events
		if (window.pollEvent(event))
		{
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
						start.Update(window, event);

						if (start.IsFileLoaded()) {
							LOG_DEBUG("pause screen");
							state = rp::GameState::Pause;
						}
						break;

					case rp::GameState::Pause:
						pause.Update(window, event);
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