#include <algorithm>
#include <cassert>
#include "../include/logger.hpp"
#include "../include/Menu.hpp"

namespace fe
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
	inline decltype(auto) 
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
	inline decltype(auto) 
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
	inline decltype(auto)
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
	const std::pair<float, float> xy,
	const std::pair<float, float> dim,
	const size_t rows, 
	const size_t cols,

	const std::tuple<sf::Color, sf::Color, sf::Color> option_color,
	const std::tuple<sf::Color, sf::Color, sf::Color> cursor_color,
	const std::pair<sf::Color, sf::Color> box_color,
	const std::pair<float, float> outer_margins,
	const std::pair<float, float> inner_margins,
	const std::string font_file,
	const size_t char_sz
)	
	: m_xy(xy)
	, m_dim(dim)
	, m_page_margins(outer_margins)
	, m_rows(rows)
	, m_cols(cols)
	, m_option_color(option_color)
	, m_cursor_color(cursor_color)
	, m_char_sz(char_sz)	
{
	assert(m_xy >= std::make_pair(0.f, 0.f));
	assert(m_dim >= std::make_pair(0.f, 0.f));
	assert(m_rows > 0);
	assert(m_cols > 0);

	// Load the font file.
	const auto success = m_font.loadFromFile(font_file);
	assert(success);

	// Create the menu box.
	const auto [x, y] = m_xy;
	const auto [width, height] = m_dim;
	const auto& [box_back_color, box_bord_color] = box_color;

	m_box.setSize({width, height});
	m_box.setFillColor(box_back_color);
	m_box.setOutlineColor(box_bord_color);
	m_box.setOutlineThickness(-1.f);
	m_box.setPosition(x, y);

	// The area of the menu inside the margins are reserved for menu options. 
	// From there, the number of rows and columns of options determine each 
	// option's width and height. For now, inner margins are included in the 
	// width and height.
	const auto [hzmargin, vtmargin] = m_page_margins;
	const auto option_width = (width - 2.f * hzmargin) / m_cols;
	const auto option_height = (height - 2.f * vtmargin) / m_rows;

	// Reserve some number of options to ease the number of forced memory 
	// reallocations.
	const auto noptions_per_page = m_rows * m_cols;
	m_options.reserve(noptions_per_page);
	
	// Create background cells to contain the menu options in one page. To save 
	// memory, only one page worth is needed since the options outside of them 
	// won't be drawn on screen. We can keep track of what the color of a cell 
	// should be based on the contained option's metadata.
	m_cells.reserve(noptions_per_page);

	for (auto i = 0u; i < noptions_per_page; ++i) {
		// Adjust cell cize so that inner margins can be inserted between them.
		const auto [inner_hzmargin, inner_vtmargin] = inner_margins;
		
		sf::RectangleShape cell({
			option_width - 2.f * inner_hzmargin, 
			option_height - 2.f * inner_vtmargin
		});

		// Insert inner margins.
		cell.setOrigin(-inner_hzmargin, -inner_vtmargin);

		// Place the cell in the appropriate spot in the menu. Cells line up from 
		// left to right, down across rows.
		const auto [r_i, c_i] = translateToRowColumn(i, m_cols);
		cell.setPosition(
			x + hzmargin + option_width * c_i, 
			y + vtmargin + option_height * r_i
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
void Menu<T>::addOption(const T id, const std::string& txt)
{
	// Make sure there is no other menu option that has the new ID.
	const auto it = findOption(id);
	assert(it == m_options.cend());

	// Create the option's graphical text.
	sf::Text option(txt, m_font, m_char_sz);
	
	// Add the option to the menu.
	m_options.push_back({id, option, m_option_color});
	const auto idx = m_options.size() - 1;

	if (idx == 0) {
		// The cursor starts on the first option by default.
		m_cursor_rc = translateToRowColumn(idx, m_cols);		
		setOptionColor(idx, m_cursor_color);
	}

	// Preset the option text's position on the menu for future rendering. Since 
	// it was just added to menu, we can use the index of the last element in the
	// menu option container.
	presetTextPosition(idx);
	LOG_DEBUG("Added menu option {" << id << "," << txt << "}");
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool Menu<T>::empty() const noexcept
{
	return m_options.empty();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::delOption(const T id)
{
	// Search for the option.
	auto it = findOption(id);
	assert(it != m_options.cend());

	// Delete it.
	it = m_options.erase(it);
	const auto rm_idx = static_cast<decltype(m_options.size())>(
		it - m_options.cbegin());

	// Ignore the rest of the method if that removed option was the only one 
	// left on the menu. The cursor will correct itself once another option is 
	// added.
	if (m_options.empty()) {
		return;
	}

	// All options that followed the removed one need to have their text's render 
	// positions shifted frontward one slot
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
			// None of the other menu movement methods (moveUp(), moveDown(), etc.) 
			// can be called because they change the colors of the option the 
			// cursor was over, and there's nothing there. The cursor's position 
			// need to be manually changed to the preceding menu option's.
			--cur_idx;
			m_cursor_rc = translateToRowColumn(cur_idx, m_cols);
		}
		
		// Otherwise, the cursor would just be pointing at the option following 
		// the removed one. Either way, The option still has the non-cursor
		// colors, so change that.
		setOptionColor(cur_idx, m_cursor_color);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::setOptionText(const T id, const std::string& txt)
{
	// Search for the option.
	const auto it = findOption(id);
	assert(it != m_options.cend());
	[[maybe_unused]] auto& [UNUSED0_, cur_txt, UNUSED2_] = *it;

	// Change to new text.
	cur_txt.setString(txt);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::setOptionColor(const T id, const sf_color3 color)
{
	// Search for the option.
	const auto it = findOption(id);
	assert(it != m_options.cend());

	// Change its colors.
	setOptionColor(it - m_options.cbegin(), color);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::optional<T> Menu<T>::getHoveredOption() const noexcept
{
	if (m_options.empty()) {
		// Empty menu.
		return {};
	}

	const auto idx = translateTo1DIndex(m_cursor_rc, m_cols);
	[[maybe_unused]] const auto& [id, UNUSED1_, UNUSED2_] = m_options[idx];
	return std::make_optional(id);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::moveUp()
{
	if ([[maybe_unused]] const auto [last_r, UNUSED_] = translateToRowColumn(
			m_options.size() - 1, m_cols
		);
		last_r == 0) 
	{
		// Up => left in a horizontal menu, which can be determined based on 
		// whether last menu option is on the first row.
		move(Direction::Left);
	}
	else {
		move(Direction::Up);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::moveDown()
{
	if ([[maybe_unused]] const auto [last_r, UNUSED_] = translateToRowColumn(
			m_options.size() - 1, m_cols
		);
		last_r == 0)
	{
		// Down => right in a horizontal menu.
		move(Direction::Right);
	} 
	else {
		move(Direction::Down);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::moveRight()
{
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
void Menu<T>::moveLeft()
{
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
void Menu<T>::draw(sf::RenderWindow& window)
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
void Menu<T>::presetTextPosition(const size_t idx)
{
	assert(idx < m_options.size());

	// Menu options are positioned from left to right down across rows. After 
	// a page is filled, the graphical positions start over from the top left 
	// for a new page.
	const auto& cell = m_cells[idx % m_cells.size()];
	[[maybe_unused]] auto& [UNUSED0_, txt, UNUSED2_] = m_options[idx];
	txt.setOrigin(cell.getOrigin());
	txt.setPosition(cell.getPosition());

	// Veritcally center and horizontally left-align this option in the cell it
	// it is placed in.
	const auto [width, height] = cell.getSize();
	const auto hzalign = .075f * width;
	const auto vtalign = .5f * (height - m_char_sz);
	txt.move(hzalign, vtalign);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::move(const Direction dir)
{
	if (m_options.empty()) {
		// No menu options => no cursor => no movement.
		return;
	}

	// Change the current menu option to the non-cursor colors before going to
	// the next option.
	auto& [r, c] = m_cursor_rc;
	setOptionColor(r, c, m_option_color);

	// Changing the current row or column...

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

	// Color the menu option the cursor just moved to.
	setOptionColor(r, c, m_cursor_color);	
	LOG_DEBUG("New cursor location: (" << r << "," << c << ")");
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::setOptionColor(const size_t idx, const sf_color3 color)
{
	assert(idx < m_options.size());
	[[maybe_unused]] auto& [UNUSED0_, UNUSED1_, cur_color] = m_options[idx];
	cur_color = color;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::setOptionColor(const size_t r, const size_t c, 
	const sf_color3 color)
{
	const auto idx = translateTo1DIndex(r, c, m_cols);
	setOptionColor(idx, color);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::drawOption(const size_t idx, sf::RenderWindow& window)
{
	assert(idx < m_options.size());

	// Although we can have pages of menu options, we have only a page worth of 
	// cells to use to save memory. So grab the one the menu option would be
	// drawn on.
	auto& cell = m_cells[idx % m_cells.size()];

	// Color the text and cell.
	[[maybe_unused]] auto& [UNUSED_, txt, color] = m_options[idx];
	const auto [txt_color, back_color, bord_color] = color;

	txt.setFillColor(txt_color);
	cell.setFillColor(back_color);
	cell.setOutlineColor(bord_color);

	// Draw the cell first, then the text over it.
	window.draw(cell);
	window.draw(txt);
}

template <typename T>
void Menu<T>::drawPageRef(sf::RenderWindow& window) const
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
	const auto atpage_box_height = 25.f;			
	const auto atpage_box_width = static_cast<float>(atpage_txt.length() 
		* m_char_sz);

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
	const auto atpage_txt_height = atpage_box_height - 9.f;
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
		const auto arrow_sz = atpage_box_height - 7.f;
		const auto arrow_radius = .5f * arrow_sz;
		const auto arrow_padding = .5f * arrow_radius;
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
auto Menu<T>::findOption(const T id) -> decltype(m_options.begin())
{
	// setOptionText() uses the returned iterator to modify the contents of the
	// object it points to, so normal iterators are needed instead of const ones.
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

}