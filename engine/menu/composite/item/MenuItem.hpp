#pragma once

#include <string>

#include "menu/composite/MenuEntry.hpp"

namespace nemo
{

class MenuCursor;

class MenuItem : public MenuEntry
{
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
public:

	using MenuEntry::MenuEntry;

	virtual std::shared_ptr<MenuEntry>
	select()
	const override;

	virtual std::vector<std::shared_ptr<MenuEntry>>
	getChildren()
	const override;

	virtual void
	accept(MenuCursor& cursor)
	override;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
private:
};

}