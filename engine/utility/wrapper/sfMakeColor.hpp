#pragma once

#include <array>
#include <SFML/Graphics/Color.hpp>

namespace nemo
{
	/**
	 * @brief Create a SFML Color object out of an array of four integers 
	 * representing RGBA attributes.
	 * 
	 * @param rgba     - An array containing RGBA values in this order: red, 
	 *                   green, blue, and alpha (opacity).
	 * 
	 * @return SFML Color object.
	 */
	sf::Color
	sfMakeColor(const std::array<uint8_t, 4>& rgba);
}