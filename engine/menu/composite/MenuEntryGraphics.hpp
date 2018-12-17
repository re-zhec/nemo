#pragma once

#include <memory>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include "utility/type/XY.hpp"
#include "utility/type/Color.hpp"
#include "utility/type/Align.hpp"
#include "MenuEntry.hpp"

namespace nemo
{

class MenuEntry; // Forward declaration.

////////////////////////////////////////////////////////////////////////////////
/// @brief Abstract class for the graphics component of a menu entry.
////////////////////////////////////////////////////////////////////////////////
class MenuEntryGraphics
{
public:
	
	virtual
	~MenuEntryGraphics() {}

	/////////////////////////////////////////////////////
	/// @brief Get the size of the menu.
	///
	/// @return The width and length of the menu.
	/////////////////////////////////////////////////////
	XYPair
	size()
	const;

	/////////////////////////////////////////////////////
	/// @brief Draw the graphic on the render window.
	/// 
	/// @param window       Render window.
	/////////////////////////////////////////////////////
	void
	drawOn(sf::RenderWindow& window);

	/////////////////////////////////////////////////////
	/// @brief Draw the graphic on the render window.
	/// 
	/// @param menu         Menu entry associated with this graphics component.
	/// @param window       Render window.
	///
	/// To be overriden.
	/////////////////////////////////////////////////////
	virtual void
	update(MenuEntry& entry, sf::RenderWindow& window) = 0;


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
protected:

	/////////////////////////////////////////////////////
	/// @brief Construct the graphics of a menu entry.
	///
	/// @param text         Text to display.
	/// @param pos          Top left position of the entry.
	/// @param dim          Overall size, including margins.
	/// @param margins      Margins around the entry's text.
	/// @param colors       Default color set.
	/// @param font         Font family.
	/// @param font_sz      Font size.
	/// @param align        Text alignment configuration.
	/////////////////////////////////////////////////////
	MenuEntryGraphics(
		const std::string&              text,
		const XYPair&                   pos,
		const XYPair&                   dim,
		const XYPair&                   margins,
		const TextBoxColors             colors,
		const std::shared_ptr<sf::Font> font,
		const int                       font_sz,
		const AlignConfig               align
	);


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
private:

	/////////////////////////////////////////////////////
	/// @brief Member attributes
	/////////////////////////////////////////////////////
	std::shared_ptr<sf::Font> font_;   ///< Fomt family.
	sf::RectangleShape        cell_;   ///< Menu entry cell.
	sf::Text                  text_;   ///< Menu entry text.
	TextBoxColors             colors_; ///< Default color set.
};

}