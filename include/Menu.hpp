#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <optional>
#include <SFML/Graphics.hpp>

namespace fe
{

enum class MenuOptionKey {
	Play,
	Options,
	Exit
};

/**
 * \brief Graphical menu
 *
 * Calling the constructor creates an empty menu. The client must add options 
 * to the menu via \property addOption. The menu isn't displayed on the render 
 * window until \property Draw is called.
 */
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
	 * \param x				starting position along x-axis relative to render window
	 * \param y				starting position along y-axis relative to render window
	 * \param width		width
	 * \param height		height
	 * \param rows			maximum rows of menu options that to be displayed 
	 * \param cols			maximum columns of menu options that to be displayed
	 * \param char_sz		character size
	 */
	Menu(const float x, const float y, const float width, const float height, 
		const size_t rows, const size_t cols, const size_t char_sz = 30);

	/**
	 * \brief Add an option to the menu
	 * 
	 * Some menus can have multiple options that have different text but are 
	 * associated with the same \a key. For example, if given a quiz question 
	 * with only one correct answer, a menu with four options would have three
	 * choices that corresponds to MenuOptionKey::Incorrect and one that
	 * corresponds to MenuOptionKey::Correct. If which incorrect choice the 
	 * player selects matters, the client should specify a unique number among 
	 * the options associated with the same MenuOptionKey for the third 
	 * parameter. 
	 * 
	 * When retrieving the menu option the highlight cursor with \property 
	 * getHoveredOption, the client will also receive the value that they passed
	 * in for the third parameter.
	 * 
	 * It is up to the client to pass unique IDs for added options that have the 
	 * same enumeration key in one menu if different actions need to be taken 
	 * with each enumeration duplicate. They only have to be unique with that 
	 * group. MenuOptionKey::Incorrect and MenuOptionKey::Correct can each have 
	 * three duplicates ID'd 0, 1, and 2.
	 * 
	 * This method generates an assert error if the new text is empty. \property 
	 * findOption relies on its text argument being an empty string to determine 
	 * whether to include it in its search filters.
	 * 
	 * \param key		Menu option's enumeration identifier
	 * \param txt		Text to display
	 * \param id		Relative ID if the menu already has multiple options with 
	 * 					the same enumeration value. Optional
	 */
	void addOption(const MenuOptionKey key, const std::string& txt, 
		const uint32_t id = 0);

	/**
	 * \brief Remove an option from the menu
	 * 
	 * \a key, \a txt, and \a id must match exactly with the arguments of the 
	 * desired menu option that the the client added via \property addOption or 
	 * changed the text of via \property setOptionText. If this method can't find
	 * a match that exists in the menu, it generates an assert error.
	 * 
	 * \param key		Menu option's enumeration identifier
	 * \param txt		Displayed text
	 * \param id		Relative ID if the menu already has multiple options with 
	 * 					the same enumeration value. Optional
	 */
	void delOption(const MenuOptionKey key, const std::string& txt, 
		const uint32_t id = 0);

	/**
	 * \brief Checks if the menu is empty
	 *
	 * \return True if yes; false otherwise
	 */
	bool empty() const;

	/**
	 * \brief Change an option's text
	 * 
	 * \a key and \a id must match exactly with the first and third arguments of 
	 * the desired menu option the client added via \property addOption . If this
	 * method can't find a match that exists in the menu, it generates an assert
	 * error.
	 * 
	 * This method also generates an assert error if the new text is empty. 
	 * \property findOption relies on its text argument being an empty string to 
	 * determine whether to include it in its search filters.
	 * 
	 * \param key		Menu option's enumeration identifier
	 * \param txt		New text to display
	 * \param id		Relative ID if the menu already has multiple options with 
	 * 					the same enumeration value. Optional
	 */
	void setOptionText(const MenuOptionKey key, const std::string& txt, 
		const uint32_t id = 0);

	/**
	 * \brief Change an option's color
	 * 
	 * The public version of this method.
	 * 
	 * \a key, \a txt, and \a id must match exactly with the arguments of the 
	 * desired menu option that the the client added via \property addOption or 
	 * changed the text of via \property setOptionText. If this method can't find
	 * a match that exists in the menu, it generates an assert error.
	 * 
	 * \param key		Menu option's enumeration identifier
	 * \param txt		Displayed text
	 * \param color	New color
	 * \param id		Relative ID if the menu already has multiple options with 
	 * 					the same enumeration value. Optional
	 */
	void setOptionColor(const MenuOptionKey key, const std::string& txt, 
		const sf::Color color, const uint32_t id = 0);
	
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
	 * This method must be called during every iteration of the game loop to keep 
	 * the menu displayed.
	 * 
	 * \param window		Window for the graphical menu to be rendered on
	 */
	void draw(sf::RenderWindow& window) const;

	/**
	 * \brief Get the option that the highlight cursor is currently on
	 * 
	 * Use this method if the player selects an option on the menu to find out 
	 * what the player selected and react from there.
	 * 
	 * \return Enumeration key of the highlighted menu option and its relative 
	 * ID that the client had specified among those with the same enumeration 
	 * key. Or nothing if the menu is empty
	 */
	std::optional<std::pair<MenuOptionKey, uint32_t>> 
	getHoveredOption() const;

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
	sf::Font m_font;				///< Font used
	sf::RectangleShape m_box;	///< Rectangle that forms the menu's background

	///< Row and column coordinate indicating which menu option is currently 
	// being highlighted for selection
	std::pair<size_t, size_t> m_sel_rc; 

	///< Menu options. The first tuple component is an option's enumeration 
	// key. The second component is the graphical text. However, it's possible 
	// to have multiple options with the same enumeration key but different text.
	// The third component of the tuple is to indicate exactly which duplicate of 
	// the same key. If the third argument in \property addOption isn't specified 
	// when called, this value would be 0.
	std::vector<std::tuple<MenuOptionKey, sf::Text, uint32_t>> m_options;

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
	 * error
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
	 * This one takes an 1-D index instead of a row and column numbers. The menu 
	 * option container performs an out-of-bound check on the index argument
	 * 
	 * \param r			Option's row number
	 * \param c			Option's Column number
	 * \param color	New color
	 */
	void setOptionColor(const size_t idx, const sf::Color color);

	/**
	 * \brief Search for an option
	 * 
	 * This method finds the first option that matches the arguments. Typically, 
	 * the first one found in the vector is the only one. If there are more, 
	 * they are ignored. The assumption is that the multiple duplicates with the 
	 * same relative IDs were intentional on the client's part and that the 
	 * client only needs to delete or change the text of the first option.
	 * 
	 * This method checks if \a txt is empty or not. 
	 * 
	 * \param key		Enumeration value
	 * \param id		Relative ID if the menu already has multiple options with 
	 * 					the same enumeration value.
	 * \param txt		Text. Optional.
	 * 
	 * \return Non-const iterator to the first menu option that matches the 
	 * arguments. If there is no match, \var m_options.end() is returned. 
	 * 
	 * method can include text as a filter in its string
	 */
	auto findOption(const MenuOptionKey key, const uint32_t id, 
		const std::string& txt = "") 
			-> decltype(m_options.begin());
};

}