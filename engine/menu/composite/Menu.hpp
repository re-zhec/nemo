#pragma once

#include <string>
#include <vector>

#include "MenuEntry.hpp"
#include "utility/wrapper/sfMakeColor.hpp"
#include "utility/type/RowColumn.hpp"

namespace nemo
{

/***
 * @brief Class for a menu, of which each entry is either a menu item or a 
 * submenu.
 * 
 * A menu with entries will look something similar to this on the render window:
 * 	 __________________________________
 * 	|                                  |
 * 	|             caption              |
 * 	|  ________   ________   ________  |
 * 	| |        | |        | |        | |
 * 	| | Entry1 | | Entry2 | | Entry3 | |
 * 	| |________| |________| |________| |
 * 	|  ________   ________   ________  |
 * 	| |        | |        | |        | |
 * 	| | Entry1 | | Entry2 | | Entry3 | |
 * 	| |________| |________| |________| |
 * 	|__________________________________|
 ***/
class Menu : public MenuEntry
{
public:
	/***
	 * @brief Construct an empty menu.
	 * 
	 * @param pos            - Top left position in the render window.
	 * @param dim            - Overall size, including padding.
	 * @param padding        - Horizontal and vertical padding at the border.
	 * @param spacing        - Horizontal and vertical margins between entries.
	 * @param font           - Default font properties.
	 * @param box_colors     - Default textbox color set for the menu box.
	 * @param entry_colors   - Default textbox color set for each entry.
	 * @param hover_colors  - Default textbox color set for the entry the cursor
	 *                         is currently over.
	 * @param row_by_col           - Number of rows and columns of entries that can be 
	 *                         displayed in the menu at a time.
	 ***/
	Menu(
		const XYPair&         pos,
		const XYPair&         dim,
		const XYPair&         padding,
		const XYPair&         margins,
		const FontProperties& font,
		const TextBoxColors   box_colors,
		const TextBoxColors   entry_colors,
		const TextBoxColors   hover_colors,
		const RCPair&         row_by_col
	);

	/***
	 * @brief Construct an empty menu.
	 * 
	 * @param file        - Path to a JSON file containing menu customizations.
	 ***/
	Menu(const std::string& file);

	/***
	 * @brief Set menu's caption.
	 * 
	 * @param caption     - New caption text.
	 * 
	 * @return The menu itself as a menu entry.
	 ***/
	std::shared_ptr<MenuEntry>
	setCaption(const std::string& caption)
	override;

	/***
	 * @brief Add a menu entry to the menu.
	 * 
	 * @param entry      - Menu entry, either a menu leaf or a submenu.
	 ***/
	void
	addEntry(const std::shared_ptr<Menu> entry);

	/***
	 * @brief Move cursor to the menu entry above the current one.
	 ***/
	void
	cursorUp() 
	noexcept;
	
	/***
	 * @brief Move cursor to the menu entry below the current one.
	 ***/
	void
	cursorDown() 
	noexcept;

	/***
	 * @brief Move cursor to the menu entry left of the current one.
	 ***/
	void
	cursorLeft() 
	noexcept;

	/***
	 * @brief Move cursor to the menu entry right of the current one.
	 ***/
	void
	cursorRight() 
	noexcept;

private:
	/***
	 * @brief Configuration to be extracted from a JSON file for a menu.
	 ***/
	struct ParseInfo
	{
		///< Top left position in the render window.
		XYPair         pos_;
		///< Overall size, including padding.
		XYPair         dim_;
		///< Horizontal and vertical padding at the border.
		XYPair         padding_;
		///< Horizontal and vertical margins between entries.
		XYPair         spacing_;
		///< Default font properties.
		FontProperties font_;
		///< Default textbox color set for the menu box.
		TextBoxColors  box_colors_;
		///< Default textbox color set for each entry.   
		TextBoxColors  entry_colors_;
		///< Default textbox color set for the entry the cursor is currently over.
		TextBoxColors  hover_colors_;
		///< Number of rows and columns of entries that can be displayed at a time.
		RCPair         row_by_col_;    

		/***
		 * @brief Constructor to populate parse fields. Refer to the public 
		 * attributes for parameters.
		 ***/
		ParseInfo(
			const XYPair& pos     = { XValue(-1.f), YValue(-1.f) },
			const XYPair& dim     = { XValue(0.f),  YValue(0.f)  },
			const XYPair& padding = { XValue(-1.f), YValue(-1.f) },
			const XYPair& spacing = { XValue(-1.f), YValue(-1.f) },
			const FontProperties& font = {
				static_cast<std::shared_ptr<sf::Font>>(nullptr), 
				0, 
				Alignment::Left
			},
			const TextBoxColors box_colors    = {
				BorderColor    { sfMakeColor({0,0,0,0}) },
				BackgroundColor{ sfMakeColor({0,0,0,0}) },
				TextColor      { sfMakeColor({0,0,0,0}) }
			},
			const TextBoxColors entry_colors  = {
				BorderColor    { sfMakeColor({0,0,0,0}) },
				BackgroundColor{ sfMakeColor({0,0,0,0}) },
				TextColor      { sfMakeColor({0,0,0,0}) }
			},
			const TextBoxColors hover_colors = {
				BorderColor    { sfMakeColor({0,0,0,0}) },
				BackgroundColor{ sfMakeColor({0,0,0,0}) },
				TextColor      { sfMakeColor({0,0,0,0}) }
			},
			const RCPair& row_by_col = { Row(0), Column(0) }
		)
			: pos_         (pos)
			, dim_         (dim)
			, padding_     (padding)
			, spacing_     (spacing)
			, font_        (font)
			, box_colors_  (box_colors)
			, entry_colors_(entry_colors)
			, hover_colors_(hover_colors)
			, row_by_col_        (row_by_col)
		{
		}
	};

	/***
	 * @brief Parameters for a menu cursor.
	 ***/
	struct Cursor
	{
		TextBoxColors colors_; ///< Textbox color set for hovered entry.
		int idx_; ///< Index of the current entry the cursor is over.

		/***
		 * @brief Constructor to populate cursor fields. Refer to the public 
		 * attributes for parameters.
		 ***/
		Cursor(TextBoxColors colors)
			: colors_(colors)
			, idx_(0)
		{
		}
	};

	/***
	 * @brief Enumeration for which direction the player moves the cursor to in 
	 * the menu.
	 * 
	 * Used by @property cursorMove, which @property cursorUp, @property cursorDown, 
	 * @property cursorLeft, and @property cursorRight are wrappers around.
	 ***/
	enum class Direction {
		Up,
		Down,
		Right,
		Left
	};

	/***
	 * @brief Construct an empty menu using configurations parsed from a JSON 
	 * file.
	 * 
	 * @param info        - Parsed configurations.
	 * 
	 * The public constructor that accepts a path to the JSON file as part of 
	 * its arguments delegates to this constructor.
	 ***/
	Menu(const ParseInfo& info);

	/***
	 * @brief Parse a json file that contains arguments for constructing a menu.
	 * 
	 * @param file        - Path to the JSON file.
	 * 
	 * This method works in tandem with the private constructor to implement the 
	 * the public constructor that takes in a JSON file.
	 * 
	 * @return Parsed configurations.
	 ***/
	ParseInfo
	parse(const std::string& file)
	const;

	/***
	 * @brief Move the cursor along menu options.
	 * 
	 * This method does the main work for @property cursorUp, @property 
	 * cursorDown, @property cursorLeft, and @property cursorRight.
	 * 
	 * @param dir        - Direction to move the cursor to.
	 ***/
	void
	moveCursor(const Direction dir)
	noexcept;

	/***
	 * @brief Private attributes.
	 ***/
	///< Horizontal and vertical margins between entries.
	XYPair spacing_;
	///< Maximum rows of displayable menu entries.
	Row    rows_;
	///< Maximum columns of displayable menu entries.
	Column cols_;
	///< Default textbox color set for each entry.
	TextBoxColors entry_colors_;
	
	Cursor cursor_;  ///< Menu cursor.

	std::vector<std::shared_ptr<MenuEntry>> entries_;  ///< Menu entries.
};

}