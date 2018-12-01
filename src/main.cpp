#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "state/GameState.hpp"
#include "state/StartState.hpp"

int main()
{
	// initBoostLogging();

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Our Secret Base");
	window.setFramerateLimit(30);
	window.setKeyRepeatEnabled(false);
	// BOOST_LOG_TRIVIAL(debug) << "Window opened.";

	std::unique_ptr<sb::GameState> state = std::make_unique<sb::StartState>();

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
				// BOOST_LOG_TRIVIAL(debug) << "Window lost focus.";
			}
			
			else if (event.type == sf::Event::GainedFocus) {
				// resume game
				// BOOST_LOG_TRIVIAL(debug) << "Window gained focus.";
			}

			else if (event.type == sf::Event::Closed) {
				// close window
				window.close();
				// BOOST_LOG_TRIVIAL(debug) << "Window closed.";
			}
			else {
				state->handleEvent(event);
			}
		}

		state->update(window);
	}

	return 0;
}