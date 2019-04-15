#pragma once

#include "MenuNode.hpp"

namespace nemo
{

/***
 * @brief Class for a menu item.
 ***/
class MenuLeaf : public MenuNode
{
public:
	/***
	 * @brief Construct an empty menu item.
	 * 
	 * @param pos         - Top left position in the render window.
	 * @param dim         - Overall size, including padding.
	 * @param padding     - Horizontal and vertical padding at the border.
	 * @param colors      - Default textbox color set.
	 * @param font        - Default font properties.
	 ***/
	using MenuNode::MenuNode;

	/***
	 * @brief Add a child to the menu node.
	 * 
	 * @
	 ***/
	std::shared_ptr<MenuNode>
	add(std::shared_ptr<MenuNode> child)
	override;

	/***
	 * @brief
	 ***/
	void
	drawIt(sf::RenderWindow& window)
	const override;

	/***
	 * @brief Set menu item's caption.
	 * 
	 * @param caption     - New caption text.
	 * 
	 * @return The menu item itself as a menu entry.
	 ***/
	std::shared_ptr<MenuNode>
	setCaption(const std::string& caption)
	override;

private:
};

}