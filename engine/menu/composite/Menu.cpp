#include <boost/assert.hpp>
#include <iostream>
#include <fstream>

#include "nlohmann/json.hpp"

#include "Menu.hpp"
#include "MenuEntry.hpp"
#include "utility/wrapper/sfVector2.hpp"
#include "utility/RC1DConverter.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Menu::Menu(
	const XYPair&         pos,
	const XYPair&         dim,
	const XYPair&         padding,
	const XYPair&         spacing,
	const FontProperties& font,
	const TextBoxColors   box_colors,
	const TextBoxColors   entry_colors,
	const TextBoxColors   hover_colors,
	const RCPair&         row_by_col
)
	: MenuEntry(pos, dim, padding, box_colors, font)
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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Menu::Menu(const std::string& file)
	: Menu(parse(file))
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<MenuEntry>
Menu::setCaption(const std::string& caption)
{
	constexpr auto vt_center = false;
	return makeCaption(caption, vt_center);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void
Menu::addEntry(const std::shared_ptr<Menu> entry)
{
	const auto row_by_col = XYPair(XValue(int(cols_)), YValue(int(rows_)));
	const auto spaced_dim = getInnerSize() / row_by_col;
	const auto dim = spaced_dim - spacing_ * 2.f;
	entry->setSize(dim);

	const auto rc_i = RC1DConverter(cols_).toRowColumn(entries_.size());
	const auto rel_pos = spaced_dim 
		* XYPair(XValue(int(rc_i.c_)), YValue(int(rc_i.r_)))
		+ spacing_;
	const auto abs_pos = getPosition() + rel_pos;
	entry->setPosition(abs_pos);

	entry->setColors(entry_colors_);
	entries_.push_back(entry);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void
Menu::cursorUp() 
noexcept
{
	moveCursor(Direction::Up);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void
Menu::cursorDown()
noexcept
{
	moveCursor(Direction::Down);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void
Menu::cursorLeft()
noexcept
{
	moveCursor(Direction::Left);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void 
Menu::cursorRight()
noexcept
{
	moveCursor(Direction::Right);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Menu::Menu(const ParseInfo& info)
	: Menu(
		info.pos_, 
		info.dim_, 
		info.padding_, 
		info.spacing_, 
		info.font_, 
		info.box_colors_,
		info.entry_colors_, 
		info.hover_colors_, 
		info.row_by_col_
	)
{
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Menu::ParseInfo
Menu::parse(const std::string& file)
const
{
	std::ifstream ifs(file);
	BOOST_ASSERT(ifs);
	nlohmann::json js;
	ifs >> js;

	try {
		const auto js_pos = js.at("position");
		const auto pos = XYPair(
			XValue(js_pos.at("x")), 
			YValue(js_pos.at("y"))
		);

		const auto js_dim = js.at("dimensions");
		const auto dim = XYPair(
			XValue(js_dim.at("width")), 
			YValue(js_dim.at("height"))
		);

		const auto js_padding = js.at("padding");
		const auto padding = XYPair(
			XValue(js_padding.at("horizontal")),
			YValue(js_padding.at("vertical"))
		);

		const auto js_box_colors = js.at("colors");
		const auto box_colors = TextBoxColors(
			BorderColor    { sfMakeColor(js_box_colors.at("border")) },
			BackgroundColor{ sfMakeColor(js_box_colors.at("background")) },
			TextColor      { sfMakeColor(js_box_colors.at("text")) }
		);

		const auto js_font = js.at("font");
		const auto js_alignment = js_font.at("alignment") == "left" 
			? Alignment::Left
			: js_font.at("alignment") == "right"
				? Alignment::Right
				: Alignment::Center;
		
		const auto font = FontProperties(
			std::make_shared<sf::Font>(),
			js_font.at("size"),
			js_alignment
		);
		
		BOOST_VERIFY(font.family_->loadFromFile(js_font.at("family")));

		std::cout << "test" << std::endl;
		const auto row_by_col = RCPair(
			Row(js.at("rows")),
			Column(js.at("columns"))
		);
		
		const auto js_entry = js.at("entry");
		const auto js_entry_spacing = js_entry.at("spacing");
		const auto entry_spacing = XYPair(
			XValue(js_entry_spacing.at("horizontal")),
			YValue(js_entry_spacing.at("vertical"))
		);

		const auto js_entry_colors = js_entry.at("colors");
		const auto js_entry_colors_normal = js_entry_colors.at("normal");
		const auto entry_colors_normal = TextBoxColors(
			BorderColor    { sfMakeColor(js_entry_colors_normal.at("border")) },
			BackgroundColor{ sfMakeColor(js_entry_colors_normal.at("background")) },
			TextColor      { sfMakeColor(js_entry_colors_normal.at("text")) }
		);

		const auto js_entry_colors_hover = js_entry_colors.at("hover");
		const auto entry_colors_hover = TextBoxColors(
			BorderColor    { sfMakeColor(js_entry_colors_hover.at("border")) },
			BackgroundColor{ sfMakeColor(js_entry_colors_hover.at("background")) },
			TextColor      { sfMakeColor(js_entry_colors_hover.at("text")) }
		);

		return ParseInfo(
			pos,
			dim,
			padding,
			entry_spacing,
			font,
			box_colors,
			entry_colors_normal,
			entry_colors_hover,
			row_by_col
		);
	}
	catch (const nlohmann::json::out_of_range& e) {
		std::cout << "failed parsing " << file << ": " << e.what() << std::endl;
		return ParseInfo();
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void 
Menu::moveCursor(const Direction dir)
noexcept
{
	if (entries_.empty()) {
		// No menu options => no cursor => no movement.
		return;
	}	

	auto& cursor_idx = cursor_.idx_;
	const auto last = static_cast<decltype(cursor_.idx_)>(entries_.size()) - 1;

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