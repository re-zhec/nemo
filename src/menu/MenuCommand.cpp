#include <memory>
#include <SFML/Window.hpp>

#include "Menu.hpp"
#include "MenuCommand.hpp"

namespace sb
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
MenuUpCommand::execute(Menu& menu)
{
	menu.moveUp();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
MenuDownCommand::execute(Menu& menu)
{
	menu.moveDown();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
MenuLeftCommand::execute(Menu& menu)
{
	menu.moveLeft();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
MenuRightCommand::execute(Menu& menu)
{
	menu.moveRight();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}