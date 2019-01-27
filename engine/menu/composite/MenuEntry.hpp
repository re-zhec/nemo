#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "utility/type/FontProperties.hpp"
#include "utility/type/Color.hpp"
#include "utility/type/XY.hpp"
#include "type_safe/strong_typedef.hpp"

namespace nemo
{

/***
 * @brief Abstract class for a menu entry, which can be either a menu with 
 * entries or a menu item.
 ***/
class MenuEntry : public std::enable_shared_from_this<MenuEntry>
{
public:
	/***
	 * @brief Construct a menu entry.
	 * 
	 * @param pos        - Top left position in the render window.
	 * @param dim        - Overall size, including padding.
	 * @param padding    - Horizontal and vertical padding at the border.
	 * @param colors     - Default textbox color set.
	 * @param font       - Default font properties.
	 * 
	 * The menu entry is displayed on screen as a colored textbox. By default, 
	 * there is no caption. It can be added via @property setCaption. It is done
	 * like this because the caption's vertical alignment is different between 
	 * menu items and menus.
	 ***/
	MenuEntry(
		const XYPair&         pos,
		const XYPair&         dim,
		const XYPair&         padding,
		const TextBoxColors   colors,
		const FontProperties& font
	);

	/***
	 * @brief Set menu entry's caption.
	 * 
	 * @param caption    - New caption text.
	 * 
	 * @return The menu entry itself.
	 ***/
	virtual 
	std::shared_ptr<MenuEntry>
	setCaption(const std::string& caption) = 0;

	/***
	 * @brief Set the border, background, and text colors of the menu entry.
	 * 
	 * @param colors     - New color set.
	 * 
	 * @return The menu entry itself.
	 ***/ 
	std::shared_ptr<MenuEntry>
	setColors(const TextBoxColors colors);

	/***
	 * @brief Move the menu entry to a new position in the render window.
	 * 
	 * @param pos        - New top left position.
	 * 
	 * @return The menu entry itself.
	 ***/ 
	std::shared_ptr<MenuEntry>
	setPosition(const XYPair& pos);

	/***
	 * @brief Get the starting position of the menu entry in the render window.
	 * 
	 * @return Top left position of the menu entry.
	 ***/
	XYPair
	getPosition()
	const noexcept;

	/***
	 * @brief Change the menu entry's overall size.
	 * 
	 * @param dim        - New overall width and length.
	 * 
	 * @return The menu entry itself.
	 ***/ 
	std::shared_ptr<MenuEntry>
	setSize(const XYPair& dim);

	/***
	 * @brief Get the size of the menu entry reserved for content.
	 * 
	 * @return Width and length of the menu entry, excluding padding.
	 ***/ 
	XYPair
	getInnerSize()
	const noexcept;

	/***
	 * @brief Draw the menu entry on the render window.
	 * 
	 * @param window     - Render window.
	 ***/
	void
	drawOn(sf::RenderWindow& window)
	const;

protected:
	/***
	 * @brief Set menu entry's caption.
	 * 
	 * @param caption    - New caption text.
	 * @param vt_center  - Whether to vertically align text to center.
	 * 
	 * Public-accessible @property setCaption should wrap around this method 
	 * since the difference between the public methods in MenuItem and Menu is 
	 * the vertical text alignment.
	 ***/
	std::shared_ptr<MenuEntry>
	makeCaption(const std::string& caption, bool vt_center);

private:
	/***
	 * @brief Member attributes
	 ***/
	sf::RectangleShape cell_;    ///< Cell containing the menu entry.
	sf::RectangleShape space_;   ///< Space reserved for menu entry content.
	sf::Text           caption_; ///< Caption text.
	FontProperties     font_;    ///< Default Font properties.
	TextBoxColors      colors_;  ///< Default textbox color set.
};

}