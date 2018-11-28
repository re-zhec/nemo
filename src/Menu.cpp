#include <algorithm>
#include <fstream>
#include <boost/assert.hpp>
#include "../include/Menu.hpp"
#include "../include/logger.hpp"

namespace rp
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

namespace {
	/**
	 * \brief For data that is represented as a 2D matrix but is actually 1-D, 
	 * this function converts the row and column coordinate to its 1-D 
	 * counterpart.
	 * 
	 * The row and column indices are expected to be 0-based.
	 * 
	 * This function is used to get the index of a particular menu option. All  
	 * menu options are stored in a vector being displayed as a 2D matrix on the 
	 * render window.
	 *
	 * \param r			Row index.
	 * \param c			Column index.
	 * \param ncols	Number of columns per row.
	 * 
	 * \return 0-based 1-D index..
	 */
	inline auto
	translateTo1DIndex(const size_t r, const size_t c, const size_t ncols)
	noexcept
	{
		// Indices are numbered from left to right, down across rows.
		return r * ncols + c;
	}

	/**
	 * \brief This function is a wrapper for the overloaded one above. Instead 
	 * of two parameters for the row and column indices, this method takes them 
	 * one parameter, a pair.
	 *
	 * \param rc		Row and column indices.
	 * \param ncols	Number of columns per row.
	 * 
	 * \return 0-based 1-D index.
	 */
	inline auto
	translateTo1DIndex(const std::pair<size_t, size_t> rc, const size_t ncols)
	noexcept
	{
		const auto [r, c] = rc;
		return translateTo1DIndex(r, c, ncols);
	}

	/**
	 * \brief This function does the opposite of \property translateTo1DIndex. 
	 * It converts an 1-D index to the corresponding row and column indices.
	 *
	 * \param idx		0-based 1-D index.
	 * \param ncols	Number of columns per row.
	 * 
	 * \return Row and column indices.
	 */
	inline auto
	translateToRowColumn(const size_t idx, const size_t ncols) 
	noexcept
	{
		return std::make_pair(idx / ncols, idx % ncols);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T> 
Menu<T>::Menu(
	const sf::Vector2f pos,
	const sf::Vector2f dim,
	const size_t rows, 
	const size_t cols,
	const sf::Vector2f outer_margins,
	const sf::Vector2f inner_margins,
	const bool align_center,
	const size_t char_sz,
	const sf_color3 option_color,
	const sf_color3 cursor_color,
	const sf_color2 box_color,
	const std::string& font_file
)	
	: m_align_center(align_center)
	, m_rows(rows)
	, m_cols(cols)
	, m_option_color(option_color)
	, m_cursor_rc({0, 0})
	, m_cursor_color(cursor_color)
	, m_char_sz(char_sz)	
{
	BOOST_ASSERT(pos.x >= 0.f && pos.y >= 0.f);
	BOOST_ASSERT(dim.x >= 0.f && dim.y >= 0.f);
	BOOST_ASSERT(m_rows > 0);
	BOOST_ASSERT(m_cols > 0);
	BOOST_ASSERT(m_char_sz > 0);

	// Load the font file.
	BOOST_VERIFY(m_font.loadFromFile(font_file));

	// Create the menu box.
	m_box.setSize(dim);
	m_box.setPosition(pos);
	m_box.setOutlineThickness(-1.f);
	
	const auto [box_back_color, box_bord_color] = box_color;
	m_box.setFillColor(box_back_color);
	m_box.setOutlineColor(box_bord_color);

	// The area of the menu inside the margins is reserved for menu options. From
	// there, the number of rows and columns of options determine each option's 
	// width and height. For now, inner margins are included in the width and 
	// height.
	const auto option_dim = sf::Vector2f(
		(dim.x - 2.f * outer_margins.x) / m_cols,
		(dim.y - 2.f * outer_margins.y) / m_rows
	);

	// Reserve some number of options to ease the number of forced memory 
	// reallocations.
	const auto noptions_per_page = m_rows * m_cols;
	m_options.reserve(noptions_per_page);
	
	// Create background cells to contain the menu options in one page. To save 
	// memory, only one page worth is needed since the options outside of them 
	// won't be drawn on screen and all cells' positions stayed the same from 
	// page to page. We can decide the color of a cell later from the contained 
	// option's color configuration.
	m_cells.reserve(noptions_per_page);

	for (auto i = 0u; i < noptions_per_page; ++i) {
		// Adjust cell cize so that inner margins can be inserted between them.
		const auto cell_dim = option_dim - 2.f * inner_margins;
		BOOST_ASSERT(cell_dim.x > m_char_sz && cell_dim.y > m_char_sz);
		sf::RectangleShape cell(cell_dim);

		// Insert inner margins.
		cell.setOrigin(-inner_margins);

		// Place the cell in the appropriate spot in the menu. Cells line up from 
		// left to right, down across rows.
		const auto [r_i, c_i] = translateToRowColumn(i, m_cols);
		cell.setPosition(
			  pos 
			+ inner_margins 
			+ sf::Vector2f(option_dim.x * c_i, option_dim.y * r_i)
		);

		// The coloring of the cells is decided in drawOption() since they can 
		// change depending on whether a cursor is over one or if the client 
		// requests a specific menu option to be colored differently.
		cell.setOutlineThickness(1.f);
		m_cells.push_back(cell);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
Menu<T>::Menu(const std::string& file)
	: Menu(parseFile(file))
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
Menu<T>& 
Menu<T>::add(const T id, const std::string& txt)
{
	// Make sure there is no other menu option that has the new ID.
	const auto it = find(id);
	BOOST_ASSERT(it == m_options.cend());

	// Create the option's graphical text.
	sf::Text option(txt, m_font, m_char_sz);
	
	// Add the option to the menu.
	m_options.push_back({id, option, m_option_color});
	
	// Preset the option text's position on the menu for future rendering. Since 
	// it was just added to menu, we can use the index of the last element in the
	// menu option container.
	presetTextPosition(m_options.size() - 1);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
Menu<T>& 
Menu<T>::remove(const T id)
{
	// Search for the option.
	auto it = find(id);
	BOOST_ASSERT(it != m_options.cend());

	// Delete it.
	it = m_options.erase(it);
	const auto rm_idx = static_cast<decltype(m_options.size())>(
		it - m_options.cbegin());

	// All options that followed the removed one need to have their text's render 
	// positions shifted frontward one slot.
	for (auto i = rm_idx; i < m_options.size(); ++i) {
		presetTextPosition(i);
	}

	// The cursor needs to be refocused if it was on or after the option that was
	// removed.
	if (auto cur_idx = translateTo1DIndex(m_cursor_rc, m_cols);
		cur_idx >= rm_idx)
	{
		// Move the cursor frontward if it was on the last option when that was 
		// removed, as the cursor is hovering over nothing there.
		if (cur_idx == m_options.size()) {
			moveLeft();
		}
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
Menu<T>& 
Menu<T>::changeOptionText(const T id, const std::string& txt)
{
	// Search for the option.
	const auto it = find(id);
	BOOST_ASSERT(it != m_options.cend());
	[[maybe_unused]] auto& [UNUSED0_, cur_txt, UNUSED2_] = *it;

	// Change to new text.
	cur_txt.setString(txt);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
Menu<T>& 
Menu<T>::changeOptionColor(const T id, const sf_color3 color)
{
	// Search for the option.
	const auto it = find(id);
	BOOST_ASSERT(it != m_options.cend());

	// Change its colors.
	setOptionColor(it - m_options.cbegin(), color);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool 
Menu<T>::empty() const noexcept
{
	return m_options.empty();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void 
Menu<T>::moveUp()
{
	if ([[maybe_unused]] const auto 
		[last_r, UNUSED_] = translateToRowColumn(m_options.size() - 1, m_cols);
		last_r == 0) 
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

template <typename T>
void 
Menu<T>::moveDown()
{
	if ([[maybe_unused]] const auto 
		[last_r, UNUSED_] = translateToRowColumn(m_options.size() - 1, m_cols);
		last_r == 0)
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

template <typename T>
void 
Menu<T>::moveRight()
{
	// move() checks that the menu isn't empty, so no need to deal with that 
	// here.
	if (m_cols == 1) {
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

template <typename T>
void 
Menu<T>::moveLeft()
{
	// move() checks that the menu isn't empty.
	if (m_cols == 1) {
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

template <typename T>
void 
Menu<T>::draw(sf::RenderWindow& window)
{
	// Draw the menu box.
	window.draw(m_box);

	// The entire container of options isn't going to be drawned on screen. Only 
	// the page of options where the cursor is needs to be.
	const auto page_sz = m_cells.size();
	const auto idx = translateTo1DIndex(m_cursor_rc, m_cols);
	const auto cur_page = idx / page_sz;

	// Draw from the first to the last option from that page. In case that page 
	// happens to be the last one, since the page doesn't necessarily have all 
	// its rows and columns filled, be sure to stop after the very last option.
	const auto start = cur_page * page_sz;
	const auto n = m_options.size();
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

template <typename T>
std::optional<T> 
Menu<T>::cursorAt() const
{
	if (m_options.empty()) {
		// Empty menu.
		return {};
	}

	const auto idx = translateTo1DIndex(m_cursor_rc, m_cols);
	[[maybe_unused]] const auto& [id, UNUSED1_, UNUSED2_] = m_options[idx];
	return {id};
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
Menu<T>::Menu(ctor_args args)
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

template <typename T>
void 
Menu<T>::presetTextPosition(const size_t idx)
{
	BOOST_ASSERT(idx < m_options.size());

	// Menu options are positioned from left to right down across rows. After 
	// a page is filled, the graphical positions start over from the top left 
	// for a new page.
	const auto& cell = m_cells[idx % m_cells.size()];
	[[maybe_unused]] auto& [UNUSED0_, txt, UNUSED2_] = m_options[idx];
	txt.setOrigin(cell.getOrigin());
	txt.setPosition(cell.getPosition());

	// Vertically center this option in the cell it is placed in. Horizontal 
	// alignment depends on what was requested during construction.
	constexpr auto center_pt = 0.475f;
	const auto [width, height] = cell.getSize();
	const auto vtalign = center_pt * (height - m_char_sz);
	const auto txt_width = txt.getLocalBounds().width;
	const auto hzalign = m_align_center ? center_pt * (width - txt_width) : 10.f;
	
	txt.move(hzalign, vtalign);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void 
Menu<T>::move(const Direction dir)
{
	if (m_options.empty()) {
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
	const auto [last_r, last_c] = translateToRowColumn(
		m_options.size() - 1, m_cols
	);

	// Get the rightmost column at the current row the cursor is on. It's needed 
	// when moving left and right.
	auto& [r, c] = m_cursor_rc;
	const auto right_c = r < last_r ? m_cols - 1 : last_c;

	switch (dir) {
		// Up/down changes the row index.
		// If the cursor will move to the bottom row but there's no option exactly 
		// below it, move it to the last option.
		case Direction::Up:
			r = r > 0 ? r - 1 : last_r;
			c = r < last_r ? c : std::min(c, last_c);
			break;
		case Direction::Down:
			r = r < last_r ? r + 1 : 0;
			c = r < last_r ? c : std::min(c, last_c);
			break;
		
		// Left/right changes the column index
		case Direction::Right:
			c = c < right_c ? c + 1 : 0;
			break;
		case Direction::Left:
			c = c > 0 ? c - 1 : right_c;
			break;
		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void 
Menu<T>::setOptionColor(const size_t idx, const sf_color3 color)
{
	BOOST_ASSERT(idx < m_options.size());
	[[maybe_unused]] auto& [UNUSED0_, UNUSED1_, cur_color] = m_options[idx];
	cur_color = color;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void 
Menu<T>::setOptionColor(const size_t r, const size_t c, 
	const sf_color3 color)
{
	const auto idx = translateTo1DIndex(r, c, m_cols);
	setOptionColor(idx, color);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void 
Menu<T>::drawOption(const size_t idx, sf::RenderWindow& window)
{
	BOOST_ASSERT(idx < m_options.size());

	// Although we can have pages of menu options, to save memory, we have only a 
	// page worth of cells to use. So, grab the one the menu option would be
	// drawn on.
	auto& cell = m_cells[idx % m_cells.size()];
	[[maybe_unused]] auto& [UNUSED_, txt, color] = m_options[idx];

	// If the cursor is over this menu option, then use the cursor's colorset
	// instead of the option's normal set.
	const auto cursor_idx = translateTo1DIndex(m_cursor_rc, m_cols);
	const auto [txt_color, back_color, bord_color] = idx != cursor_idx 
		? color 
		: m_cursor_color;

	txt.setFillColor(txt_color);
	cell.setFillColor(back_color);
	cell.setOutlineColor(bord_color);

	// Draw the cell first, then the text over it.
	window.draw(cell);
	window.draw(txt);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void 
Menu<T>::drawPageRef(sf::RenderWindow& window) const
{
	// Get the page number the cursor is on as well as the total number of pages 
	// of options the menu has.
	const auto page_sz = m_cells.size();
	const auto npages = (m_options.size() - 1) / page_sz + 1;
	const auto cur_page = translateTo1DIndex(m_cursor_rc, m_cols) / page_sz;
	const auto atpage_txt = std::to_string(cur_page + 1) 
		+ " / " 
		+ std::to_string(npages);

	// Draw a small box that will contain the page numbers and navigation arrow
	// indicators, assuming the menu has more than one page.
	constexpr auto atpage_box_height = 25.f;			
	constexpr auto atpage_box_width = 5.f * atpage_box_height;

	// The box should have the same background layer as the menu box's since it 
	// will be appended to the menu. 
	sf::RectangleShape atpage_box({atpage_box_width, atpage_box_height});
	atpage_box.setFillColor(m_box.getFillColor());
	atpage_box.setOutlineColor(m_box.getOutlineColor());
	atpage_box.setOutlineThickness(m_box.getOutlineThickness());
	
	// Place it directly below the bottom right corner of the menu.
	atpage_box.setPosition(m_box.getPosition() + m_box.getSize());
	atpage_box.move(-atpage_box_width, m_box.getOutlineThickness());
	window.draw(atpage_box);

	// For the color of the page number and navigation arrow indicators, use the 
	// cursor's text color
	[[maybe_unused]] const auto [txt_color, UNUSED1_, UNUSED2_] = m_option_color;

	// Draw the page numbers on the right half of the box.
	constexpr auto atpage_txt_height = atpage_box_height - 9.f;
	sf::Text atpage(atpage_txt, m_font, atpage_txt_height);
	atpage.setOrigin(0.f, -2.f);
	atpage.setFillColor(txt_color);
	atpage.setPosition(
		atpage_box.getPosition() + 
		sf::Vector2f(.5f * atpage_box_width, 0.f)
	);

	window.draw(atpage);
	
	if (npages > 1) {
		// Draw the navigation arrow indicators on the left half of the box.
		// Up arrow.
		constexpr auto arrow_sz = atpage_box_height - 7.f;
		constexpr auto arrow_radius = .5f * arrow_sz;
		constexpr auto arrow_padding = .5f * arrow_radius;
		
		sf::CircleShape up(arrow_radius, 3);
		up.setFillColor(txt_color);
		up.setOrigin(-arrow_padding, -arrow_padding);
		up.setPosition(
			atpage_box.getPosition() + 
			sf::Vector2f(arrow_padding, 2.f)
		);

		window.draw(up);

		// Down arrow right next to the up arrow.
		sf::CircleShape down(up);
		down.scale(1.f, -1.f);
		down.move(2.f * arrow_radius, 2.5f * arrow_radius);
		window.draw(down);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
auto 
Menu<T>::find(const T id) -> decltype(m_options.begin())
{
	auto it = std::find_if(m_options.begin(), m_options.end(),
		[id](const auto& option) {
			[[maybe_unused]] const auto& [id_i, UNUSED1_, UNUSED2_] = option;
			return id_i == id;
		}
	);

	return it;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
typename Menu<T>::ctor_args 
Menu<T>::parseFile(const std::string& file)
{
	using json = nlohmann::json;
	
	// Load json file.
	std::ifstream ifs(file);
	BOOST_ASSERT(ifs.is_open());
	json js;
	ifs >> js;

	// Populate constructor arguments struct
	ctor_args args = {};
	constexpr auto position = "position";
	constexpr auto dimensions = "dimensions";
	constexpr auto margins = "margins";
	constexpr auto horizontal = "horizontal";
	constexpr auto vertical = "vertical";
	constexpr auto options = "options";
	constexpr auto cursor = "cursor";
	constexpr auto box = "box";
	constexpr auto colors = "colors";
	constexpr auto text = "text";
	constexpr auto background = "background";
	constexpr auto border = "border";

	try {
		args.pos = {
			js.at(position).at("x"), 
			js.at(position).at("y")
		};
	}
	catch (json::out_of_range& e) {
		LOG_DEBUG("[" << position << "]: " << e.what() << ".");
	}
		
	try {
		args.dim = { 
			js.at(dimensions).at("width"), 
			js.at(dimensions).at("height") 
		};
	}
	catch (json::out_of_range& e) {
		LOG_DEBUG("[" << dimensions << "]: " << e.what() << ".");
	}
		
	try {
		args.outer_margins = {
			js.at(box).at(margins).at(horizontal),
			js.at(box).at(margins).at(vertical)
		};
	}
	catch (json::out_of_range& e) {
		LOG_DEBUG("[" << box << "][" << margins << "]: " << e.what() << ".");
	}

	try {
		args.inner_margins = {
			js.at(options).at(margins).at(horizontal),
			js.at(options).at(margins).at(vertical)
		};
	}
	catch (json::out_of_range& e) {
		LOG_DEBUG("[" << options << "][" << margins << "]: " << e.what() << ".");
	}
		
	try {
		args.rows         = js.at(options).at("rows");
		args.cols         = js.at(options).at("columns");
		args.align_center = js.at(options).at("center");
		args.char_sz      = js.at(options).at("size");
	}
	catch (json::out_of_range& e) {
		LOG_DEBUG("[" << options << "]: " << e.what() << ".");
	}

	try {
		args.font_file = js.at("font");
	}
	catch (json::out_of_range& e) {
		LOG_DEBUG("[font]: " << e.what() << ".");
	}
	
	try {
		args.option_color = {
			args.makeColor(js.at(options).at(colors).at(text)),
			args.makeColor(js.at(options).at(colors).at(background)),
			args.makeColor(js.at(options).at(colors).at(border))
		};
	}
	catch (json::out_of_range& e) {
		LOG_DEBUG("[" << options << "][" << colors << "]: " << e.what() << ".");
	}

	try {
		args.cursor_color = {
			args.makeColor(js.at(cursor).at(colors).at(text)),
			args.makeColor(js.at(cursor).at(colors).at(background)),
			args.makeColor(js.at(cursor).at(colors).at(border))
		};
	}
	catch (json::out_of_range& e) {
		LOG_DEBUG("[" << cursor << "][" << colors << "]: " << e.what() << ".");
	}

	try {
		args.box_color = {
			args.makeColor(js.at(box).at(colors).at(background)),
			args.makeColor(js.at(box).at(colors).at(border))
		};
	}
	catch (json::out_of_range& e) {
		LOG_DEBUG("[" << box << "][" << colors << "]: " << e.what() << ".");
	}

	return args;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
sf::Color 
Menu<T>::ctor_args::makeColor(const std::vector<int>& rgba)
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