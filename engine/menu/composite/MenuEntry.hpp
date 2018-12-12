#pragma once

#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>

#include "utility/type/Key.hpp"

namespace nemo
{

class MenuCursor;
class MenuEntryGraphics;

class MenuEntry : public std::enable_shared_from_this<MenuEntry>
{
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
public:

	MenuEntry(
		const int                                id,
		const std::shared_ptr<MenuEntryGraphics> graphics
	);

	virtual
	~MenuEntry() {}

	void
	update(sf::RenderWindow& window, const KeyAction action);

	std::shared_ptr<MenuEntry>
	getParent()
	const;

	void
	setParent(std::shared_ptr<MenuEntry> parent)
	noexcept;

	void 
	setText(const std::string& text);

	virtual std::vector<std::shared_ptr<MenuEntry>>
	getChildren()
	const = 0;

	virtual std::shared_ptr<MenuEntry>
	select() 
	const = 0;

	virtual void
	accept(MenuCursor& cursor) = 0;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
private:
	int                        id_;
	std::shared_ptr<MenuEntry> parent_;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
protected:
	std::shared_ptr<MenuEntryGraphics> graphics_;
};

}