#pragma once

#include <memory>

#include "menu/composite/MenuNode.hpp"
#include "menu/factory/MenuNodeFactory.hpp"

namespace nemo
{

std::shared_ptr<MenuNode>
createTitleMenu();

}