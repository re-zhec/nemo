#pragma once

#include <SFML/Graphics/Color.hpp>

namespace nemo
{

struct BorderColor {
	sf::Color v_;
};

struct BackgroundColor {
	sf::Color v_;
};

struct TextColor {
	sf::Color v_;
};

struct TextBoxColors
{
	BorderColor     border_;
	BackgroundColor backgnd_;
	TextColor       text_;

	TextBoxColors(const BorderColor border, const BackgroundColor backgnd, 
		const TextColor text)
		: border_ (border)
		, backgnd_(backgnd)
		, text_   (text)
	{
	}
};

}