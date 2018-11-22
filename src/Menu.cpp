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
	 * \brief For data that is represented as a 2D matrix but is actually 1-D and 
	 * contiguous, this function converts the row and column coordinate to its 
	 * 1-D counterpart
	 * 
	 * This function is used to get the index of a particular menu option, as 
	 * they are stored in a single vector despite being displayed as a 2D matrix
	 *
	 * \param r			Row number
	 * \param c			Column number
	 * \param ncols	Number of columns per row
	 * 
	 * \return 1-D index for array, vector, etc.
	 */
	inline decltype(auto) 
	translateTo1DIndex(const size_t r, const size_t c, const size_t ncols)
	noexcept
	{
		// Indices are numbered from left to right, going downward
		return r * ncols + c;
	}

	/**
	 * \brief For data that is represented as a 2D matrix but is actually 1-D and 
	 * contiguous, this function converts the row and column coordinate to its 
	 * 1-D counterpart
	 * 
	 * This method is a wrapper for the method above, which takes the split row 
	 * and column number as input instead of the pair
	 *
	 * \param rc		Row and column coordinate
	 * \param ncols	Number of columns per row
	 * 
	 * \return 1-D index for array, vector, etc.
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
	 * It converts an 1-D index to a corresponding row and column coordinate
	 *
	 * \param idx		1-D index for array, vector, etc.
	 * \param ncols	Number of columns per row
	 * 
	 * \return Row and column coordinate
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
	const float width,
	const float height,
	const size_t rows, 
	const size_t cols, 
	const size_t char_sz, 
	const sf::Color txt_color,
	const sf::Color cursor_color,
	const std::pair<sf::Color, sf::Color> cell_color,
	const std::pair<sf::Color, sf::Color> backgd_color
)	
	: m_x(xy.first)
	, m_y(xy.second)
	, m_width(width)
	, m_height(height)
	, m_rows(rows)
	, m_cols(cols)
	, m_char_sz(char_sz)

	// Menu options' width and height are scaled based on the menu's overall 
	// dimensions and the number of rows and columns of options to fit in at a 
	// time
	, m_option_width((width - .5f * char_sz) / cols)
	, m_option_height((height - 1.55f * char_sz) / rows)
	
	, m_option_color(txt_color)
	, m_cursor_color(cursor_color)

	// Create rectangular box for the menu's background
	, m_backgd(sf::Vector2f(width, height))
{
	assert(m_x >= 0.f);
	assert(m_y >= 0.f);
	assert(m_width >= 0.f);
	assert(m_height >= 0.f);
	assert(m_rows > 0);
	assert(m_cols > 0);

	// Load font from file
	const auto success = m_font.loadFromFile(
		"font/Montserrat/Montserrat-Regular.ttf"
	);

	assert(success);

	// Reserve some number of options to ease the number of forced memory 
	// reallocations
	const auto noptions_per_page = rows * cols;
	m_options.reserve(noptions_per_page);
	
	// Create background cells for one page full of menu options. Only one page
	// is needed since the options outside of them won't be drawn.
	m_cells.reserve(noptions_per_page);

	for (auto i = 0U; i < noptions_per_page; ++i) {
		// Tweak cell size so that margins can be inserted between them
		const auto margin_width = .05f * m_option_width;
		const auto margin_height = .1f * m_option_height;
		
		sf::RectangleShape cell({
			m_option_width - margin_width, 
			m_option_height - margin_height
		});

		// Cell colors
		const auto& [cell_fill, cell_oline] = cell_color;
		cell.setFillColor(cell_fill);
		cell.setOutlineColor(cell_oline);
		cell.setOutlineThickness(1.f);

		// To insert margins between cells
		cell.setOrigin(-margin_width, -margin_height);

		// Position cells from left to right, downward across rows
		const auto [r_i, c_i] = translateToRowColumn(i, m_cols);
		cell.setPosition(
			m_x + m_option_width * c_i, 
			m_y + m_option_height * r_i
		);

		m_cells.push_back(cell);
	}

	// Color for the entire menu background
	const auto& [backgd_fill, backgd_oline] = backgd_color;
	m_backgd.setFillColor(backgd_fill);
	m_backgd.setOutlineColor(backgd_oline);
	m_backgd.setOutlineThickness(-2.f);

	// Starting position of the menu
	m_backgd.setPosition(m_x, m_y);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::addOption(const T id, const std::string& txt)
{
	// Make sure there's no other menu options with the new ID
	const auto it = findOption(id);
	assert(it == m_options.cend());

	// Create graphical text
	sf::Text option(txt, m_font, m_char_sz);

	if (m_options.empty()) {
		// Selection cursor is on the first option by default
		option.setFillColor(m_cursor_color);
		m_sel_rc = {0, 0};
	}
	else {
		option.setFillColor(m_option_color);
	}

	// Add the option to the menu
	m_options.push_back({id, option});
	LOG_DEBUG("Added menu option {" << id << "," << txt << "}");

	// Preset its window position on the menu for future rendering. Since it was 
	// just added to menu, use the index of the last element in the menu option 
	// container
	presetOptionPosition(m_options.size() - 1);
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
	// Search for the option
	auto it = findOption(id);
	assert(it != m_options.cend());

	// Delete it
	it = m_options.erase(it);
	const auto rm_idx = static_cast<decltype(m_options.size())>(
		it - m_options.cbegin());

	// Ignore the rest of the method if that removed option was the only one 
	// left on the menu. The cursor will correct itself once another option is 
	// added
	if (m_options.empty()) {
		return;
	}

	// The highlight cursor needs to be readjusted if it was on or after the 
	// option that was removed
	if (auto cur_idx = translateTo1DIndex(m_sel_rc, m_cols);
		cur_idx >= rm_idx)
	{
		// Move the cursor back if it was on the last option when that was 
		// removed. There is nothing left at its current coordinate
		if (cur_idx == m_options.size()) {
			// Since there's nothing there, none of the other menu movement methods
			// can be called because they change the color of the option  the 
			// cursor was on. The cursor's coordinates need to be manually changed 
			// to the preceding menu option's
			--cur_idx;
			m_sel_rc = translateToRowColumn(cur_idx, m_cols);
		}
		
		// Otherwise, the cursor would just be pointing at the option following 
		// the removed one. Either way, The option still has the nonselected 
		// color, so change that
		setOptionColor(cur_idx, m_cursor_color);
	}

	// All options that followed the removed one need to have their render 
	// positions shifted forward one slot
	for (auto i = rm_idx; i < m_options.size(); ++i) {
		presetOptionPosition(i);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::setOptionText(const T id, const std::string& txt)
{
	// Search for the option
	const auto it = findOption(id);
	assert(it != m_options.cend());
	[[maybe_unused]] auto& [UNUSED_, cur_txt] = *it;

	// Change to new text
	cur_txt.setString(txt);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::setOptionColor(const T id, const sf::Color color)
{
	// Search for the option
	const auto it = findOption(id);
	assert(it != m_options.cend());

	// Change its color
	setOptionColor(it - m_options.cbegin(), color);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::optional<T> Menu<T>::getHoveredOption() const noexcept
{
	if (m_options.empty()) {
		// Empty menu
		return {};
	}

	const auto idx = translateTo1DIndex(m_sel_rc, m_cols);
	[[maybe_unused]] const auto& [id, UNUSED_] = m_options[idx];
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
		// whether last menu option is on the first row
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
		// Down => right in a horizontal menu, which can be determined based on 
		// whether last menu option is on the first row
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
		// Right => down in a vertical menu
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
		// Left => up in a vertical menu
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
void Menu<T>::draw(sf::RenderWindow& window) const
{
	// Draw background
	window.draw(m_backgd);

	// Drawing the menu options...

	// The entire list isn't going to be drawned into the render window. Only the 
	// "page" that has the menu option the highlight cursor is on needs to be. A
	// page is the number of menu options that can be displayed at once based on
	// the specified number of rows and columns in the menu's construction
	const auto page_sz = m_rows * m_cols;
	const auto idx = translateTo1DIndex(m_sel_rc, m_cols);
	const auto cur_page = idx / page_sz;

	// Find the first and last menu option that would appear in the highlighed 
	// option's frame. In the case of this frame being the last one, it doesn't 
	// necessarily have all the rows and columns filled, so cap at the last 
	// option index
	const auto start = cur_page * page_sz;
	const auto n = m_options.size();
	const auto end = std::min(start + page_sz, n);

	// Draw the options
	for (auto i = start; i < end; ++i) {
		window.draw(m_cells[i - start]);
		[[maybe_unused]] const auto& [UNUSED_, txt] = m_options[i];
		window.draw(txt);
	}

	if (const auto npages = (n - 1) / page_sz + 1;
		npages > 1) 
	{
		// Draw the current page number out of the total so that the player knows 
		// where they are
		const auto page_txt = std::to_string(cur_page + 1) + "/" + 
			std::to_string(npages);		
		const auto page_txt_sz = page_txt.length() * m_char_sz;
		const auto page_char_sz = .9f * m_char_sz;
		
		// Place them at the bottom right of the displayable portion of the menu
		const auto botright_x = m_x + m_width - page_txt_sz;
		const auto botright_y = m_y + m_height - page_char_sz;

		sf::Text page_mark(page_txt, m_font, page_char_sz);
		page_mark.setFillColor(m_option_color);
		page_mark.setOrigin(0.f, .45f * m_char_sz);
		page_mark.setPosition(botright_x, botright_y);

		// Draw scroll arrows
		const auto radius = .5f * page_char_sz;

		// Up arrow
		sf::CircleShape up(radius, 3);
		up.setFillColor(m_option_color);
		up.setOrigin(radius / 2.f, radius / 2.f);
		up.setPosition(botright_x - 3.25f * page_char_sz, botright_y);
		
		// Down arrow
		sf::CircleShape down(up);
		down.scale(1.f, -1.f);
		down.setPosition(
			botright_x - 2.f * page_char_sz, 
			botright_y + radius / 2.f
		);

		window.draw(page_mark);
		window.draw(up);
		window.draw(down);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::presetOptionPosition(const size_t idx)
{
	assert(idx < m_options.size());

	[[maybe_unused]] auto& [UNUSED_, txt] = m_options[idx];
	const auto [r, c] = translateToRowColumn(idx, m_cols);

	// Veritcally align each
	const auto margin_width = .1f * m_option_width;
	const auto margin_height = .5f * (m_option_height - m_char_sz);
	
	txt.setOrigin(-margin_width, -margin_height);

	// Menu options are positioned from left to right down across rows. For every
	// "page" of options (maximum number of rows x columns displayable options), 
	// start over from the top.
	txt.setPosition(
		m_x + m_option_width * c,
		m_y + m_option_height * (r % m_rows)
	);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::move(const Direction dir)
{
	if (m_options.empty()) {
		// No menu options => no cursor => no movement
		return;
	}

	// Change the current menu option to the non-selected color before moving to
	// the next option
	auto& [r, c] = m_sel_rc;
	setOptionColor(r, c, m_option_color);

	// Changing the current row or column...

	// The highlight cursor should be able to wrap around the ends of the menu.
	// Moving left when the cursor is at the leftmost option should take the 
	// cursor to the rightmost option at the same row, and vice versa. Similarly,
	// moving up when the cursor is at the topmost option should take the cursor 
	// to the bottomost option at the same column, and vice versa. The wrapping 
	// should take into account that the bottomost row may be partially filled.

	// Get the row and column coordinate of the last option
	const auto [last_r, last_c] = translateToRowColumn(
		m_options.size() - 1, m_cols
	);

	// Rightmost column at the current row the cursor is on. Needed for moving 
	// left and right
	const auto right_c = r < last_r ? m_cols - 1 : last_c;

	switch (dir) {
		// Up/down changes the row index.
		// If the cursor will move to the bottom row but there's no option exactly 
		// below it, move it to the last option
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

	// Highlight the menu option the cursor just moved to
	setOptionColor(r, c, m_cursor_color);	
	LOG_DEBUG("New cursor location: (" << r << "," << c << ")");
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::setOptionColor(const size_t idx, const sf::Color color)
{
	assert(idx < m_options.size());
	[[maybe_unused]] auto& [UNUSED_, txt] = m_options[idx];
	txt.setFillColor(color);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Menu<T>::setOptionColor(const size_t r, const size_t c, 
	const sf::Color color)
{
	const auto idx = translateTo1DIndex(r, c, m_cols);
	setOptionColor(idx, color);
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
			[[maybe_unused]] const auto& [id_i, UNUSED_] = option;
			return id_i == id;
		}
	);

	return it;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}