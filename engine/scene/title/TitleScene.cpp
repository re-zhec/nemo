#include "TitleScene.hpp"
#include "menu/composite/item/MenuItem.hpp"
#include "menu/composite/item/MenuItemGraphics.hpp"
#include "menu/composite/menu/Menu.hpp"
#include "menu/composite/menu/MenuGraphics.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

TitleScene::TitleScene()
{
	menu_ = mainMenu();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<Scene>
TitleScene::update(sf::RenderWindow& window)
const
{
	menu_->update(window);
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MenuEntry>
TitleScene::mainMenu()
const
{
	auto font = std::make_shared<sf::Font>();
	BOOST_VERIFY(font->loadFromFile("font/Montserrat-Regular.ttf"));

	auto menu = std::make_shared<Menu>(
		0,
		std::make_shared<MenuGraphics>("", "data/menu/title/menu.json", font)
	);

	return menu;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}