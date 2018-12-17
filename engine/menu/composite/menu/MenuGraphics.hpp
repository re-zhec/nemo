#pragma once

#include "../MenuEntryGraphics.hpp"
#include "utility/type/RowColumn.hpp"
#include "utility/RC1DConverter.hpp"

namespace nemo
{

class MenuEntry; // Forward declaration.

////////////////////////////////////////////////////////////////////////////////
/// @brief Graphics component of a menu.
////////////////////////////////////////////////////////////////////////////////
class MenuGraphics : public MenuEntryGraphics
{
public:

	/////////////////////////////////////////////////////
	/// @brief Construct the graphics of a menu.
	///
	/// @param title        Menu title.
	/// @param pos          Top left position of the menu.
	/// @param dim          Overall size, including the margins.
	/// @param margins      Horizontal and vertical margins at the border.
	/// @param colors       Default color set outside of the entries.
	/// @param font         Title's font family.
	/// @param font_sz      Title's font size.
	/// @param align        Horizontal text alignment configuration.
	/// @param rows         Maximum rows of menu entries per page.
	/// @param cols         Maximum columns of menu entries per page.
	/////////////////////////////////////////////////////
	MenuGraphics(
		const std::string&              title,
		const XYPair&                   pos,
		const XYPair&                   dim,
		const XYPair&                   margins,
		const TextBoxColors             colors,
		const std::shared_ptr<sf::Font> font,
		const int                       font_sz,
		const AlignConfig               align,
		const Row&                      rows,
		const Column&                   cols
	);

	/////////////////////////////////////////////////////
	/// @brief Construct the graphics for a menu using a JSON file containing 
	/// configurations.
	///
	/// @param title        Menu title.
	/// @param file         Path to the JSON file.
	/// @param font         Font family.
	/////////////////////////////////////////////////////
	MenuGraphics(
		const std::string&              title,
		const std::string&              file,
		const std::shared_ptr<sf::Font> font
	);

	/////////////////////////////////////////////////////
	/// @brief Draw the graphic on the render window.
	/// 
	/// @param menu         Menu entry (menu) associated with this graphics 
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
	/// @brief Member attributes.
	/////////////////////////////////////////////////////
	Row    rows_;
	Column cols_;

	/////////////////////////////////////////////////////
	/// @brief Configuration to be extracted from a JSON file for a menu.
	///
	/// Refer to the parameters of the top constructor, except for @title and 
	/// @font, for the struct's member variables.
	/////////////////////////////////////////////////////
	struct ParseInfo
	{
		XYPair        pos_;
		XYPair        dim_;
		XYPair        margins_;
		TextBoxColors colors_;
		int           font_sz_;
		AlignConfig   align_;
		Row           rows_;
		Column        cols_;

		/////////////////////////////////////////////////////
		/// @brief Constructor to populate parse fields. Refer to the public 
		/// attributes for parameters.
		/////////////////////////////////////////////////////
		ParseInfo(
			const XYPair&       pos     = {XValue(-1.f), YValue(-1.f)},
			const XYPair&       dim     = {XValue(0.f), YValue(0.f)},
			const XYPair&       margins = {XValue(-1.f), YValue(-1.f)},
			const TextBoxColors colors  = {{0,0,0},{0,0,0},{0,0,0}},
			const int           font_sz = 0,
			const AlignConfig   align   = {HzAlign::Center, VtAlign::Top},
			const Row&          rows    = Row(0),
			const Column&       cols    = Column(0)
		);
	};

	/////////////////////////////////////////////////////
	/// @brief Construct the graphics for a menu entry using configurations 
	/// parsed from the JSON file.
	///
	/// @param title        Menu title.
	/// @param info         Parsed configurations.
	/// @param font         Font family.
	///
	/// The public constructor that accepts a path to the JSON file as part of 
	/// its arguments calls this as a delegate constructor.
	/////////////////////////////////////////////////////
	MenuGraphics(
		const std::string&              title,
		const ParseInfo&                info,
		const std::shared_ptr<sf::Font> font
	);

	ParseInfo
	parse(const std::string& file)
	const;
};

}