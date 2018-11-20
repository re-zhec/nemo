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
	{
		return std::make_pair(idx / ncols, idx % ncols);
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

Menu::Menu(const float x, const float y, const float width, const float height, 
	const size_t rows, const size_t cols, const size_t char_sz)
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
	
	// Create rectangular box for the menu's background
	, m_box(sf::Vector2f(width, height))
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

	m_box.setPosition(m_x, m_y);
	m_box.setFillColor(sf::Color(20, 20, 20));
	m_box.setOutlineColor(sf::Color(255, 255, 255));
	m_box.setOutlineThickness(2.f);
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
		option.setFillColor(sf::Color::Red);
		m_sel_rc = {0, 0};
	}
	else {
		option.setFillColor(sf::Color::White);
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

void Menu::delOption(const MenuOptionKey key, const std::string& txt, 
	const uint32_t id)
{
	// Search for the option
	auto it = findOption(key, id, txt);
	assert(it != m_options.cend());

	// Delete it
	it = m_options.erase(it);
	const auto rm_idx = it - m_options.cbegin();

	// The highlight cursor needs to be readjusted if it was on or after the 
	// option that was removed
	if (const auto cur_idx = translateTo1DIndex(m_sel_rc, m_cols);
		cur_idx >= rm_idx)
	{
		// Move the cursor if it was on the last option when that was removed. 
		// There is nothing left at its current coordinate
		if (cur_idx == m_options.size()) {
			// moveLeft() already accounts for moving left in a vertical menu
			moveLeft();
		}
		else {
			// Otherwise, the cursor would just be pointing at the option following 
			// the removed one. That option still has the nonselected color, so 
			// change its color
			setOptionColor(cur_idx, sf::Color::Red);
		}
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

std::pair<MenuOptionKey, uint32_t> Menu::getHoveredOption() const
{
	const auto idx = translateTo1DIndex(m_sel_rc, m_cols);
	[[maybe_unused]] const auto& [key, UNUSED1_, id] = m_options[idx];
	return {key, id};
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
	// Draw background box
	window.draw(m_box);

	// The entire list of menu options aren't going to be drawned into the render
	// window, only the "frame" that has the currently highlighted menu option. 
	// A frame is the maximum number of menu options that can be displayed at 
	// once based on the maximum rows and columns in the menu's construction
	const auto frame_sz = m_rows * m_cols;
	const auto idx = translateTo1DIndex(m_sel_rc, m_cols);

	// Getting the index of the first option of the frame the option highlighted 
	// by the cursor is in.
	const auto start = idx / frame_sz * frame_sz;

	// The last frame doesn't necessarily have all the rows and columns filled, 
	// so cap at the last option if the highlighted menu option is in there
	const auto end = std::min(start + frame_sz, m_options.size());

	// Draw the options' text
	for (auto i = start; i < end; ++i) {
		[[maybe_unused]] const auto& [UNUSED0_, txt, UNUSED2_] = m_options[i];
		window.draw(txt);
	}

	// Also write which frame number out the total number the cursor is in the 
	// menu so that the player knows where they are
	const auto cur_frame = idx / frame_sz + 1;
	const auto nframes = m_options.size() / frame_sz + 1;
	const auto frame_txt = std::to_string(cur_frame) + "/" + 
		std::to_string(nframes);
	
	sf::Text frame_gtxt(frame_txt, m_font, m_char_sz);
	frame_gtxt.setOrigin(
		m_width / m_height * static_cast<float>(m_char_sz), 
		2.f * static_cast<float>(m_char_sz)
	);
	frame_gtxt.setPosition(m_x + m_width, m_y + m_height);

	window.draw(frame_gtxt);

	// Draw scroller arrows if there is more than one frame
	if (nframes > 1) {
		if (cur_frame < nframes) {
			sf::CircleShape triangle(m_char_sz, 3);
			frame_gtxt.setPosition(m_x + m_width - m_char_sz, m_y + m_height);

		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::presetOptionPosition(size_t idx)
{
	assert(idx < m_options.size());
	[[maybe_unused]] auto& [UNUSED0_, txt, UNUSED2_] = m_options[idx];

	// Adjust relative spacing	
	txt.setOrigin(-m_option_width / 4.f, -m_option_height / 4.f);

	// Menu options are positioned from left to right down across rows. For every
	// "frame" of options (maximum number of rows x columns displayable options), 
	// start over from the top
	txt.setPosition(
		m_x + static_cast<decltype(m_x)>(idx % m_cols) * m_option_width,
		m_y + static_cast<decltype(m_y)>(idx / m_cols % m_rows) * m_option_height
	);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void Menu::move(const Direction dir)
{	
	// Change the current menu option to the non-selected color before moving to
	// the next option
	auto& [r, c] = m_sel_rc;
	setOptionColor(r, c, sf::Color::White);

	// For changing the current row or column...

	// The highlight cursor should be able to wrap around the ends of the menu.
	// Moving left when the cursor is at the leftmost option should take the 
	// cursor to the rightmost option at the same row, and vice versa. Similarly,
	// moving up when the cursor is at the topmost option should take the cursor 
	// to the bottomost option at the same column, and vice versa. The wrapping 
	// should take into account that the bottomost row could be partially filled.

	// Get the current number of rows that the options make up
	const auto n = m_options.size();
	const auto rows_touched = (n - 1) / m_cols + 1;

	// Get the number of columns that are filled in the last row
	const auto cols_filled = n % m_cols != 0 ? n % m_cols : m_cols;

	// Determine the bottommost row and rightmost column numbers from the
	// cursor's current location
	const auto bottom = c >= cols_filled && rows_touched > 1 
		? rows_touched - 2 
		: rows_touched - 1;

	const auto right = r < rows_touched - 1
		? m_cols - 1
		: cols_filled - 1;

	switch (dir) {
		// Up/down changes the row index
		case Direction::Up:
			r = r > 0 ? r - 1 : bottom;
			break;
		case Direction::Down:
			r = r < bottom ? r + 1 : 0;
			break;
		// Left/right changes the column index
		case Direction::Right:
			c = c < right ? c + 1 : 0;
			break;
		case Direction::Left:
			c = c > 0 ? c - 1 : right;
			break;
		default:
			break;
	}

	// Highlight the menu option the cursor just moved to
	setOptionColor(r, c, sf::Color::Red);	
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