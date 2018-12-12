#pragma once

#include <SFML/System/Vector2.hpp>

#include "utility/type/XY.hpp"

namespace nemo
{
	sf::Vector2f sfVector2(const XValue x, const YValue y);
	sf::Vector2f sfVector2(const XYPair xy);
}