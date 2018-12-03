#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "state/GameState.hpp"
#include "state/MenuState.hpp"

int main()
{
	// initBoostLogging();

	// Open a window.
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Our Secret Base");
	window.setFramerateLimit(30);
	window.setKeyRepeatEnabled(false);
	// BOOST_LOG_TRIVIAL(debug) << "Window opened.";

	std::unique_ptr<sb::GameState> state = std::make_unique<sb::MenuState>();

	// Run the program as long as its window is open.
	while (window.isOpen())
	{
		sf::Event event;

		// Check for pending events.
		if (window.pollEvent(event))
		{
			switch (event.type) {
				// These cases apply to any state of the game.
				case sf::Event::LostFocus:
					// Pause the game.
					// BOOST_LOG_TRIVIAL(debug) << "Window lost focus.";
					break;
				case sf::Event::GainedFocus:
					// Resume the game.
					// BOOST_LOG_TRIVIAL(debug) << "Window gained focus.";
					break;

				case sf::Event::Closed:
					// Close the window.
					window.close();
					// BOOST_LOG_TRIVIAL(debug) << "Window closed.";
					break;
				default:
					// The other events depend on the current state of the game.
					state->handleEvent(event);
					break;
			}
		}

		// Draw the updated render window on screen.
		state->update(window);
	}

	return EXIT_SUCCESS;
}