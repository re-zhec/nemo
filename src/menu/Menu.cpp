#include <algorithm>
#include <fstream>
#include <boost/assert.hpp>

#include "nlohmann/json.hpp"
#include "utility/type/XYValue.hpp"
#include "utility/type/RowColumn.hpp"
#include "utility/wrapper/sfml.hpp"
#include "Menu.hpp"

namespace sb
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Menu::Menu(
	const XYValue      pos,
	const XYValue      dim,
	const Row          rows, 
	const Column       cols,
	const XYValue      outer_margins,
	const XYValue      inner_margins,
	const bool         align_center,
	const size_t       char_sz,
	const TextBoxColor option_color,
	const TextBoxColor cursor_color,
	const TextBoxColor box_color,
	const std::string& font_file
)	
	: align_center_(align_center)
	, rows_(rows)
	, cols_(cols)
	, rc1d_conv_(cols)
	, option_color_(option_color)
	, cursor_color_(cursor_color)
	, cursor_rc_({ Row(0), Column(0) })
	, char_sz_(char_sz)	
{
	BOOST_ASSERT(pos.x_ >= 0.f && pos.y_ >= 0.f);
	BOOST_ASSERT(dim.x_ >= 0.f && dim.y_ >= 0.f);
	BOOST_ASSERT(rows_ > 0);
	BOOST_ASSERT(cols_ > 0);
	BOOST_ASSERT(char_sz_ > 0);

	// Load the font file.
	BOOST_VERIFY(font_.loadFromFile(font_file));

	// Create the menu box.
	box_.setSize(sfVector2(dim));
	box_.setPosition(sfVector2(pos));
	box_.setOutlineThickness(-1.f);	
	box_.setFillColor(box_color.backgnd_);
	box_.setOutlineColor(box_color.border_);

	// The area of the menu inside the margins is reserved for menu options. From
	// there, the number of rows and columns of options determine each option's 
	// width and height. For now, inner margins are included in the calculated 
	// width and height.
	const auto option_dim_v = sfVector2(
		(dim.x_ - XValue(2.f) * outer_margins.x_) / XValue(int(cols_)),
		(dim.y_ - YValue(2.f) * outer_margins.y_) / YValue(int(rows_))
	);
	
	// Create background cells to contain the menu options in one page. To save 
	// memory, only one page worth is needed since the options outside of them 
	// won't be drawn on screen and all cells' positions stayed the same from 
	// page to page. We can decide the color of a cell later from the contained 
	// option's color configuration.
	const auto noptions_per_page = int(rows_) * int(cols_);
	options_.reserve(noptions_per_page);
	cells_.reserve(noptions_per_page);

	for (auto i = 0; i < noptions_per_page; ++i) {
		// Adjust cell cize so that inner margins can be inserted between them.
		const auto inner_margins_v = sfVector2(inner_margins);
		const auto cell_dim_v = option_dim_v - 2.f * inner_margins_v;
		BOOST_ASSERT(cell_dim_v.x > char_sz_ && cell_dim_v.y > char_sz_);

		sf::RectangleShape cell(cell_dim_v);

		// Insert inner margins.
		cell.setOrigin(-inner_margins_v);

		// Place the cell in the appropriate spot in the menu. Cells line up from 
		// left to right, down across rows.
		const auto rc_i = rc1d_conv_.toRowColumn(i);
		cell.setPosition(
			inner_margins_v
			+ sfVector2(
				XValue(option_dim_v.x * int(rc_i.c_)), 
				YValue(option_dim_v.y * int(rc_i.r_))
			)
			+ sfVector2(pos) 
		);

		// The coloring of the cells is decided in drawOption() since they can 
		// change depending on whether a cursor is over one or if the client 
		// requests a specific menu option to be colored differently.
		cell.setOutlineThickness(1.f);
		cells_.push_back(cell);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Menu::Menu(const std::string& file)
	: Menu(parseFile(file))
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Menu& 
Menu::add(const MenuOptionID id, const std::string& txt)
{
	// Make sure there is no other menu option that has the new ID.
	const auto it = find(id);
	BOOST_ASSERT(it == options_.cend());

	// Create the option's graphical text.
	sf::Text option_txt(txt, font_, char_sz_);
	
	// Add the option to the menu.
	options_.push_back({ id, option_txt, option_color_ });
	
	// Preset the option text's position on the menu for future rendering. Since 
	// it was just added to menu, we can use the index of the last element in the
	// menu option container.
	presetTextPosition(options_.size() - 1);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Menu& 
Menu::remove(const MenuOptionID id)
{
	// Delete the option.
	auto iter = find(id);
	BOOST_ASSERT(iter != options_.cend());
	iter = options_.erase(iter);

	// All options that followed the removed one need to have their text's render 
	// positions shifted frontward one slot. presetTextPosition already accounts
	// for this when called after the option is deleted.
	for (auto it = iter; it != options_.cend(); ++it) {
		presetTextPosition(it - options_.cbegin());
	}
	
	if (const auto cur_idx = rc1d_conv_.to1D(cursor_rc_);
		cur_idx == static_cast<decltype(cur_idx)>(options_.size()))
	{
		// The cursor was on the last option before one of the options was 
		// removed. Move the cursor frontward because it's now hovering over 
		// nothing.
		moveLeft();
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Menu& 
Menu::changeOptionText(const MenuOptionID id, const std::string& txt)
{
	const auto it = find(id);
	BOOST_ASSERT(it != options_.cend());
	it->txt_.setString(txt);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Menu& 
Menu::changeOptionColor(const MenuOptionID id, const TextBoxColor color)
{
	const auto it = find(id);
	BOOST_ASSERT(it != options_.cend());
	setOptionColor(it - options_.cbegin(), color);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool 
Menu::empty() const noexcept
{
	return options_.empty();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::moveUp()
{
	if (const auto last = rc1d_conv_.toRowColumn(options_.size() - 1);
		last.r_ == 0) 
	{
		// Up => left in a horizontal menu, which can be determined based on 
		// whether last menu option is on the first row.
		move(Direction::Left);
	}
	else {
		// move() checks that the menu isn't empty, so no need to deal with that 
		// here.
		move(Direction::Up);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::moveDown()
{
	if (const auto last = rc1d_conv_.toRowColumn(options_.size() - 1);
		last.r_ == 0)
	{
		// Down => right in a horizontal menu.
		move(Direction::Right);
	} 
	else {
		// move() checks that the menu isn't empty.
		move(Direction::Down);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::moveRight()
{
	// move() checks that the menu isn't empty, so no need to deal with that 
	// here.
	if (cols_ == 1) {
		// Right => down in a vertical menu. The number of columns is always 
		// capped at the number of columns per page, unlike the number of rows in 
		// moveUp() and moveDown()'s cases.
		move(Direction::Down);
	} 
	else {
		move(Direction::Right);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::moveLeft()
{
	// move() checks that the menu isn't empty.
	if (cols_ == 1) {
		// Left => up in a vertical menu.
		move(Direction::Up);
	} 
	else {
		move(Direction::Left);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::draw(sf::RenderWindow& window)
{
	// Draw the menu box.
	window.draw(box_);

	// All the menu options aren't going to be drawned on screen. Only the page 
	// of options that has the cursor needs to be.
	const auto page_sz = cells_.size();
	const auto idx = rc1d_conv_.to1D(cursor_rc_);
	const auto cur_page = idx / page_sz;

	// Draw from the first to the last option of that page. In case that page 
	// happens to be the last one, since the page doesn't necessarily have all 
	// its rows and columns filled, be sure to stop at the very last option.
	const auto start = cur_page * page_sz;
	const auto n = options_.size();
	const auto end = std::min(start + page_sz, n);

	for (auto i = start; i < end; ++i) {
		drawOption(i, window);
	}

	if (n > page_sz) {
		// If the options fill up the menu past one page, draw the current page 
		// number out of the total so that the player would know where they are. 
		// Draw the navigation arrow indicators as well.
		drawPageRef(window);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

std::optional<MenuOptionID>
Menu::cursorAt() const
{
	if (options_.empty()) {
		// Empty menu.
		return {};
	}

	const auto idx = rc1d_conv_.to1D(cursor_rc_);
	return { options_[idx].id_ };
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::presetTextPosition(const int idx)
{
	BOOST_ASSERT(idx >= 0 && idx < static_cast<decltype(idx)>(options_.size()));

	// Menu options are positioned from left to right down across rows. After 
	// a page is filled, the graphical positions start over from the top left 
	// for a new page.
	const auto& cell = cells_[idx % cells_.size()];
	auto& txt = options_[idx].txt_;
	txt.setOrigin(cell.getOrigin());
	txt.setPosition(cell.getPosition());

	// Vertically center this option in the cell it is placed in. Horizontal 
	// alignment depends on what was requested during construction.
	constexpr auto center_pt = .475f;
	const auto [width, height] = cell.getSize();
	const auto vtalign = center_pt * (height - char_sz_);
	const auto txt_width = txt.getLocalBounds().width;
	const auto hzalign = align_center_ ? center_pt * (width - txt_width) : 10.f;
	
	txt.move(sfVector2( XValue(hzalign), YValue(vtalign) ));
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::setOptionColor(const int idx, const TextBoxColor color)
{
	BOOST_ASSERT(idx >= 0 && idx < static_cast<decltype(idx)>(options_.size()));
	options_[idx].color_ = color;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::drawOption(const int idx, sf::RenderWindow& window)
{
	BOOST_ASSERT(idx >= 0 && idx < static_cast<decltype(idx)>(options_.size()));

	// Although we can have pages of menu options, to save memory, we have only a 
	// page worth of cells to use. So, grab the one the menu option would be
	// drawn on.
	auto& cell = cells_[idx % cells_.size()];
	auto& option = options_[idx];

	// If the cursor is over this menu option, then use the cursor's colorset
	// instead of the option's normal set.
	const auto cursor_idx = rc1d_conv_.to1D(cursor_rc_);
	const auto color = idx != cursor_idx ? option.color_ : cursor_color_;

	option.txt_.setFillColor(color.txt_);
	cell.setFillColor(color.backgnd_);
	cell.setOutlineColor(color.border_);

	// Draw the cell first, then the text over it.
	window.draw(cell);
	window.draw(option.txt_);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::drawPageRef(sf::RenderWindow& window) const
{
	// Get the page number the cursor is on as well as the total number of pages 
	// of options the menu has.
	const auto page_sz = cells_.size();
	const auto npages = (options_.size() - 1) / page_sz + 1;
	const auto cur_page = rc1d_conv_.to1D(cursor_rc_) / page_sz;
	const auto atpage_txt = std::to_string(cur_page + 1) 
		+ " / " 
		+ std::to_string(npages);

	// Draw a small box that will contain the page numbers and navigation arrow
	// indicators, assuming the menu has more than one page.
	constexpr auto atpage_box_height = YValue(25.f);			
	constexpr auto atpage_box_width = XValue(5.f) 
		* XValue(float(atpage_box_height));

	// The box should have the same background layer as the menu box's since it 
	// will be appended to the menu. 
	sf::RectangleShape atpage_box(sfVector2(atpage_box_width, atpage_box_height));
	
	atpage_box.setFillColor(box_.getFillColor());
	atpage_box.setOutlineColor(box_.getOutlineColor());
	atpage_box.setOutlineThickness(box_.getOutlineThickness());
	
	// Place it directly below the bottom right corner of the menu.
	atpage_box.setPosition(box_.getPosition() + box_.getSize());
	atpage_box.move(sfVector2(
		-atpage_box_width, 
		YValue(box_.getOutlineThickness())
	));

	window.draw(atpage_box);

	// Draw the page numbers on the right half of the box.
	constexpr auto atpage_txt_height = float(atpage_box_height) - 9.f;
	sf::Text atpage(atpage_txt, font_, atpage_txt_height);
	atpage.setOrigin(sfVector2( XValue(0.f), YValue(-2.f) ));
	atpage.setPosition(
		atpage_box.getPosition() + 
		sfVector2(XValue(.5f) * atpage_box_width, YValue(0.f))
	);

	// For the color of the page number and navigation arrow indicators, use the 
	// menu options' default text color.
	atpage.setFillColor(option_color_.txt_);
	window.draw(atpage);
	
	if (npages > 1) {
		// Draw the navigation arrow indicators on the left half of the box.
		// Up arrow.
		constexpr auto arrow_sz = float(atpage_box_height) - 7.f;
		constexpr auto arrow_radius = .5f * arrow_sz;
		constexpr auto arrow_padding = .5f * arrow_radius;
		
		sf::CircleShape up(arrow_radius, 3);
		up.setFillColor(option_color_.txt_);
		up.setOrigin(-arrow_padding, -arrow_padding);
		up.setPosition(
			atpage_box.getPosition() + 
			sfVector2(XValue(arrow_padding), YValue(2.f))
		);

		window.draw(up);

		// Down arrow right next to the up arrow.
		sf::CircleShape down(up);
		down.scale(sfVector2( XValue(1.f), YValue(-1.f) ));
		down.move(sfVector2(
			XValue(2.f * arrow_radius), 
			YValue(2.5f * arrow_radius)
		));
		
		window.draw(down);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
Menu::move(const Direction dir)
{
	if (options_.empty()) {
		// No menu options => no cursor => no movement.
		return;
	}	

	// The cursor should be able to wrap around the ends of the menu. Moving the 
	// cursor left when it is at the leftmost option should take it to the 
	// rightmost option at the same row, and vice versa. Similarly, moving it up 
	// when it is at the topmost option should take it to the bottomost option at 
	// the same column, and vice versa. The wrapping should take into account 
	// that the bottomost row may be partially filled, which column the cursor 
	// can move to in the last row.

	// Get the row and column indices of the last option.
	const auto last = rc1d_conv_.toRowColumn(options_.size() - 1);

	// Get the rightmost column at the current row the cursor is on. It's needed 
	// when moving left and right.
	auto& [r, c] = cursor_rc_;
	const auto right_c = r < last.r_ ? cols_ - Column(1) : last.c_;

	switch (dir) {
		// Up/down changes the row index.
		// If the cursor will move to the bottom row but there's no option exactly 
		// below it, move it to the last option.
		case Direction::Up:
			r = r > 0 ? r - Row(1) : last.r_;
			c = r < last.r_ ? c : std::min(c, last.c_);
			break;
		case Direction::Down:
			r = r < last.r_ ? r + Row(1) : Row(0);
			c = r < last.r_ ? c : std::min(c, last.c_);
			break;
		
		// Left/right changes the column index
		case Direction::Right:
			c = c < right_c ? c + Column(1) : Column(0);
			break;
		case Direction::Left:
			c = c > 0 ? c - Column(1) : right_c;
			break;
		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

auto 
Menu::find(const MenuOptionID id) -> decltype(options_.begin())
{
	auto it = std::find_if(options_.begin(), options_.end(),
		[id](const auto& option) {
			return id == option.id_;
		}
	);

	return it;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Menu::Menu(ctor_args args)
	: Menu(
		args.pos,
		args.dim,
		args.rows,
		args.cols,
		args.outer_margins,
		args.inner_margins,
		args.align_center,
		args.char_sz,
		args.option_color,
		args.cursor_color,
		args.box_color,
		args.font_file
	)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

typename Menu::ctor_args 
Menu::parseFile(const std::string& file)
{	
	// Load json file.
	std::ifstream ifs(file);
	BOOST_ASSERT(ifs.is_open());
	nlohmann::json js;
	ifs >> js;

	// Populate constructor arguments struct
	ctor_args args = {};
	constexpr auto position   = "position";
	constexpr auto dimensions = "dimensions";
	constexpr auto margins    = "margins";
	constexpr auto horizontal = "horizontal";
	constexpr auto vertical   = "vertical";
	constexpr auto options    = "options";
	constexpr auto cursor     = "cursor";
	constexpr auto box        = "box";
	constexpr auto colors     = "colors";
	constexpr auto text       = "text";
	constexpr auto background = "background";
	constexpr auto border     = "border";

	try {
		args.pos = {
			XValue(js.at(position).at("x")), 
			YValue(js.at(position).at("y"))
		};

		args.dim = { 
			XValue(js.at(dimensions).at("width")), 
			YValue(js.at(dimensions).at("height")) 
		};

		args.outer_margins = {
			XValue(js.at(box).at(margins).at(horizontal)),
			YValue(js.at(box).at(margins).at(vertical))
		};

		args.inner_margins = {
			XValue(js.at(options).at(margins).at(horizontal)),
			YValue(js.at(options).at(margins).at(vertical))
		};

		args.rows         = Row(js.at(options).at("rows"));
		args.cols         = Column(js.at(options).at("columns"));
		args.align_center = js.at(options).at("center");
		args.char_sz      = js.at(options).at("size");
		args.font_file		= js.at("font");

		args.option_color = {
			args.makeColor(js.at(options).at(colors).at(text)),
			args.makeColor(js.at(options).at(colors).at(background)),
			args.makeColor(js.at(options).at(colors).at(border))
		};

		args.cursor_color = {
			args.makeColor(js.at(cursor).at(colors).at(text)),
			args.makeColor(js.at(cursor).at(colors).at(background)),
			args.makeColor(js.at(cursor).at(colors).at(border))
		};

		args.box_color = {
			{0, 0, 0},
			args.makeColor(js.at(box).at(colors).at(background)),
			args.makeColor(js.at(box).at(colors).at(border))
		};
	}
	catch (const nlohmann::json::out_of_range& e) {
		// BOOST_LOG_TRIVIAL(error) << file << " parsing failed. " << e.what();
	}

	return args;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

sf::Color 
Menu::ctor_args::makeColor(const std::array<int, 4>& rgba)
{
	return {
		static_cast<sf::Uint8>(rgba[0]),
		static_cast<sf::Uint8>(rgba[1]),
		static_cast<sf::Uint8>(rgba[2]),
		static_cast<sf::Uint8>(rgba[3])
	};
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}