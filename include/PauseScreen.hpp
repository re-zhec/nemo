#pragma once
#include <SFML/Graphics.hpp>
#include "../include/Inventory.hpp"
#include "../include/Menu.hpp"

namespace rp
{

class PauseScreen
{
public:

	PauseScreen(std::shared_ptr<Inventory> inv);
	void Update(sf::RenderWindow& window, sf::Event& event);

private:
	std::shared_ptr<Inventory> m_inv;
	Menu<ItemID> m_menu;
};

}