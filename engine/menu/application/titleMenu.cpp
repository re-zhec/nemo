#include "titleMenu.hpp"

namespace nemo
{

std::shared_ptr<MenuNode>
createTitleMenu()
{
	MenuNodeFactory factory;
	factory.setDefaultConfig("data/menu/title.json");

	std::shared_ptr<MenuNode> menu = factory.create(MenuNodeType::Tree);
	menu->setCaption("hello");

	std::shared_ptr<MenuNode> new_game = factory.create(MenuNodeType::Leaf);
	new_game->setCaption("New Game");
	
	std::shared_ptr<MenuNode> cont_game = factory.create(MenuNodeType::Leaf);
	cont_game->setCaption("Continue Game");

	menu
		->add(new_game)
		->add(cont_game);
	
	return menu;
}

}