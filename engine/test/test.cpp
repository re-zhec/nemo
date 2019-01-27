#include "test.hpp"

namespace nemo
{

void test(sf::RenderWindow& window)
{
	std::unique_ptr<Menu> menu = std::make_unique<Menu>("data/menu/inventory.json");
	menu->drawOn(window);
}

}