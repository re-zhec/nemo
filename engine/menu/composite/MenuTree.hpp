#pragma once

#include <string>
#include <vector>

#include "MenuNode.hpp"
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
class MenuTree : public MenuNode
{
public:
	/***
	 * @brief Construct an empty menu.
	 * 
	 * @param pos            - Top left position in the render window.
	 * @param dim            - Overall size, including padding.
	 * @param row_by_col     - Number of rows and columns of entries that can be 
	 *                         displayed in the menu at a time.
	 * @param padding        - Horizontal and vertical padding at the border.
	 * @param spacing        - Horizontal and vertical margins between entries.
	 * @param box_colors     - Default textbox color set for the menu box.
	 * @param entry_colors   - Default textbox color set for each entry.
	 * @param hover_colors   - Default textbox color set for the entry the cursor
	 *                         is currently over.
	 * @param font           - Default font properties.

	 ***/
	MenuTree(
		const XYPair&         pos,
		const XYPair&         dim,
		const RCPair&         row_by_col,
		const XYPair&         padding,
		const XYPair&         spacing,
		const TextBoxColors   box_colors,
		const TextBoxColors   entry_colors,
		const TextBoxColors   hover_colors,
		const FontProperties& font);

	/***
	 * @brief Construct an empty menu.
	 * 
	 * @param file        - Path to a JSON file containing menu customizations.
	 ***/
	MenuTree(const std::string& file);

	/***
	 * @brief
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
	 * @brief Set menu's caption.
	 * 
	 * @param caption     - New caption text.
	 * 
	 * @return The menu itself as a menu entry.
	 ***/
	std::shared_ptr<MenuNode>
	setCaption(const std::string& caption)
	override;

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
	XYPair spacing_; ///< Horizontal and vertical margins between entries.
	Row    rows_; ///< Maximum rows of displayable menu entries.
	Column cols_; ///< Maximum columns of displayable menu entries.
	TextBoxColors entry_colors_; ///< Default textbox color set for each entry.
	Cursor cursor_;  ///< MenuTree cursor.
	std::vector<std::shared_ptr<MenuNode>> children_;  ///< MenuTree children nodes.
};

}