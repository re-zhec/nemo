#pragma once

#include <string>
#include <vector>
#include <utility>
#include <optional>
#include <SFML/Graphics.hpp>

#include "utility/type/MenuOptionID.hpp"
#include "utility/type/Color.hpp"
#include "utility/type/XYValue.hpp"
#include "utility/type/RowColumn.hpp"
#include "utility/adaptor/RC1DConverter.hpp"
#include "utility/wrapper/sfml.hpp"

namespace sb
{

/**
 * \brief Graphical menu.
 *
 * Calling the constructor creates an empty menu. The client must add options 
 * to the menu via \property add. The menu isn't displayed on the render window
 * until \property draw is called.
 */
class Menu
{
public:
	/////////////////////////////////////////////////////////
	// Methods                                             //
	/////////////////////////////////////////////////////////
	/**
	 * \brief Constructs an empty menu.
	 * 
	 * Upon construction, the menu will be empty. The client should add options 
	 * to the menu before calling \property draw; otherwise, the menu would be 
	 * displayed in the render window as an empty box. The first menu option 
	 * added will always have the cursor over it by default. If the client 
	 * doesn't fill all the slots in a page of menu options, then there will be 
	 * empty space where the rest of the options would normally be.
	 * 
	 * This constructor generates an assertion error if 
	 * 	1. \a pos, \a dim, or \a margins contains negative values.
	 * 	2. \a rows or \a cols is non-positive.
	 * 	3. a font file cannot be found at \var font_file.
	 * 	4. \a char_sz is too tall to fit text in each slot allocated for menu 
	 * 		options.
	 * 
	 * \param pos              Starting position in the render window.
	 * \param dim              Overall size. This doesn't include the page number 
	 *                         that may be added directly below the menu.
	 * \param rows             Maximum rows of menu options per page.
	 * \param cols             Maximum columns of menu options per page.
	 * 
	 * \param outer_margins    Margins between the sides of the menu and of the 
	 *                         outer menu options.
	 * \param inner_margins    Margins between each menu option.
	 * 
	 * \param align_center     Whether all menu options' text should be 
	 *                         horizontally centered.
	 * \param char_sz          Character size of each menu option's text.
	 * 
	 * \param option_color     Colorset for each menu option.
	 * \param cursor_color		Colorset for the menu option the cursor is over.
	 * \param box_color			Colorset for the menu's box.
	 * 
	 * \param font_file			Font's filepath.
	 */
	Menu(
		const XYValue pos,
		const XYValue dim,

		const Row rows, 
		const Column cols, 

		const XYValue outer_margins = { XValue(10.f), YValue(10.f) },
		const XYValue inner_margins = { XValue(10.f), YValue(10.f) },

		const bool align_center = false,
		const size_t char_sz = 16,

		const TextBoxColor option_color = {{43,7,0}, {249,231,228}, {229,197,191}},
		const TextBoxColor cursor_color = {{244,50,116}, {250,250,250}, {229,197,191}},
		const TextBoxColor box_color    = {{0,0,0}, {251,245,240}, {243,200,214}},
		
		const std::string& font_file = "font/Montserrat-Regular.ttf"
	);

	/**
	 * \brief Constructs an empty menu.
	 * 
	 * This constructor reads a json file containing the arguments for the first 
	 * constructor and use the above constructor for the extracted values.
	 * 
	 * \param file    Path to the file containing menu arguments.
	 */
	Menu(const std::string& file);

	/**
	 * \brief Add an option to the menu.
	 * 
	 * The client must specify a unique ID for every menu option added. If a new 
	 * option shares \a id with an existing one from this menu, this method 
	 * generates an assertion error.
	 * 
	 * \param id      ID of menu option to add.
	 * \param txt     Text to display.
	 * 
	 * \return A reference to the object itself. Chaining is possible.
	 */
	Menu& add(const MenuOptionID id, const std::string& txt);

	/**
	 * \brief Remove an option from the menu.
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, this method generates an assertion error.
	 * 
	 * \param id      ID of the menu option to remove.
	 * 
	 * \return A reference to the object itself. Chaining is possible.
	 */
	Menu& remove(const MenuOptionID id);

	/**
	 * \brief Change an option's text
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, then this method generates an assertion error.
	 * 
	 * \param key     Menu option's ID.
	 * \param txt     New text to display.
	 * 
	 * \return A reference to the object itself. Chaining is possible
	 */
	Menu& changeOptionText(const MenuOptionID id, const std::string& txt);

	/**
	 * \brief Change an option's colors.
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, this method generates an assertion error.
	 * 
	 * \param key     ID of the menu option to change the colors of.
	 * \param colors  New color set.
	 * 
	 * \return A reference to the object itself. Chaining is possible.
	 */
	Menu& changeOptionColor(const MenuOptionID id, const TextBoxColor color);

	/**
	 * \brief Checks if the menu is empty.
	 *
	 * \return True if so; false otherwise.
	 */
	bool empty() const noexcept;

	/**
	 * \brief Move cursor to the menu option above the current one.
	 */
	void moveUp();

	/**
	 * \brief Move cursor to the menu option below the current one.
	 */
	void moveDown();

	/**
	 * \brief Move cursor to the menu option to the right of the current one.
	 */
	void moveRight();

	/**
	 * \brief Move cursor to the menu option to the left of the current one.
	 */
	void moveLeft();

	/**
	 * \brief Render the menu on screen.
	 * 
	 * This method should be called during every iteration of the game loop to 
	 * keep the menu displayed.
	 * 
	 * \param window     Render window for the graphical menu to be drawn on.
	 */
	void draw(sf::RenderWindow& window);

	/**
	 * \brief Get the option that the cursor is currently over.
	 * 
	 * Use this method if the player selects an option on the menu to find out 
	 * what the player selected.
	 * 
	 * \return The ID of the menu option that the cursor is on, or nothing 
	 * if the menu is empty.
	 */
	std::optional<MenuOptionID> cursorAt() const;

private:
	/////////////////////////////////////////////////////////
	// Additional types                                    //
	/////////////////////////////////////////////////////////
	/**
	 * \brief Struct containing all the arguments needed to construct a Menu
	 * object, listed in order of the parameters of the first constructor. The 
	 * second constructor creates one from reading the json file.
	 */
	struct ctor_args {
		XYValue pos;
		XYValue dim;
		Row rows;
		Column cols;
		XYValue outer_margins;
		XYValue inner_margins;
		bool align_center;
		size_t char_sz;
		TextBoxColor option_color;
		TextBoxColor cursor_color;
		TextBoxColor box_color;
		std::string font_file;
	
		/**
		 * \brief Create a sf::Color object out of a 4-element vector of integers. 
		 * attributes.
		 * 
		 * \param color   An json element node containing RGB values in this 
		 *                order: red, green, blue, and alpha (opacity).
		 * 
		 * \return An SFML Color object
		 */
		sf::Color makeColor(const std::array<int, 4>& rgba);
	};

	/**
	 * \brief Menu option.
	 */ 
	struct MenuOption
	{
		MenuOptionID id_;    ///< Identifier.
		sf::Text     txt_;   ///< Graphical text.
		TextBoxColor color_; ///< Colorset.
	};

	/**
	 * \brief Enumeration for which direction the player moves the cursor to in 
	 * the menu.
	 * 
	 * This is used by \property move, which \property moveUp, \property 
	 * moveDown, \property moveLeft, and \property moveRight are wrappers of.
	 */ 
	enum class Direction {
		Up,
		Down,
		Right,
		Left
	};

	/////////////////////////////////////////////////////////
	// Variables                                           //
	/////////////////////////////////////////////////////////
	///< All menu options' text are aligned to the center horizontally if this is
	// true. Otherwise, they are aligned to the left.
	bool align_center_;

	Row    rows_; ///< Maximum rows of menu options per page.
	Column cols_; ///< Maximum columns of menu options per page.

	///< Row-column/1-D index converter to index the container where all the menu 
	// options are stored. While options are displayed as rows and columns, they 
	// are internally stored in 1-D.
	RC1DConverter rc1d_conv_;

	///< Container where all menu options that are added to, deleted from, etc.. 
	std::vector<MenuOption> options_;

	TextBoxColor option_color_; ///< Default colorset of each menu option other 
	                            // than the one the cursor is over.
	TextBoxColor cursor_color_; ///< Colorset for the menu option the cursor is 
	                            // over.

	RCPoint cursor_rc_; ///< 0-based row and column indices of the menu's cursor.
	size_t char_sz_;    ///< Character size for all menu options' text.

	std::vector<sf::RectangleShape> cells_; ///< Menu options' cells.
	sf::RectangleShape box_;                ///< Menu box.
	sf::Font font_;                        ///< Font.

	/////////////////////////////////////////////////////////
	// Methods                                             //
	/////////////////////////////////////////////////////////
	/**
	 * \brief Set where a menu option's text should be drawn on the render 
	 * window.
	 * 
	 * The exact position is based on its index in the menu option vector.
	 * Options are positioned from left to right, down across rows. This or the 
	 * other version should be called for every new menu option added or for an 
	 * option that needs to be shifted frontward due to the removal of a 
	 * preceding option.
	 * 
	 * If \a idx is out of range, this method generates an assertion error.
	 *  
	 * \param idx     0-based index of the option in the menu option vector that
	 *                needs to its position preset.
	 */
	void presetTextPosition(const int idx);

	/**
	 * \brief Change an option's colors
	 * 
	 * If \a idx is outside the range of menu options, this method generates an
	 * assertion error.
	 * 
	 * \param idx     0-based index of the menu option to change the colors of
	 * \param color   New colorset.
	 */
	void setOptionColor(const int idx, const TextBoxColor color);

	/**
	 * \brief Render a menu option on screen.
	 * 
	 * \property draw calls this method to render the right page of menu options 
	 * on screen.
	 * 
	 * This method generates an assertion error if \a idx is out of range.
	 * 
	 * \param idx		   0-based index of the menu option to render.
	 * \param window	   Render window for the menu option to be drawn on.
	 */
	void drawOption(const int idx, sf::RenderWindow& window);

	/**
	 * \brief Render the page numbers and navigator arrow indicators on screen.
	 * 
	 * \property draw calls this method if there are multiple pages. If there is 
	 * only one when this is called, the arrows are excluded.
	 * 
	 * \param window     Render window for the page navigation references to be 
	 *                   drawn on.
	 */
	void drawPageRef(sf::RenderWindow& window) const;

	/**
	 * \brief Move the cursor along menu options.
	 * 
	 * This method does the main work for \property moveUp, \property moveDown, 
	 * \property moveLeft, and \property moveRight, which all give this method 
	 * the directional enumeration value as the argument.
	 *  
	 * \param dir     Direction to move the cursor to.
	 */
	void move(const Direction dir);

	/**
	 * \brief Search for an option
	 * 
	 * This method finds the option that matches \a id. It SHOULDN'T find more 
	 * than one since \property add disallows multiple menu options having 
	 * the same ID, but if for some reason there is, it grabs the first 
	 * occurrence.
	 * 
	 * \param id      ID of the menu option to find.
	 * 
	 * \return Non-const iterator to the menu option found. If there is no match, 
	 * \var options_.end() is returned.
	 */
	auto find(const MenuOptionID id) -> decltype(options_.begin());

	/**
	 * \brief Construct an empty menu from a struct containing arguments for the
	 * non-file-based public constructor.
	 * 
	 * This constructor works in tandem with \property parseFile to implement 
	 * the public constructor that takes in a json file containing menu 
	 * specifications.
	 */
	Menu(ctor_args args);

	/**
	 * \brief Parse a json file that contains arguments for constructing a Menu 
	 * object.
	 * 
	 * This method works in tandem with the private constructor to implement the 
	 * the public constructor that takes in a json file.
	 * 
	 * It generates an assertion error if the json parsing fails for any 
	 * reason.
	 * 
	 * \return Struct containing arguments needed by the first public 
	 * constructor.
	 */
	ctor_args parseFile(const std::string& file);
};

}