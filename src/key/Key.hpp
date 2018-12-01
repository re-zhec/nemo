#pragma once

#include <SFML/Window/Keyboard.hpp>
#include "utility/StrongType.hpp"

namespace rp
{

enum class KeyAction {
	Up = 0,
	Down,
	Left,
	Right,
	Select,
	Cancel,
	Pause,
	None,

	count
};

using Key = StrongType<sf::Keyboard::Key, struct KeyParam>;

}