#pragma once

#include <SFML/System/Vector2.hpp>

#include "utility/type/XYValue.hpp"

namespace sb
{
	sf::Vector2f sfVector2(const XValue x, const YValue y);
	sf::Vector2f sfVector2(const XYValue xy);
}