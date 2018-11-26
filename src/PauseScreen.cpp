#include "../include/PauseScreen.hpp"
#include "../include/Menu.hpp"
#include "../include/Item.hpp"
#include "../include/logger.hpp"

namespace rp
{

PauseScreen::PauseScreen(std::shared_ptr<Inventory> inv)
	: m_inv(std::move(inv))
	, m_menu("xml/menu/inventory.xml")
{
	for (auto entry : m_inv->peek()) {
		const auto& [id, name, quantity] = entry;
		m_menu.add(id, name + " x " + std::to_string(quantity));
	}
}

void 
PauseScreen::Update(sf::RenderWindow& window, sf::Event& event)
{
	switch (event.type) {
		case sf::Event::KeyPressed:
			switch (event.key.code) {
				case sf::Keyboard::W:
					m_menu.moveUp();
					break;
				case sf::Keyboard::S:
					m_menu.moveDown();
					break;
				case sf::Keyboard::A:
					m_menu.moveLeft();
					break;
				case sf::Keyboard::D:
					m_menu.moveRight();
					break;
				case sf::Keyboard::Enter:
					if (const auto id = m_menu.cursorAt()) {
						if (const auto result = m_inv->remove(*id, 0)) {
							if (const auto [item, n_remain] = *result;
								n_remain > 0)
							{
								m_menu.setOptionText(
									*id, 
									item->Name() + " x " + std::to_string(n_remain)
								);
							}
							else {
								m_menu.remove(*id);
							}
						}
					}
					break;
				default:
					break;
			}
			break;

		default:
			break;
	}

	m_menu.draw(window);
}

}