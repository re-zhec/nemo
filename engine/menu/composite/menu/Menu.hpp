#pragma once

#include <vector>

#include "../MenuEntry.hpp"
#include "utility/RC1DConverter.hpp"

namespace nemo
{

class MenuGraphics;     // Forward declaration.
class MenuItem;         // Forward declaration.
class MenuItemGraphics; // Forward declaration.

////////////////////////////////////////////////////////////////////////////////
///
/// @brief Menu, of which each menu entry is either a menu item or another menu.
///
/// This derived class corresponds to the composite part of the composite design 
/// pattern for a menu of any depth. 
///
////////////////////////////////////////////////////////////////////////////////
class Menu : public MenuEntry
{
	/////////////////////////////////////////////////////
	/// @brief Enumeration for which direction the player moves the cursor to in 
	/// the menu.
	/// 
	/// Used by @property move, which @property cursorUp, @property cursorDown, 
	/// @property cursorLeft, and @property cursorRight are wrappers around.
	///////////////////////////////////////////////////// 
	enum class Direction {
		Up,
		Down,
		Right,
		Left
	};

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
public:
	using MenuEntry::MenuEntry;

	/////////////////////////////////////////////////////
	/// @brief Add a menu entry to the menu.
	///
	/// @param id           ID for the menu entry.
	/// @param item         Menu entry.
	///
	/// The menu entry can be a menu item or another menu. The client must 
	/// specify a unique ID for every menu entry added. Two menus can use the 
	/// same ID, however.
	/////////////////////////////////////////////////////
	void
	add(const MenuEntryID id, const std::shared_ptr<MenuEntry> entry);

	/////////////////////////////////////////////////////
	/// @brief Remove an entry from the menu.
	///
	/// This method finds the option based on its ID. If no entries in the menu 
	/// has @a id, this method generates an assertion error.
	///
	/// @param id           ID of the menu entry to remove.
	/////////////////////////////////////////////////////
	void
	remove(const MenuEntryID id);

	/////////////////////////////////////////////////////
	/// @brief Move cursor to the menu entry above the current one.
	/////////////////////////////////////////////////////
	void
	cursorUp() 
	noexcept;
	
	/////////////////////////////////////////////////////
	/// @brief Move cursor to the menu entry below the current one.
	/////////////////////////////////////////////////////
	void
	cursorDown() 
	noexcept;

	/////////////////////////////////////////////////////
	/// @brief Move cursor to the menu entry left of the current one.
	/////////////////////////////////////////////////////
	void
	cursorLeft() 
	noexcept;

	/////////////////////////////////////////////////////
	/// @brief Move cursor to the menu entry right of the current one.
	/////////////////////////////////////////////////////
	void
	cursorRight() 
	noexcept;

	/////////////////////////////////////////////////////
	/// @brief Get the menu entries that are children of this one.
	///
	/// @return Child entries.
	/////////////////////////////////////////////////////
	std::vector<std::shared_ptr<MenuEntry>>
	children()
	const;

	virtual void
	accept(MenuCursor& cursor)
	override;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
private:

	///< Container for all menu entry children
	std::vector<std::shared_ptr<MenuEntry>> entries_;

	int    cursor_idx_; ///< Current index of the menu's cursor.
	Row    rows_;       ///< Maximum rows of menu entries per page.
	Column cols_;       ///< Maximum columns of menu entries per page.

	/////////////////////////////////////////////////////
	/// @brief Move the cursor along menu options.
	/// 
	/// This method does the main work for @property cursorUp, @property cursorDown, 
	/// @property cursorLeft, and @property cursorRight
	/// 
	/// @param dir     Direction to move the cursor to.
	/////////////////////////////////////////////////////
	void
	move(const Direction dir)
	noexcept;
};

}