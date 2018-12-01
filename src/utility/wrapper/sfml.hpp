#pragma once

#include <SFML/System/Vector2.hpp>
#include "utility/type/defs.hpp"

namespace sb
{
	sf::Vector2f sfVector2(const XValue x, const YValue y)
	{
		return { x.v_, y.v_ };
	}
}