#pragma once

#include <optional>

#include "utility/type/Key.hpp"
#include "Menu.hpp"

namespace nemo
{

class MenuHandler
{
public:
	std::optional<int>
	handleAction(Menu& menu, const KeyAction action);
};

}