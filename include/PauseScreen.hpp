#pragma once
#include <SFML/Graphics.hpp>
#include "../include/Inventory.hpp"
#include "../include/Menu.hpp"

namespace rp
{

class PauseScreen
{
public:
	enum class MainMenuKey {
		Party,
		Items,
		Gears,
		Map,
		Logs,
		Default
	};

	PauseScreen(std::shared_ptr<Inventory> inv);
	void Update(sf::RenderWindow& window, sf::Event& event);

private:
	std::shared_ptr<Inventory> m_inv;
	Menu<MainMenuKey> m_main_menu;
	Menu<int> m_item_menu;
};

}