#pragma once

#include <SFML/Window/Keyboard.hpp>

#include "StrongType.hpp"
#include "ArithmeticType.hpp"

namespace sb
{
	using Key = StrongType<sf::Keyboard::Key, struct KeyTag>;
	using XValue = ArithmeticType<float, struct XValueTag>;
	using YValue = ArithmeticType<float, struct YValueTag>;

	using Row = ArithmeticType<size_t, struct RowTag>;
	using Column = ArithmeticType<size_t, struct ColumnTag>;

	struct RowColumn
	{
		Row r_;
		Column c_;
	};
}