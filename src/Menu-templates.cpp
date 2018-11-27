/**
 * \brief Menu is templatized but not defined in its header file Menu.hpp. This 
 * source file is for explicit instantiation declarations
 */

#include "Menu.cpp"

// Include headers containing scoped enum types here
#include "../include/StartScreen.hpp"
#include "../include/PauseScreen.hpp"
#include "../include/Item.hpp"

namespace rp
{

template class Menu<StartScreen::MainMenuKey>;
template class Menu<PauseScreen::MainMenuKey>;
template class Menu<ItemID>;

}