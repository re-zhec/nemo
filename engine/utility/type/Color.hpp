#pragma once

#include <SFML/Graphics/Color.hpp>

namespace nemo
{

struct TextBoxColors
{
	sf::Color text_;
	sf::Color backgnd_;
	sf::Color border_;

	TextBoxColors(const sf::Color txt, const sf::Color backgnd, 
		const sf::Color border)
		: text_    (txt)
		, backgnd_(backgnd)
		, border_ (border)
	{
	}
};

}