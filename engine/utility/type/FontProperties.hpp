#pragma once

#include <memory>
#include <string>
#include <boost/assert.hpp>
#include <SFML/Graphics/Font.hpp>

namespace nemo
{

/***
 * @brief Enumeration for horizontal text alignment.
 ***/
enum class Alignment {
	Left,
	Center,
	Right
};

/***
 * @brief Font properties.
 ***/
struct FontProperties
{
	/***
	 * @brief Public attributes.
	 ***/
	std::shared_ptr<sf::Font> family_; ///< Font family.
	unsigned int              size_;   ///< Font size.
	Alignment               align_;  ///< Horizontal text alignment.

	/***
	 * @brief Construct font properties.
	 * 
	 * @param path        - Path to the font family file.
	 * @param size        - Font size.
	 * @param align       - Horizontal text alignment.
	 ***/
	FontProperties(
		const std::string& file, 
		const unsigned int size, 
		const Alignment  align
	) 
		: family_(std::make_shared<sf::Font>())
		, size_  (size)
		, align_ (align)
	{
		BOOST_VERIFY(family_->loadFromFile(file));
	}

	/***
	 * @brief Construct font properties.
	 * 
	 * @param family      - Font family.
	 * @param size        - Font size.
	 * @param align       - Horizontal text alignment.
	 ***/
	FontProperties(
		const std::shared_ptr<sf::Font> family, 
		const unsigned int              size, 
		const Alignment               align
	)
		: family_(family)
		, size_  (size)
		, align_ (align)
	{
	}
};

}