#include <memory>
#include <stack>
#include <optional>
#include <utility>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "Game.hpp"
#include "key/KeyControls.hpp"

int main()
{
	nemo::Game game;
	nemo::KeyControls controls_;

	// Open a window.
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Nemo");
	window.setFramerateLimit(30);
	window.setKeyRepeatEnabled(false);

	// Run the program as long as its window is open.
	while (window.isOpen()) {
		window.clear(sf::Color::White);
		std::optional<nemo::KeyAction> input;

		// Check for pending events.
		if (sf::Event event;
			window.pollEvent(event))
		{
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
				break;

				case sf::Event::LostFocus:
					game.pause();
				break;

				case sf::Event::GainedFocus:
					game.resume();
				break;

				case sf::Event::KeyPressed: {
					const nemo::Key pressed_key(event.key.code);
					input = { controls_.convert(nemo::Key(event.key.code)) };
				}
				break;
				
				default:
				break;
			}
		}

		game.update(input, window);
		window.display();
	}

	return EXIT_SUCCESS;
}