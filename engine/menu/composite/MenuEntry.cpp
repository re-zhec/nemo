#include "MenuEntry.hpp"
#include "MenuEntryGraphics.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuEntry::MenuEntry(
	const int                                id,
	const std::shared_ptr<MenuEntryGraphics> graphics
)
	: id_      (id)
	, graphics_(graphics)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
MenuEntry::update(sf::RenderWindow& window, const KeyAction action)
{
	graphics_->update(window);	
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MenuEntry>
MenuEntry::getParent()
const
{
	return parent_;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
MenuEntry::setParent(std::shared_ptr<MenuEntry> parent)
noexcept
{
	parent_ = parent;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
MenuEntry::setText(const std::string& text)
{
	graphics_->setText(text);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}