#pragma once

#include <vector>

#include "menu/composite/MenuEntry.hpp"
#include "utility/RC1DConverter.hpp"

namespace nemo
{

class MenuGraphics;

class Menu : public MenuEntry
{
public:
	using MenuEntry::MenuEntry;

	void
	add(const std::shared_ptr<MenuEntry> entry);

	void
	moveUp() 
	noexcept;
	
	void
	moveDown() 
	noexcept;

	void
	moveLeft() 
	noexcept;

	void
	moveRight() 
	noexcept;

	virtual std::vector<std::shared_ptr<MenuEntry>>
	getChildren()
	const override;

	virtual std::shared_ptr<MenuEntry>
	select()
	const override;

	virtual void
	accept(MenuCursor& cursor)
	override;

private:
	/**
	 * \brief Enumeration for which direction the player moves the cursor to in 
	 * the menu.
	 * 
	 * This is used by \property move, which \property moveUp, \property 
	 * moveDown, \property moveLeft, and \property moveRight are wrappers of.
	 */ 
	enum class Direction {
		Up,
		Down,
		Right,
		Left
	};

	std::vector<std::shared_ptr<MenuEntry>> entries_;
	int    cursor_idx_;
	Column cols_;

	/**
	 * \brief Move the cursor along menu options.
	 * 
	 * This method does the main work for \property moveUp, \property moveDown, 
	 * \property moveLeft, and \property moveRight
	 *  
	 * \param dir     Direction to move the cursor to.
	 */
	void
	move(const Direction dir)
	noexcept;
};

}