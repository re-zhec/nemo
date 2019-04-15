#include "MenuNode.hpp"
#include "utility/wrapper/sfVector2.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

MenuNode::MenuNode(
	const XYPair&         pos,
	const XYPair&         dim,
	const XYPair&         padding,
	const TextBoxColors   colors,
	const FontProperties& font)
	
	: font_  (font)
	, colors_(colors)
{
	const auto x0y0 = XYPair(XValue(0.f), YValue(0.f));
	BOOST_ASSERT(pos >= x0y0);
	BOOST_ASSERT(dim >= x0y0);
	BOOST_ASSERT(padding >= x0y0);
	
	BOOST_ASSERT(font.family_ != nullptr);
	BOOST_ASSERT(font.size_ > 0);

	// Create the entry cell.
	cell_.setSize( sfVector2(dim) );
	cell_.setPosition( sfVector2(pos) );
	cell_.setOutlineThickness(-1.f);
	cell_.setOutlineColor(colors.border_.v_);
	cell_.setFillColor(colors.backgnd_.v_);

	// Account for padding. Entry content will be placed in the allocated space.
	space_.setSize( sfVector2(dim - padding - padding) );
	space_.setPosition( sfVector2(pos + padding) );

	// Set future caption's color.
	caption_.setFillColor(colors.text_.v_);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MenuNode>
MenuNode::setColors(const TextBoxColors colors)
{
	cell_.setOutlineColor(colors.border_.v_);
	cell_.setFillColor(colors.backgnd_.v_);
	caption_.setFillColor(colors.text_.v_);
	return shared_from_this();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MenuNode>
MenuNode::setPosition(const XYPair& pos)
{
	const auto pos_v = sfVector2(pos);
	const auto padding_v = space_.getPosition() - cell_.getPosition();
	
	cell_.setPosition(pos_v);
	space_.setPosition(pos_v + padding_v);
	return shared_from_this();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

XYPair
MenuNode::getPosition()
const noexcept
{
	const auto pos_v = cell_.getPosition();
	return { XValue(pos_v.x), YValue(pos_v.y) };
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MenuNode>
MenuNode::setSize(const XYPair& dim)
{
	const auto dim_v = sfVector2(dim);
	const auto padding_v = space_.getPosition() - cell_.getPosition();
	
	cell_.setSize(dim_v);
	space_.setSize(dim_v - 2.f * padding_v);

	// Reset caption to re-align it.
	setCaption(caption_.getString());
	return shared_from_this();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

XYPair
MenuNode::getInnerSize()
const noexcept
{
	const auto inner_dim_v = space_.getSize();
	return { XValue(inner_dim_v.x), YValue(inner_dim_v.y) };
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MenuNode>
MenuNode::makeCaption(const std::string& caption, bool vt_center)
{
	// Create graphical text.
	caption_.setFont(*font_.family_);
	caption_.setString(caption);
	caption_.setCharacterSize(font_.size_);
	caption_.setPosition(space_.getPosition());
	
	// Vertically align the title.
	const auto space_height = YValue(space_.getSize().y);
	const auto caption_height = YValue(caption_.getLocalBounds().height);
	const auto y_to_move = vt_center 
		? (space_height - caption_height) / YValue(2.f) // Center.
		: YValue(5.f);                                  // Top.
	
	// Horizontally align the title.
	auto x_to_move = XValue(5.f);
	const auto caption_width = XValue(caption_.getLocalBounds().width);
	const auto space_width = XValue(space_.getSize().x);

	switch (font_.align_) {
		case Alignment::Left:
			break;

		case Alignment::Center:
			x_to_move = (space_width - caption_width) / XValue(2.f);
			break;

		case Alignment::Right:
			x_to_move = space_width - caption_width - x_to_move;
			break;
	}

	caption_.move(sfVector2(x_to_move, y_to_move));
	return shared_from_this();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void 
MenuNode::drawTextBox(sf::RenderWindow& window)
const
{
	window.draw(cell_);
	window.draw(caption_);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

}