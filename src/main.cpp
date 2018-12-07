#include <memory>
#include <stack>
#include <utility>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "state/State.hpp"
#include "state/State/MenuState/TitleState.hpp"
#include "state/State/FreezeState.hpp"

int main()
{
	// initBoostLogging();

	// Open a window.
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Nemo");
	window.setFramerateLimit(30);
	window.setKeyRepeatEnabled(false);
	// BOOST_LOG_TRIVIAL(debug) << "Window opened.";

	// Start game at the title screen.
	std::stack< std::unique_ptr<nemo::State> > states;
	states.push(std::make_unique<nemo::TitleState>());

	// Run the program as long as its window is open.
	while (window.isOpen())
	{
		std::unique_ptr<nemo::State> new_state = nullptr;

		// Check for pending events.
		if (sf::Event event;
			window.pollEvent(event))
		{
			switch (event.type) {
				// These cases apply to any state of the game.
				case sf::Event::Closed:
					// Close the window.
					// BOOST_LOG_TRIVIAL(debug) << "Window closed.";
					window.close();
					break;

				case sf::Event::LostFocus:
					// Pause the game.
					// BOOST_LOG_TRIVIAL(debug) << "Window lost focus.";
					states.push(std::make_unique<nemo::FreezeState>());
					break;

				case sf::Event::GainedFocus:
					// Resume the game.
					// BOOST_LOG_TRIVIAL(debug) << "Window gained focus.";
					states.pop();
					break;

				default:
					// The other events depend on the current state of the game.
					new_state = states.top()->handleEvent(event);
					break;
			}
		}

		// Update the render window.
		states.top()->update(window);
		window.display();

		if (new_state != nullptr) {
			states.push(std::move(new_state));
		}
	}

	return EXIT_SUCCESS;
}