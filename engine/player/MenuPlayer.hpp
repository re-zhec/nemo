#pragma once

#include <memory>
#include <optional>

#include "menu/composite/MenuNode.hpp"
#include "menu/factory/MenuNodeFactory.hpp"
#include "utility/type/Key.hpp"

namespace nemo
{

class MenuPlayer
{
public:
	/***
	 * @brief Constructs the menu portion of the engine.
	 ***/
	MenuPlayer();

	/***
	 * @brief Indicates whether a menu is currently opened on-screen.
	 * 
	 * @return True if yes, false otherwise.
	 ***/
	bool
	menuIsOpened()
	const noexcept;

	/***
	 * @brief Updates the state of the game and currently opened menu upon 
	 * player input. Changes are reflected on the render window.
	 * 
	 * @param key         - Player input.
	 * @param window      - Render window.
	 ***/
	void
	update(
		const std::optional<KeyAction> key, 
		sf::RenderWindow& window);

private:
	bool active_; ///< This not only indicates whethr a menu is currently being 
	// accessed, but it also dictates the state of menu operations. True means 
	// that a menu is currently being accessed and that any player input will 
	// affect solely the menu. False means otherwise.

	std::shared_ptr<MenuNode> current_entry_; ///< Current menu entry (menu or 
	// an item from the menu) shown to the player.
	
	MenuNodeFactory factory_; ///< Factory to facilitate menu or menu item 
	// creation.
};

}