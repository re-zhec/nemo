#include <boost/assert.hpp>
#include <iostream>
#include <fstream>

#include "nlohmann/json.hpp"

#include "MenuTree.hpp"
#include "MenuNode.hpp"
#include "utility/wrapper/sfVector2.hpp"
#include "utility/RC1DConverter.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

MenuTree::MenuTree(
	const XYPair&         pos,
	const XYPair&         dim,
	const RCPair&         row_by_col,
	const XYPair&         padding,
	const XYPair&         spacing,
	const TextBoxColors   box_colors,
	const TextBoxColors   entry_colors,
	const TextBoxColors   hover_colors,
	const FontProperties& font)

	: MenuNode(pos, dim, padding, box_colors, font)
	, spacing_(spacing)
	, rows_(row_by_col.r_)
	, cols_(row_by_col.c_)
	, entry_colors_(entry_colors)
	, cursor_(hover_colors)
{
	BOOST_ASSERT(spacing >= XYPair(XValue(0.f), YValue(0.f)));
	BOOST_ASSERT(row_by_col.r_ > 0);
	BOOST_ASSERT(row_by_col.c_ > 0);
}
///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MenuNode>
MenuTree::add(const std::shared_ptr<MenuNode> child)
{
	const auto row_by_col = XYPair(XValue(int(cols_)), YValue(int(rows_)));
	const auto spaced_dim = getInnerSize() / row_by_col;
	const auto dim = spaced_dim - spacing_ * 2.f;
	child->setSize(dim);

	const auto rc_i = RC1DConverter(cols_).toRowColumn(children_.size());
	const auto rel_pos = spaced_dim 
		* XYPair(XValue(int(rc_i.c_)), YValue(int(rc_i.r_)))
		+ spacing_;
	const auto abs_pos = getPosition() + rel_pos;
	child->setPosition(abs_pos);

	child->setColors(entry_colors_);
	children_.push_back(child);
	return shared_from_this();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void 
MenuTree::drawIt(sf::RenderWindow& window)
const
{
	drawTextBox(window);

	for (auto c : children_) {
		c->drawIt(window);
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MenuNode>
MenuTree::setCaption(const std::string& caption)
{
	constexpr auto vt_center = false;
	return makeCaption(caption, vt_center);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void
MenuTree::cursorUp() 
noexcept
{
	moveCursor(Direction::Up);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void
MenuTree::cursorDown()
noexcept
{
	moveCursor(Direction::Down);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void
MenuTree::cursorLeft()
noexcept
{
	moveCursor(Direction::Left);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void 
MenuTree::cursorRight()
noexcept
{
	moveCursor(Direction::Right);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void 
MenuTree::moveCursor(const Direction dir)
noexcept
{
	if (children_.empty()) {
		// No menu options => no cursor => no movement.
		return;
	}	

	auto& cursor_idx = cursor_.idx_;
	const auto last = static_cast<decltype(cursor_.idx_)>(children_.size()) - 1;

	switch (dir) {
		// Up/down changes the row.
		case Direction::Up: {
			const auto new_idx = cursor_idx - int(cols_);
			// The cursor should be able to wrap around the ends of the menu.
			cursor_idx = new_idx >= 0 
				? new_idx
				: cursor_idx != 0
					? 0 
					: last;
			break;
		}

		case Direction::Down: {
			const auto new_idx = cursor_idx + int(cols_);
			cursor_idx = new_idx <= last 
				? new_idx 
				: cursor_idx != last 
					? last
					: 0;
			break;
		}

		// Left/right changes the column.
		case Direction::Right: {
			cursor_idx = cursor_idx < last ? cursor_idx + 1 : 0;
			break;
		}
		
		case Direction::Left: {
			cursor_idx = cursor_idx > 0 ? cursor_idx - 1 : last;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

}