#include <SFML/Window.hpp>
#include "../include/Menu.hpp"
#include "../include/MenuCommand.hpp"

namespace rp
{

void MenuUpCommand::execute(Menu& menu)
{
	menu.moveUp();
}

void MenuDownCommand::execute(Menu& menu)
{
	menu.moveDown();
}

void MenuLeftCommand::execute(Menu& menu)
{
	menu.moveLeft();
}

void MenuRightCommand::execute(Menu& menu)
{
	menu.moveRight();
}

}