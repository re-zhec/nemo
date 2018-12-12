#include <boost/assert.hpp>

#include "MenuEntryGraphics.hpp"
#include "utility/wrapper/sfVector2.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
MenuEntryGraphics::setText(const std::string& text)
{
	text_.setString(text);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
MenuEntryGraphics::setColors(const TextBoxColors colors)
{
	cell_.setOutlineColor(colors.border_);
	cell_.setFillColor(colors.backgnd_);
	text_.setFillColor(colors.text_);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
 
MenuEntryGraphics::MenuEntryGraphics(
	const XYPair&                   pos,
	const XYPair&                   dim,
	const XYPair&                   margins,
	const TextBoxColors             colors,
	const std::shared_ptr<sf::Font> font,
	const int                       font_sz,
	const bool                      center
)
	: font_   (font)
	, font_sz_(font_sz)
	, center_ (center)
{
	const auto x0y0 = XYPair(XValue(0.f), YValue(0.f));
	BOOST_ASSERT(pos >= x0y0);
	BOOST_ASSERT(dim >= x0y0);
	BOOST_ASSERT(margins >= x0y0);

	cell_.setPosition(sfVector2(pos));
	cell_.setSize(sfVector2(dim));
	cell_.setOutlineThickness(-1.f);
	cell_.setFillColor(colors.backgnd_);
	cell_.setOutlineColor(colors.border_);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}