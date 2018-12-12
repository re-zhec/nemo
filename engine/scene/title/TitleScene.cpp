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
	menu_->update(window, nemo::KeyAction::Cancel);
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
		std::make_shared<MenuGraphics>("data/menu/title/menu.json", font)
	);

	auto settings = std::make_shared<MenuItem>(
		1,
		std::make_shared<MenuItemGraphics>(
			"data/menu/title/item.json", 
			XYPair(XValue(20.f), YValue(20.f)),
			XYPair(XValue(300.f), YValue(100.f)),
			font
		)
	);

	menu->add(settings);
	return menu;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}