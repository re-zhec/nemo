#pragma once

#include <string>
#include <vector>

#include "MenuEntry.hpp"
#include "utility/type/RowColumn.hpp"

namespace nemo
{

/***
 * @brief Class for a menu item.
 ***/
class MenuItem : public MenuEntry
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
	using MenuEntry::MenuEntry;

	/***
	 * @brief Set menu item's caption.
	 * 
	 * @param caption     - New caption text.
	 * 
	 * @return The menu item itself as a menu entry.
	 ***/
	std::shared_ptr<MenuEntry>
	setCaption(const std::string& caption)
	override;

private:
};

}