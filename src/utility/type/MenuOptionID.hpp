#pragma once

#include "type_safe/strong_typedef.hpp"

namespace sb
{

struct MenuOptionID 
	: type_safe::strong_typedef<MenuOptionID, int>
	, type_safe::strong_typedef_op::equality_comparison<MenuOptionID>
{
	using strong_typedef::strong_typedef;
};

}