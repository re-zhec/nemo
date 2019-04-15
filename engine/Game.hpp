#pragma once

#include <optional>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

#include "player/MenuPlayer.hpp"
#include "utility/type/Key.hpp"

namespace nemo
{

class Game
{
public:
	/***
	 * @brief Construct the game.
	 ***/
	Game();

	/***
	 * @brief Pause the game.
	 ***/
	void 
	pause();

	/***
	 * @brief Resume the game.
	 ***/
	void 
	resume();

	/***
	 * @brief Process a frame in the game.
	 * 
	 * @param key         - Player input.
	 * @param window      - Render window.
	 ***/
	void 
	update(
		const std::optional<KeyAction> key, 
		sf::RenderWindow& window);

private:
	bool running_;
	MenuPlayer menu_player_;
};

}