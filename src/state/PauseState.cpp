#include <SFML/Graphics.hpp>

#include "GameState.hpp"
#include "PauseState.hpp"
#include "item/Inventory.hpp"
#include "menu/Menu.hpp"

namespace sb
{

PauseState::PauseState(std::shared_ptr<Inventory> inv)
	: m_inv(std::move(inv))
	, m_main_menu("json/menus/pause.json")
	, m_item_menu("json/menus/inventory.json")
{
	m_main_menu
		.add(MainMenuKey::Party, "Party")
		.add(MainMenuKey::Items, "Items")
		.add(MainMenuKey::Gears, "Gears")
		.add(MainMenuKey::Map, "Map")
		.add(MainMenuKey::Logs, "Logs");

	for (auto entry : m_inv->peek()) {
		const auto& [id, name, quantity] = entry;
		m_item_menu.add(id, name + " x " + std::to_string(quantity));
	}
}

void 
PauseState::handleEvent(const sf::Event& event) &
{
	switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
				case sf::Keyboard::W:
					m_main_menu.moveUp();
					break;
				case sf::Keyboard::S:
					m_main_menu.moveDown();
					break;
				case sf::Keyboard::A:
					m_main_menu.moveLeft();
					break;
				case sf::Keyboard::D:
					m_main_menu.moveRight();
					break;
				case sf::Keyboard::Enter:
					break;
				default:
					break;
			}
			break;

		default:
			break;
	}
}

void 
PauseState::update(sf::RenderWindow& window) &
{
	window.clear(sf::Color::White);
	m_main_menu.draw(window);

	switch (const auto which_menu = m_main_menu.cursorAt();
		which_menu.value_or(MainMenuKey::Default))
	{
		case MainMenuKey::Party:
			break;
		case MainMenuKey::Items:
			m_item_menu.draw(window);
			break;
		case MainMenuKey::Gears:
			break;
		case MainMenuKey::Map:
			break;
		case MainMenuKey::Logs:
			break;
		default:
			break;
	}
}

}