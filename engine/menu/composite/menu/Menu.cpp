#include <algorithm>

#include "Menu.hpp"
#include "../../visitor/MenuCursor.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// void
// Menu::add(const int id, const std::string& text)
// {
// 	const auto option_dim = XYPair(
// 		(dim_.x_ - XValue(2.f) * graphics_->margins_.x_) / XValue(int(cols_)),
// 		(dim_.y_ - YValue(2.f) * margins_.y_) / YValue(int(rows_))
// 	);

// 	const auto r = YValue(entries_.size() / int(rows_));
// 	const auto c = XValue(entries_.size() % int(cols_));
// 	const auto option_pos = XYPair(c * option_dim.x_, r * option_dim.y_);
// 	std::shared_Ptr<MenuItem> item = MenuItem(
// 		id, option_pos, option_dim, 	
// 	)
// }

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
Menu::add(const MenuEntryID id, const std::shared_ptr<MenuEntry> entry)
{
	parent_ = shared_from_this();
	entries_.push_back(entry);
}


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
Menu::cursorUp() 
noexcept
{
	move(Direction::Up);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
Menu::cursorDown()
noexcept
{
	move(Direction::Down);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
Menu::cursorLeft()
noexcept
{
	move(Direction::Left);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::cursorRight()
noexcept
{
	move(Direction::Right);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

std::vector<std::shared_ptr<MenuEntry>>
Menu::children()
const
{
	return entries_;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void
Menu::accept(MenuCursor& cursor)
{
	cursor.visit(*this);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::move(const Direction dir)
noexcept
{
	if (entries_.empty()) {
		// No menu options => no cursor => no movement.
		return;
	}	

	// The cursor should be able to wrap around the ends of the menu.
	const auto last = static_cast<decltype(cursor_idx_)>( entries_.size() ) - 1;

	switch (dir) {
		// Up/down changes the row.
		case Direction::Up: {
			const auto new_idx = cursor_idx_ - int(cols_);
			cursor_idx_ = new_idx >= 0 
				? new_idx
				: cursor_idx_ != 0
					? 0 
					: last;
			break;
		}

		case Direction::Down: {
			const auto new_idx = cursor_idx_ + int(cols_);
			cursor_idx_ = new_idx <= last 
				? new_idx 
				: cursor_idx_ != last 
					? last
					: 0;
			break;
		}

		// Left/right changes the column.
		case Direction::Right: {
			cursor_idx_ = cursor_idx_ < last ? cursor_idx_ + 1 : 0;
			break;
		}
		
		case Direction::Left: {
			cursor_idx_ = cursor_idx_ > 0 ? cursor_idx_ - 1 : last;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}