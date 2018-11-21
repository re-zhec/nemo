#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <optional>
#include <SFML/Graphics.hpp>

namespace fe
{

/**
 * \brief Graphical menu
 *
 * Calling the constructor creates an empty menu. The client must add options 
 * to the menu via \property addOption. The menu isn't displayed on the render 
 * window until \property Draw is called.
 * 
 * Menu options's IDs are templatized. This design is to help with compiler 
 * warnings/errors if the client accidentally two options from two different 
 * menus, given that the IDs are scoped enums, etc..
 */
template <typename T> 
class Menu
{
public:
	/**
	 * \brief Constructs an empty menu
	 * 
	 * Upon construction, the menu will be empty. The client must add options to 
	 * the menu before drawing it on the render window; otherwise, the menu 
	 * would be displayed as an empty box. The first menu option added will 
	 * always be the default option to be highlighted for selection on the 
	 * window. If the client doesn't populate all of the displayable option 
	 * slots, then there will be empty space for the unfilled menu options where 
	 * text would normally be.
	 * 
	 * This constructor generates an assert error if at least one of the 
	 * positions is negative and if the number of rows or columns is zero or 
	 * negative.
	 * 
	 * \param xy			Top left xy coordinate for the menu in render window
	 * \param dim			Overall width
	 * \param height		Overall height	
	 * \param rows			Maximum rows of menu options that to be displayed 
	 * \param cols			Maximum columns of menu options that to be displayed
	 * \param char_sz			[Optional] Character size.
	 * \param option_color	[Optional] Menu option text and box outline's color
	 * \param backgd_color	[Optional] Menu background color
	 * \param cursor_color	[Optional] Selection cursor and page number's color
	 */
	Menu(
		const std::pair<float, float> xy,
		const float width,
		const float height,
		const size_t rows, 
		const size_t cols, 
		const size_t char_sz = 22, 
		const sf::Color option_color = {5, 255, 5},
		const sf::Color backgd_color = {25, 25, 25},
		const sf::Color cursor_color = {240, 240, 240}
	);

	/**
	 * \brief Add an option to the menu
	 * 
	 * The client must specify a unique ID for every menu option added. If a new 
	 * option shares \a id with one of the existing ones in this menu, then this 
	 * method generates an assertion error.
	 * 
	 * \param id		Menu option's ID
	 * \param txt		Text to display
	 */
	void addOption(const T id, const std::string& txt);

	/**
	 * \brief Remove an option from the menu
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, then this method generates an assertion error.
	 * 
	 * \param id		Menu option's ID
	 */
	void delOption(const T id);

	/**
	 * \brief Checks if the menu is empty
	 *
	 * \return True if yes; false otherwise
	 */
	bool empty() const noexcept;

	/**
	 * \brief Change an option's text
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, then this method generates an assertion error.
	 * 
	 * \param key		Menu option's ID
	 * \param txt		New text to display
	 */
	void setOptionText(const T id, const std::string& txt);

	/**
	 * \brief Change an option's color
	 * 
	 * The public version of this method.
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, then this method generates an assertion error.
	 * 
	 * \param key		Menu option's ID
	 * \param color	New color
	 */
	void setOptionColor(const T id, const sf::Color color);
	
	/**
	 * moveUp(), moveDown(), moveLeft(), and moveRight() move the highlight 
	 * cursor to the next menu option determined by which method is called. 
	 * The menu option that the highlight cursor is over will be colored in 
	 * red. The others will be colored in white.
	 */

	/**
	 * \brief Move cursor to the menu option above the current one
	 */
	void moveUp();

	/**
	 * \brief Move cursor to the menu option below the current one
	 */
	void moveDown();

	/**
	 * \brief Move cursor to the menu option to the right of the current one
	 */
	void moveRight();

	/**
	 * \brief Move cursor to the menu option to the left of the current one
	 */
	void moveLeft();

	/**
	 * \brief Draws the menu on the render window.
	 * 
	 * This method should be called during every iteration of the game loop to 
	 * keep the menu displayed.
	 * 
	 * \param window		Window for the graphical menu to be rendered on
	 */
	void draw(sf::RenderWindow& window) const;

	/**
	 * \brief Get the option that the highlight cursor is currently on
	 * 
	 * Use this method if the player selects an option on the menu to find out 
	 * what the player selected.
	 * 
	 * \return The menu option that the selection cursor is on via its ID, or 
	 * nothing if the menu is empty
	 */
	std::optional<T> getHoveredOption() const noexcept;

private:
	/**
	 * \brief Enumeration for which direction the player moves the cusor to in 
	 * the menu.
	 * 
	 * This is used only by the \property Move method, which \property moveUp, 
	 * \property moveDown, \property moveLeft, and \property moveRight wraps 
	 * around.
	 */ 
	enum class Direction {
		Up,
		Down,
		Right,
		Left
	};

	/////////////////////////////////////////////////////////
	// Member data
	/////////////////////////////////////////////////////////
	float m_x;			///< Starting x position relative to render window
	float m_y;			///< Starting y position relative to render window
	float m_width;		///< Overall width
	float m_height;	///< Overall height
	size_t m_rows;		///< Maximum rows of menu options to be displayed
	size_t m_cols;		///< Maximum columns of menu options to be displayed
	size_t m_char_sz;	///< Character size

	float m_option_width;		///< Width of each menu option
	float m_option_height;		///< Height of each menu option

	///< Color for the menu option text and the menu box outline
	sf::Color m_option_color;

	///< Row and column coordinate of the menu's selection cursor
	std::pair<size_t, size_t> m_sel_rc;

	///< Color of the menu option the selection cursor is on as well as the 
	// current menu page number
	sf::Color m_cursor_color;

	///< Menu options. The first component is their IDs. The second is their
	// graphical text.
	std::vector<std::tuple<T, sf::Text>> m_options;

	sf::Color m_backgd_color;		///< Menu background color
	sf::RectangleShape m_backgd;	///< Rectangle that forms the menu's background
	sf::Font m_font;					///< Font used

	/**
	 * \brief Set where a menu option should be drawn on the render window
	 * 
	 * The exact position is based on its index in the menu option container. In 
	 * the render window, they are positioned from left to right, down across 
	 * rows. This or the other version should be called for every new menu option 
	 * added or for an option that needs to be shifted forward due to the removal 
	 * of a preceding option.
	 * 
	 * The menu option container performs an out-of-bound check with the 
	 * index argument
	 *  
	 * \param idx		0th-based index of the option in the menu option vector
	 * 					that needs to its position preset (again)
	 */
	void presetOptionPosition(const size_t idx);

	/**
	 * \brief Move the highlight cursor along menu options
	 * 
	 * This method does the main work for \property moveUp, \property moveDown, 
	 * \property moveLeft, and \property moveRight, which all give this method 
	 * the directional enumeration value as the argument.
	 *  
	 * \param dir		Direction to move to
	 */
	void move(const Direction dir);

	/**
	 * \brief Change an option's color
	 * 
	 * A private version of this method.
	 * 
	 * This is a wrapper around the below version. It translates the given row 
	 * and column number to an 1-D index to pass to the other version. A row and
	 * column coordinate that is outside the menu option range will cause an 
	 * assertion error
	 * 
	 * \param r			Option's row number
	 * \param c			Option's Column number
	 * \param color	New color
	 */
	void setOptionColor(const size_t r, const size_t c, const sf::Color color);

	/**
	 * \brief Change an option's color
	 * 
	 * A private version of this method.
	 * 
	 * This one takes an 1-D index instead of a row and column numbers. An 
	 * out-of-bound index will cause an assertion error.
	 * 
	 * \param idx		1-D index corresponding to the option's row and column 
	 * 					coordinate
	 * \param color	New color
	 */
	void setOptionColor(const size_t idx, const sf::Color color);

	/**
	 * \brief Search for an option
	 * 
	 * This method finds the option that matches \a id. It SHOULDN'T find more 
	 * than one since \property addOption disallows multiple menu options having 
	 * the same ID, but if for some reason there is, it grabs the first 
	 * occurrence.
	 * 
	 * \param id		Menu option's ID
	 * 
	 * \return Non-const iterator to the menu option found. If there is no match, 
	 * \var m_options.end() is returned
	 */
	auto findOption(const T id) -> decltype(m_options.begin());
};

}