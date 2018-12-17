#pragma once

#include "../MenuEntryGraphics.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
/// @brief Graphics component of a menu entry.
////////////////////////////////////////////////////////////////////////////////
class MenuItemGraphics : public MenuEntryGraphics
{
public:

	using MenuEntryGraphics::MenuEntryGraphics;

	/////////////////////////////////////////////////////
	/// @brief Construct the graphics for a menu item using a JSON file 
	/// containing configurations for the item.
	///
	/// @param text         Text to display.
	/// @param file         Path to the JSON file.
	/// @param pos          Top left position of the menu item.
	/// @param dim          Overall size.
	/// @param font         Font family.
	/////////////////////////////////////////////////////
	MenuItemGraphics(
		const std::string&              text,
		const std::string&              file,
		const XYPair&                   pos,
		const XYPair&                   dim,
		const std::shared_ptr<sf::Font> font
	);

	/////////////////////////////////////////////////////
	/// @brief Draw the graphic on the render window.
	/// 
	/// @param menu         Menu entry (item) associated with this graphics 
	///                     component.
	/// @param window       Render window.
	/////////////////////////////////////////////////////
	virtual void 
	update(MenuEntry& entry, sf::RenderWindow& window) 
	override;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
private:

	/////////////////////////////////////////////////////
	/// @brief Configuration to be extracted from a JSON file for a menu item.
	/////////////////////////////////////////////////////
	struct ParseInfo
	{
		XYPair        margins_; ///< Margins around a menu item's text.
		TextBoxColors colors_;  ///< Border, background, and text color.
		int           font_sz_; ///< Font size.
		AlignConfig   align_;   ///< Text alignment configuration

		/////////////////////////////////////////////////////
		/// @brief Constructor to populate parse fields. Refer to the public 
		/// attributes for parameters.
		/////////////////////////////////////////////////////
		ParseInfo(
			const XYPair&       margins = {XValue(-1.f), YValue(-1.f)},
			const TextBoxColors colors  = {{0,0,0},{0,0,0},{0,0,0}},
			const int           font_sz = 0,
			const AlignConfig   align   = {HzAlign::Center, VtAlign::Center}
		);
	};

	/////////////////////////////////////////////////////
	/// @brief Construct the graphics for a menu entry using configurations 
	/// parsed from the JSON file.
	///
	/// @param text         Text to display.
	/// @param info         Parsed configurations.
	/// @param pos          Top left position of the menu item.
	/// @param dim          Overall size.
	/// @param font         Font family.
	///
	/// The public constructor that accepts a path to the JSON file as part of 
	/// its arguments calls this as a delegate constructor.
	/////////////////////////////////////////////////////
	MenuItemGraphics(
		const std::string&              text,
		const ParseInfo&                info,
		const XYPair&                   pos,
		const XYPair&                   dim,
		const std::shared_ptr<sf::Font> font
	);

	/////////////////////////////////////////////////////
	/// @brief Parse a JSON file containing configurations for a menu item.
	///
	/// @param file         Path to the JSON file.
	/// 
	/// The public constructor that accepts a path to the JSON file as part of 
	/// its arguments calls this during its initialization.
	/////////////////////////////////////////////////////
	ParseInfo
	parse(const std::string& file)
	const;
};

}