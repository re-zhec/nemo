#pragma once

#include <string>
#include <vector>
#include <utility>
#include <optional>
#include <SFML/Graphics.hpp>

#include "utility/type/Color.hpp"
#include "utility/type/XY.hpp"
#include "utility/type/RowColumn.hpp"
#include "utility/adaptor/RC1DConverter.hpp"
#include "utility/wrapper/sfVector2.hpp"

namespace nemo
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
		const XYPair pos,
		const XYPair dim,

		const Row rows, 
		const Column cols, 

		const XYPair outer_margins     = { XValue(10.f), YValue(10.f) },
		const XYPair inner_margins     = { XValue(10.f), YValue(10.f) },

		const bool align_center         = false,
		const size_t char_sz            = 16,

		const TextBoxColor option_color = {{43,7,0}, {249,231,228}, {229,197,191}},
		const TextBoxColor cursor_color = {{244,50,116}, {250,250,250}, {229,197,191}},
		const TextBoxColor box_color    = {{0,0,0}, {251,245,240}, {243,200,214}},
		
		const std::string& font_file    = "font/Montserrat-Regular.ttf"
	);

	/**
	 * \brief Constructs an empty menu.
	 * 
	 * This constructor uses a json file containing the customizations for the 
	 * menu.
	 * 
	 * \param file    Path to the file containing menu arguments.
	 */
	Menu(const std::string& file);

	/**
	 * \brief Add an option to the menu.
	 * 
	 * The client must specify a unique ID for every menu option added. Two menus
	 * can each have a menu option with the same ID, though.
	 * 
	 * \param id      ID of menu option to add.
	 * \param txt     Text to display.
	 * 
	 * \return A reference to the object itself.
	 */
	Menu& 
	add(const int id, const std::string& txt);

	/**
	 * \brief Remove an option from the menu.
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, this method generates an assertion error.
	 * 
	 * \param id      ID of the menu option to remove.
	 * 
	 * \return A reference to the object itself.
	 */
	Menu& 
	remove(const int id);

	/**
	 * \brief Change an option's text
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, then this method generates an assertion error.
	 * 
	 * \param key     Menu option's ID.
	 * \param txt     New text to display.
	 * 
	 * \return A reference to the object itself.
	 */
	Menu& 
	changeOptionText(const int id, const std::string& txt);

	/**
	 * \brief Change an option's colors.
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, this method generates an assertion error.
	 * 
	 * \param key     ID of the menu option to change the colors of.
	 * \param colors  New color set.
	 * 
	 * \return A reference to the object itself.
	 */
	Menu& 
	changeOptionColor(const int id, const TextBoxColor color);

	/**
	 * \brief Checks if the menu is empty.
	 *
	 * \return True if so; false otherwise.
	 */
	bool 
	empty() 
	const noexcept;

	/**
	 * \brief Move cursor to the menu option above the current one.
	 */
	void 
	moveUp()
	noexcept;

	/**
	 * \brief Move cursor to the menu option below the current one.
	 */
	void 
	moveDown()
	noexcept;

	/**
	 * \brief Move cursor to the menu option to the right of the current one.
	 */
	void 
	moveRight()
	noexcept;

	/**
	 * \brief Move cursor to the menu option to the left of the current one.
	 */
	void 
	moveLeft()
	noexcept;

	/**
	 * \brief Render the menu on screen.
	 * 
	 * This method should be called each frame of the game loop when applicable 
	 * to keep the menu displayed.
	 * 
	 * \param window     Render window for the graphical menu to be drawn on.
	 */
	void 
	draw(sf::RenderWindow& window);

	/**
	 * \brief Get the option that the cursor is currently over.
	 * 
	 * Use this method if the player selects an option on the menu to find out 
	 * what the player selected.
	 * 
	 * \return The ID of the menu option that the cursor is on, or nothing 
	 * if the menu is empty.
	 */
	std::optional<int> 
	cursorAt() 
	const;

private:
	/////////////////////////////////////////////////////////
	// Additional types                                    //
	/////////////////////////////////////////////////////////
	/**
	 * \brief Struct containing all the arguments needed to construct a Menu
	 * object, listed in order of the parameters of the first constructor. 
	 * The file-parsing constructor passes an instance of this struct returned by 
	 * \property parseFile to a private constructor to create a menu.
	 */
	struct CtorArgs
	{
		///< Default arguments are bad values that would cause assertion errors
		// during construction.
		CtorArgs(
			const XYPair      pos           = { XValue(-1.f), YValue(-1.f) },
			const XYPair      dim           = { XValue(-1.f), YValue(-1.f) },
			const Row          rows          = Row(0),
			const Column       cols          = Column(0),
			const XYPair      outer_margins = { XValue(-1.f), YValue(-1.f) },
			const XYPair      inner_margins = { XValue(-1.f), YValue(-1.f) },
			const bool         align_center  = false,
			const size_t       char_sz       = 0,
			const TextBoxColor option_color  = { {0,0,0}, {0,0,0}, {0,0,0} },
			const TextBoxColor cursor_color  = { {0,0,0}, {0,0,0}, {0,0,0} },
			const TextBoxColor box_color     = { {0,0,0}, {0,0,0}, {0,0,0} },
			const std::string& font_file     = ""
		);

		XYPair      pos_;
		XYPair      dim_;
		Row          rows_;
		Column       cols_;
		XYPair      outer_margins_;
		XYPair      inner_margins_;
		bool         align_center_;
		size_t       char_sz_;
		TextBoxColor option_color_;
		TextBoxColor cursor_color_;
		TextBoxColor box_color_;
		std::string  font_file_;
	};

	/**
	 * \brief Menu option data.
	 */ 
	struct MenuOption
	{
		int id_;    ///< Identifier.
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

	///< Row-column/1-D index converter for indexing the container where all the 
	// menu options are stored. While options are displayed as rows and columns, 
	// they are internally stored as 1-D.
	RC1DConverter rc1d_conv_;

	///< Container where all menu options that are added to, deleted from, etc.. 
	std::vector<MenuOption> options_;

	TextBoxColor option_color_; ///< Default colorset of each menu option other 
	                            // than the one the cursor is over.
	TextBoxColor cursor_color_; ///< Colorset of the menu option with the cursor.

	RCPair cursor_rc_; ///< 0-based row-column location of the menu's cursor.
	size_t char_sz_;    ///< Character size for all menu options' text.

	std::vector<sf::RectangleShape> cells_; ///< Menu options' cells.
	sf::RectangleShape box_;                ///< Menu box.
	sf::Font font_;                         ///< Font.

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
	 * \param idx     0-based index of the option in the menu option vector that
	 *                needs to its position preset.
	 */
	void 
	presetTextPosition(const int idx);

	/**
	 * \brief Change an option's colors
	 * 
	 * \param idx     0-based index of the menu option to change the colors of.
	 * \param color   New colorset.
	 */
	void 
	setOptionColor(const int idx, const TextBoxColor color);

	/**
	 * \brief Render a menu option on screen.
	 * 
	 * \property draw calls this method to render the appropriate page of menu 
	 * options on screen.
	 * 
	 * \param idx		   0-based index of the menu option to render.
	 * \param window	   Render window for the menu option to be drawn on.
	 */
	void 
	drawOption(const int idx, sf::RenderWindow& window);

	/**
	 * \brief Render the page numbers and navigator arrow indicators on screen.
	 * 
	 * \property draw calls this method if there are multiple pages. If there is 
	 * only one when this is called, the arrows are excluded.
	 * 
	 * \param window     Render window for the page navigation references to be 
	 *                   drawn on.
	 */
	void 
	drawPageRef(sf::RenderWindow& window) 
	const;

	/**
	 * \brief Move the cursor along menu options.
	 * 
	 * This method does the main work for \property moveUp, \property moveDown, 
	 * \property moveLeft, and \property moveRight
	 *  
	 * \param dir     Direction to move the cursor to.
	 */
	void
	move(const Direction dir)
	noexcept;

	/**
	 * \brief Search for an option
	 * 
	 * This method finds the option that matches \a id. It SHOULDN'T find more 
	 * than one since \property add disallows multiple menu options having 
	 * the same ID, but if for some reason there is, it returns the first 
	 * occurrence.
	 * 
	 * \param id      ID of the menu option to find.
	 * 
	 * \return Non-const iterator to the menu option found. If there is no match, 
	 * \var options_.end() is returned.
	 */
	auto 
	find(const int id) 
	-> decltype(options_.begin());

	/**
	 * \brief Construct an empty menu from a struct containing arguments for the
	 * non-file-based public constructor.
	 * 
	 * This constructor works in tandem with \property parseFile to implement 
	 * the public constructor that takes in a json file containing menu 
	 * specifications.
	 */
	Menu(CtorArgs args);

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
	CtorArgs 
	parseFile(const std::string& file)
	const;
};

}