#pragma once

#include <array>
#include <SFML/Graphics/Color.hpp>

namespace nemo
{
	/**
	 * \brief Create a sf::Color object out of an array of four integers 
	 * representing RGB attributes.
	 * 
	 * \param rgba    An array containing RGB values in this order: red, green, 
	 *                blue, and alpha (opacity).
	 * 
	 * \return An SFML Color object
	 */
	sf::Color sfMakeColor(const std::array<uint8_t, 4>& rgba);
}