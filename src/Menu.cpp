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

Menu::Menu(
	const float x, 
	const float y, 
	const float width, 
	const float height, 
	const size_t rows, 
	const size_t cols, 
	const size_t char_sz, 
	const sf::Color option_color, 
	const sf::Color backgd_color,
	const sf::Color cursor_color
)	
	: m_x(x)
	, m_y(y)
	, m_width(width)
	, m_height(height)
	, m_rows(rows)
	, m_cols(cols)
	, m_char_sz(char_sz)

	// Menu options' width and height are scaled based on the menu's overall 
	// dimensions and the number of rows and columns of options to fit in at a 
	// time
	, m_option_width(m_width / static_cast<decltype(m_width)>(m_cols))
	, m_option_height((m_height - static_cast<decltype(m_height)>(m_char_sz)) / 
			static_cast<decltype(m_height)>(m_rows))
	
	, m_option_color(option_color)
	, m_cursor_color(cursor_color)
	, m_backgd_color(backgd_color)

	// Create rectangular box for the menu's background
	, m_backgd(sf::Vector2f(width, height))
{
	assert(m_x >= 0.f);
	assert(m_y >= 0.f);
	assert(m_width >= 0.f);
	assert(m_height >= 0.f);
	assert(m_rows > 0);
	assert(m_cols > 0);

	// Reserve some number of options ease the number of forced memory 
	// reallocations
	m_options.reserve(m_rows * m_cols);

	// Load font from file
	const auto success = m_font.loadFromFile("font/EBGaramond-Regular.ttf");
	assert(success);

	m_backgd.setPosition(m_x, m_y);
	m_backgd.setFillColor(m_backgd_color);
	m_backgd.setOutlineColor(m_option_color);
	m_backgd.setOutlineThickness(-2.f);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::addOption(const MenuOptionKey key, const std::string& txt, 
	const uint32_t id)
{	
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
	m_options.push_back({key, option, id});
	LOG_DEBUG("Added menu option {" << key << "," << txt << "," << id);

	// Preset its window position on the menu for future rendering. Since it was 
	// just added to menu, use the index of the last element in the menu option 
	// container
	presetOptionPosition(m_options.size() - 1);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

bool Menu::empty() const noexcept
{
	return m_options.empty();
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::delOption(const MenuOptionKey key, const std::string& txt, 
	const uint32_t id)
{
	// Search for the option
	auto it = findOption(key, id, txt);
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

void Menu::setOptionText(const MenuOptionKey key, const std::string& txt, 
	const uint32_t id)
{
	// Search for the option
	const auto it = findOption(key, id);
	assert(it != m_options.cend());
	[[maybe_unused]] auto& [UNUSED0_, cur_txt, UNUSED2_] = *it;

	// Change to new text
	cur_txt.setString(txt);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::setOptionColor(const MenuOptionKey key, const std::string& txt, 
	const sf::Color color, const uint32_t id)
{
	// Search for the option
	const auto it = findOption(key, id, txt);
	assert(it != m_options.cend());

	// Change its color
	setOptionColor(it - m_options.cbegin(), color);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

std::optional<std::pair<MenuOptionKey, uint32_t>> 
Menu::getHoveredOption() const noexcept
{
	if (m_options.empty()) {
		// Empty menu
		return {};
	}

	const auto idx = translateTo1DIndex(m_sel_rc, m_cols);
	[[maybe_unused]] const auto& [key, UNUSED1_, id] = m_options[idx];
	return std::make_optional(std::make_pair(key, id));
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::moveUp()
{
	if (m_rows == 1 && m_options.size() < m_rows * m_cols) {
		// Up => left in a horizontal menu but only if there aren't more options 
		// besides what are shown on screen. Otherwise, without the second 
		// condition, the cursor would never get past the first page
		move(Direction::Left);
	}
	else {
		move(Direction::Up);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::moveDown()
{
	if (m_rows == 1 && m_options.size() < m_rows * m_cols) {
		// Down => right in a horizontal menu. Same second condition as moveUp()
		move(Direction::Right);
	} 
	else {
		move(Direction::Down);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::moveRight()
{
	if (m_cols == 1) {
		// Right => down in a vertical menu. Don't need check if there are more 
		// options besides what are shown on screen like moveUp() and moveDown() 
		// because the menu scrolls vertically, not horizontally. Any "overflow" 
		// would just allow the cursor to move down and up
		move(Direction::Down);
	} 
	else {
		move(Direction::Right);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::moveLeft()
{
	if (m_cols == 1) {
		// Left => up in a vertical menu. Don't need to check other conditions 
		// for same reason as moveRight()
		move(Direction::Up);
	} 
	else {
		move(Direction::Left);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::draw(sf::RenderWindow& window) const
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

	// Draw the options' text
	for (auto i = start; i < end; ++i) {
		[[maybe_unused]] const auto& [UNUSED0_, txt, UNUSED2_] = m_options[i];
		window.draw(txt);
	}

	// Draw the current page number out of the total so that the player knows
	// where they are.
	const auto npages = (n - 1) / page_sz + 1;
	const auto page_txt = std::to_string(cur_page + 1) + "/" + 
		std::to_string(npages);
	
	const auto char_sz = static_cast<decltype(m_x)>(m_char_sz);
	const auto page_txt_sz = static_cast<decltype(m_x)>(page_txt.length()) 
		* char_sz;
	
	// Place them at the bottom right of the displayable portion of the menu
	const auto botright_x = m_x + m_width - page_txt_sz;
	const auto botright_y = m_y + m_height - char_sz;

	sf::Text page_mark(page_txt, m_font, m_char_sz);
	page_mark.setOrigin(0, char_sz / 2.f);
	page_mark.setPosition(botright_x, botright_y);
	window.draw(page_mark);

	if (npages > 1) {
		// Draw scroll arrows
		const auto radius = char_sz / 2.5f;

		// Up arrow
		sf::CircleShape arrow(radius, 3);
		arrow.setOrigin(radius / 2.f, radius / 2.f);
		arrow.setPosition(botright_x - 3.25f * char_sz, botright_y);
		window.draw(arrow);
		
		// Flip and redraw for down arrow
		arrow.scale(1.f, -1.f);
		arrow.setPosition(botright_x - 2.f * char_sz, botright_y + radius / 2.f);
		window.draw(arrow);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::presetOptionPosition(const size_t idx)
{
	assert(idx < m_options.size());
	[[maybe_unused]] auto& [UNUSED0_, txt, UNUSED2_] = m_options[idx];
	LOG_DEBUG(m_option_width << "," << m_option_height);

	// Menu options are positioned from left to right down across rows. For every
	// "frame" of options (maximum number of rows x columns displayable options), 
	// start over from the top. Add 1/4 of each option's dimensions to their 
	// positions to center them somewhat
	txt.setPosition(
		m_x + (static_cast<decltype(m_x)>(idx % m_cols) + 0.25f) * 
			m_option_width,
		m_y + (static_cast<decltype(m_y)>(idx / m_cols % m_rows) + 0.25f) * 
			m_option_height
	);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::move(const Direction dir)
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

	// Get the bottomost row that the options make up its last column. This is 
	// needed for moving up and down
	const auto last = m_options.size() - 1;
	const auto bottom_r = last / m_cols;
	const auto bottom_c = last % m_cols;

	// Rightmost column at the current row the cursor is on. Needed for moving 
	// left and right
	const auto right_c = r < bottom_r ? m_cols - 1 : bottom_c;

	switch (dir) {
		// Up/down changes the row index.
		// If the cursor will move to the bottom row but there's no option exactly 
		// below it, move it to the last option
		case Direction::Up:
			r = r > 0 ? r - 1 : bottom_r;
			c = r < bottom_r ? c : std::min(c, bottom_c);
			break;
		case Direction::Down:
			r = r < bottom_r ? r + 1 : 0;
			c = r < bottom_r ? c : std::min(c, bottom_c);
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

void Menu::setOptionColor(const size_t idx, const sf::Color color)
{
	assert(idx < m_options.size());
	[[maybe_unused]] auto& [UNUSED0_, txt, UNUSED2_] = m_options[idx];
	txt.setFillColor(color);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::setOptionColor(const size_t r, const size_t c, 
	const sf::Color color)
{
	const auto idx = translateTo1DIndex(r, c, m_cols);
	setOptionColor(idx, color);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

auto Menu::findOption(const MenuOptionKey key, const uint32_t id, 
	const std::string& txt) 
		 -> decltype(m_options.begin())
{
	// setOptionText() uses the returned iterator to modify the contents of the
	// object it points to, so normal iterators are needed instead of const ones.
	auto it = std::find_if(m_options.begin(), m_options.end(),
		[key, &txt, id](const auto& option) {
			const auto& [key_i, txt_i, id_i] = option;
			auto match = key_i == key && id_i == id;
			
			// If text isn't empty, include it in the comparison
			if (!txt.empty()) {
				match &= txt_i.getString() == txt; 
			}

			return match;
		}
	);

	return it;
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}