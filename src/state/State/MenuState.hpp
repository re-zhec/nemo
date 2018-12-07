#pragma once

#include "../State.hpp"
#include "menu/Menu.hpp"
#include "menu/MenuHandler.hpp"

namespace nemo
{

class MenuState : public State
{
public:
	MenuState(const Menu&& menu);
	
protected:
	Menu menu_;
	MenuHandler menu_handler_;
};

}