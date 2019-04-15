#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "type_safe/strong_typedef.hpp"

#include "utility/type/FontProperties.hpp"
#include "utility/type/Color.hpp"
#include "utility/type/XY.hpp"

namespace nemo
{

/***
 * @brief Abstract class for a menu node, which can be either a menu with 
 * items and submenus or a menu item.
 ***/
class MenuNode : public std::enable_shared_from_this<MenuNode>
{
public:
	/***
	 * @brief Construct a menu node.
	 * 
	 * @param pos        - Top left position in the render window.
	 * @param dim        - Overall size, including padding.
	 * @param padding    - Horizontal and vertical padding at the border.
	 * @param colors     - Default textbox color set.
	 * @param font       - Default font properties.
	 * 
	 * The menu node is displayed on screen as a colored textbox. By default, 
	 * there is no caption. It can be added via @property setCaption. It is 
	 * handled in the method call instead in construction because the caption's 
	 * vertical alignment is different between menu items and menus.
	 ***/
	MenuNode(
		const XYPair&         pos,
		const XYPair&         dim,
		const XYPair&         padding,
		const TextBoxColors   colors,
		const FontProperties& font);

	/***
	 * @brief Add a child to the menu node.
	 * 
	 * @
	 ***/
	virtual std::shared_ptr<MenuNode>
	add(std::shared_ptr<MenuNode> child) = 0;

	/***
	 * @brief
	 ***/
	virtual void
	drawIt(sf::RenderWindow& window)
	const = 0;

	/***
	 * @brief Set menu node's caption.
	 * 
	 * @param caption    - New caption text.
	 * 
	 * @return The menu node itself.
	 ***/
	virtual std::shared_ptr<MenuNode>
	setCaption(const std::string& caption) = 0;

	/***
	 * @brief Set the border, background, and text colors of the menu node.
	 * 
	 * @param colors     - New color set.
	 * 
	 * @return The menu node itself.
	 ***/ 
	std::shared_ptr<MenuNode>
	setColors(const TextBoxColors colors);

	/***
	 * @brief Move the menu node to a new position in the render window.
	 * 
	 * @param pos        - New top left position.
	 * 
	 * @return The menu node itself.
	 ***/ 
	std::shared_ptr<MenuNode>
	setPosition(const XYPair& pos);

	/***
	 * @brief Get the starting position of the menu node in the render window.
	 * 
	 * @return Top left position of the menu node.
	 ***/
	XYPair
	getPosition()
	const noexcept;

	/***
	 * @brief Change the menu node's overall size.
	 * 
	 * @param dim        - New overall width and length.
	 * 
	 * @return The menu node itself.
	 ***/ 
	std::shared_ptr<MenuNode>
	setSize(const XYPair& dim);

	/***
	 * @brief Get the size of the menu node reserved for content.
	 * 
	 * @return Width and length of the menu node, excluding padding.
	 ***/ 
	XYPair
	getInnerSize()
	const noexcept;

protected:
	/***
	 * @brief Set menu node's caption.
	 * 
	 * @param caption    - New caption text.
	 * @param vt_center  - Whether to vertically align text to center.
	 * 
	 * The publicly accessible @property setCaption wraps around this method 
	 * since the difference between the public methods in the MenuLeaf and MenuTree 
	 * classes is the vertical text alignment.
	 ***/
	std::shared_ptr<MenuNode>
	makeCaption(const std::string& caption, bool vt_center);

	/***
	 * @brief Draw the menu node on the render window.
	 * 
	 * @param window     - Render window.
	 ***/
	void
	drawTextBox(sf::RenderWindow& window)
	const;

private:
	/***
	 * @brief Member attributes
	 ***/
	sf::RectangleShape cell_;    ///< Cell containing the menu node.
	sf::RectangleShape space_;   ///< Space reserved for menu node content.
	sf::Text           caption_; ///< Caption text.
	FontProperties     font_;    ///< Default Font properties.
	TextBoxColors      colors_;  ///< Default textbox color set.
};

}