#pragma once

#include <SFML/Window/Keyboard.hpp>

#include "type_safe/strong_typedef.hpp"

namespace sb
{

struct Key 
	: type_safe::strong_typedef<Key, sf::Keyboard::Key>
	, type_safe::strong_typedef_op::equality_comparison<Key>
	, type_safe::strong_typedef_op::relational_comparison<Key>
{
	using strong_typedef::strong_typedef;
};

}