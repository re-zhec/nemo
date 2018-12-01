#pragma once

#include <SFML/Graphics.hpp>

#include "GameState.hpp"
#include "item/Inventory.hpp"
#include "menu/Menu.hpp"

namespace rp
{

class PauseState : public GameState
{
public:
	PauseState(std::shared_ptr<Inventory> inv);
	virtual void handleEvent(const sf::Event& event) & override;
	virtual void update(sf::RenderWindow& window) & override;

private:
	enum MainMenuKey {
		Party,
		Items,
		Gears,
		Map,
		Logs,
		Default
	};

	std::shared_ptr<Inventory> m_inv;
	Menu m_main_menu;
	Menu m_item_menu;
};

}