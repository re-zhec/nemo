#pragma once

#include <SFML/Graphics/Color.hpp>

namespace nemo
{

struct TextBoxColor
{
	sf::Color txt_;
	sf::Color backgnd_;
	sf::Color border_;

	TextBoxColor(const sf::Color txt, const sf::Color backgnd, 
		const sf::Color border)
		: txt_    (txt)
		, backgnd_(backgnd)
		, border_ (border)
	{
	}
};

}