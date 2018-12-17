#include <boost/assert.hpp>

#include "MenuEntryGraphics.hpp"
#include "utility/wrapper/sfVector2.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

XYPair
MenuEntryGraphics::size()
const
{
	const auto sz = cell_.getSize();
	return { XValue(sz.x), YValue(sz.y) };
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
void
MenuEntryGraphics::drawOn(sf::RenderWindow& window)
{
	window.draw(cell_);
	window.draw(text_);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
 
MenuEntryGraphics::MenuEntryGraphics(
	const std::string&              text,
	const XYPair&                   pos,
	const XYPair&                   dim,
	const XYPair&                   margins,
	const TextBoxColors             colors,
	const std::shared_ptr<sf::Font> font,
	const int                       font_sz,
	const AlignConfig               align
)
	: font_(font)
	, colors_(colors)
{
	const auto x0y0 = XYPair(XValue(0.f), YValue(0.f));
	BOOST_ASSERT(pos >= x0y0);
	BOOST_ASSERT(dim > x0y0);
	BOOST_ASSERT(margins >= x0y0);
	BOOST_ASSERT(font != nullptr);
	BOOST_ASSERT(font_sz > 0);
	
	// Create the cell.
	cell_.setSize(sfVector2(dim));
	cell_.setOutlineThickness(-1.f);
	cell_.setOutlineColor(colors.border_);
	cell_.setFillColor(colors.backgnd_);
	cell_.setPosition(sfVector2(pos));
	
	// Create the text.
	text_.setFont(*font_);
	text_.setString(text);
	text_.setCharacterSize(font_sz);
	text_.setFillColor(colors.text_);
	text_.setPosition(cell_.getPosition());

	// Text alignment.
	constexpr auto center_pt = .475f;
	const auto text_width = text_.getLocalBounds().width;
	const auto vt_align = align.vt_ == VtAlign::Center 
		? center_pt * (float(dim.y_) - font_sz)
		: 10.f;
	const auto hz_align = align.hz_ == HzAlign::Center 
		? center_pt * float(dim.x_) - text_width 
		: 10.f;
	
	text_.move(sfVector2( XValue(hz_align), YValue(vt_align) ));
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}