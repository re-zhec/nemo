#pragma once

#include <string>
#include <vector>
#include <utility>
#include <optional>
#include <SFML/Graphics.hpp>

#include "utility/type/defs.hpp"
#include "utility/adaptor/RowColumnAdaptor.hpp"

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
	// Additional types                                    //
	/////////////////////////////////////////////////////////
	using SFColor2 = std::pair<sf::Color, sf::Color>;
	using SFColor3 = std::tuple<sf::Color, sf::Color, sf::Color>;
	
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
	 * 	2. \a rows or \a cols is 0.
	 * 	3. a font file cannot be found at \var font_file.
	 * 	4. \a char_sz is too tall to fit text in each slot allocated for menu 
	 * 		options.
	 * 
	 * \param pos			Starting position in the render window, in this order:
	 * 							1. X-coordinate.
	 * 							2. Y-coordinate.
	 * \param dim			Overall size. This doesn't include the page number that 
	 * 						may be added directly below the menu.
	 * 							1. Width.
	 * 							2. Height.
	 * \param rows			Maximum rows of menu options per page.
	 * \param cols			Maximum columns of menu options per page.
	 * 
	 * [Optional parameters]
	 * 
	 * \param outer_margins		Margins between the sides of the menu and of the 
	 * 								outer menu options, in this order:
	 * 									1. horizontal margins.
	 * 									2. vertical margins.
	 * \param inner_margins		Margins between each menu option, in this order:
	 * 									1. horizontal margins.
	 * 									2. vertical margins.
	 * \param align_center		Whether all menu's text should be horizontally 
	 * 								centered.
	 * \param char_sz				Character size of each menu option's text.
	 * \param option_color		Colors of each menu option, in this order:
	 * 									1. text color.
	 * 									2. background color. 
	 * 									3. border color.
	 * \param cursor_color		Cursor's colors, in this order:
	 * 									1. text color.
	 * 									2. background color.
	 * 									3. border color.
	 * \param box_color			Colors of the menu's box, in this order:
	 * 									1. background color.
	 * 									2. border color.
	 * \param font_file			Font's filepath.
	 */
	Menu(
		const sf::Vector2f pos,
		const sf::Vector2f dim,
		const Row rows, 
		const Column cols, 

		const sf::Vector2f outer_margins = {10.f, 10.f},
		const sf::Vector2f inner_margins = {10.f, 10.f},
		const bool align_center = false,
		const size_t char_sz = 20,

		const SFColor3 option_color = {{43,7,0}, {249,231,228}, {229,197,191}},
		const SFColor3 cursor_color = {{244,50,116}, {250,250,250}, {229,197,191}},
		const SFColor2 box_color = {{251,245,240}, {243,200,214}},
		
		const std::string& font_file = "font/Montserrat/Montserrat-Regular.ttf"
	);

	/**
	 * \brief Constructs an empty menu.
	 * 
	 * This constructor reads a json file containing the arguments for the first 
	 * constructor, extracts them, and calls the above constructor with them.
	 * 
	 * \param file		Path to the file containing menu arguments.
	 */
	Menu(const std::string& file);

	/**
	 * \brief Add an option to the menu.
	 * 
	 * The client must specify a unique ID for every menu option added. If a new 
	 * option shares \a id with an existing one from this menu, this method 
	 * generates an assertion error.
	 * 
	 * \param id		ID of menu option to add.
	 * \param txt		Text to display.
	 * 
	 * \return A reference to the object itself. This allows for chaining 
	 * multiples and combinations of \property add, \property remove, \property 
	 * changeOptionText, \property changeOptionColor, etc..
	 */
	Menu& add(const int id, const std::string& txt);

	/**
	 * \brief Remove an option from the menu.
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, this method generates an assertion error.
	 * 
	 * \param id		ID of the menu option to remove.
	 * 
	 * \return A reference to the object itself. This allows for chaining 
	 * multiples and combinations of \property add, \property remove, \property 
	 * changeOptionText, \property changeOptionColor, etc..
	 */
	Menu& remove(const int id);

	/**
	 * \brief Change an option's text
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, then this method generates an assertion error.
	 * 
	 * \param key		Menu option's ID.
	 * \param txt		New text to display.
	 * 
	 * \return A reference to the object itself. This allows for chaining 
	 * multiples and combinations of \property add, \property remove, \property 
	 * changeOptionText, \property changeOptionColor, etc..
	 */
	Menu& changeOptionText(const int id, const std::string& txt);

	/**
	 * \brief Change an option's colors.
	 * 
	 * This method finds the option based on its ID. If no options in the menu 
	 * has \a id, this method generates an assertion error.
	 * 
	 * \param key		ID of the menu option to change the colors of.
	 * \param colors	New color set.
	 * 
	 * \return A reference to the object itself. This allows for chaining 
	 * multiples and combinations of \property add, \property remove, \property 
	 * changeOptionText, \property changeOptionColor, etc..
	 */
	Menu& changeOptionColor(const int id, const SFColor3 color);

	/**
	 * \brief Checks if the menu is empty.
	 *
	 * \return True if so; false otherwise.
	 */
	bool empty() const noexcept;
	
	/**
	 * moveUp(), moveDown(), moveLeft(), and moveRight() move the menu's cursor 
	 * from one option to another.
	 */

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
	 * \param window		Render window for the graphical menu to be drawn on.
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
	std::optional<int> cursorAt() const;

private:
	/////////////////////////////////////////////////////////
	// Additional types                                    //
	/////////////////////////////////////////////////////////
	/**
	 * \brief Struct containing all the arguments needed to construct a Menu
	 * object, listed in order of the parameters of the first constructor. The 
	 * second constructor creates one from reading the json file.
	 */
	using ctor_args = struct ctor_args {
		sf::Vector2f pos;
		sf::Vector2f dim;
		Row rows;
		Column cols;
		sf::Vector2f outer_margins;
		sf::Vector2f inner_margins;
		bool align_center;
		size_t char_sz;
		SFColor3 option_color;
		SFColor3 cursor_color;
		SFColor2 box_color;
		std::string font_file;
	
		/**
		 * \brief Create a sf::Color object out of a 4-element vector of integers. 
		 * attributes.
		 * 
		 * \param color 		An json element node containing RGB values in this 
		 * 						order: red, green, blue, and alpha (opacity).
		 * 
		 * \return An SFML Color object
		 */
		sf::Color makeColor(const std::vector<int>& rgba);
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
	///< Whether all menu options' text should be center-aligned horizontally.
	bool align_center_;

	///< Whether the menu options' text should 

	///< Maximum rows of menu options per page.
	Row rows_;

	///< Maximum columns of menu options per page.
	Column cols_;

	///< Row-column indices <-> 1-D index converter for the container storing 
	// all the menu options. The menu options are displayed as rows and columns, 
	// but their storage is 1-D.
	RowColumnAdaptor rc_adaptor_;

	///< All menu options are stored in this container. Each element consists of 
	// the menu option's:
	//		1. ID.
	//		2. graphical text.
	//		3. colors, in this order:
	//			a. text color.
	//			b. background color.
	//			c. border color.
	std::vector<std::tuple<int, sf::Text, SFColor3>> options_;

	///< Default colors of each menu option other than the one the cursor is
	// over, in this order:
	//		1. Text color.
	//		2. Background color.
	//		3. Border color.
	SFColor3 option_color_;

	///< 0-based row and column indices of the menu's cursor.
	//		1. Row index.
	//		2. Column index.
	RowColumn cursor_rc_;

	///< Color of the menu option the cursor is over.
	//		1. Text color.
	//		2. Background color.
	//		3. Border color.
	SFColor3 cursor_color_;

	///< Character size for all menu options' text.
	size_t char_sz_;

	///< Menu options' cells.
	std::vector<sf::RectangleShape> cells_;

	///< Menu box.
	sf::RectangleShape box_;
	
	///< Font.
	sf::Font m_font;

	/////////////////////////////////////////////////////////
	// Methods                                             //
	/////////////////////////////////////////////////////////
	/**
	 * \brief Construct an empty menu from a struct containing arguments for the
	 * first public constructor.
	 * 
	 * This constructor works in tandem with \property parseFile to implement 
	 * the second public constructor, which takes in a json file containing the 
	 * menu specifications.
	 */
	Menu(ctor_args args);

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
	 * \param idx		0-based index of the option in the menu option vector that
	 * 					needs to its position preset.
	 */
	void presetTextPosition(const size_t idx);

	/**
	 * \brief Move the cursor along menu options.
	 * 
	 * This method does the main work for \property moveUp, \property moveDown, 
	 * \property moveLeft, and \property moveRight, which all give this method 
	 * the directional enumeration value as the argument.
	 *  
	 * \param dir		Direction to move the cursor to.
	 */
	void move(const Direction dir);

	/**
	 * \brief Change an option's colors
	 * 
	 * If \a idx is outside the range of menu options, this method generates an
	 * assertion error.
	 * 
	 * \param idx		0-based index of the menu option to change the colors of 
	 * 					from the vector of menu options.
	 * \param color	New color set.
	 */
	void setOptionColor(const size_t idx, const SFColor3 color);

	/**
	 * \brief Change an option's colors.
	 * 
	 * This is a wrapper around the above version. It takes the row and column 
	 * indices of the menu option to change the colors of and translate that to 
	 * its 1-D correspondent to grab the desired menu option by index. A row and
	 * column position that is outside the range of menu options will cause an 
	 * assertion error.
	 * 
	 * \param r			0-based row index of the menu option to change.
	 * \param c			0-based column index of the menu option to change.
	 * \param colors	New color set.
	 */
	void setOptionColor(const Row r, const Column c, const SFColor3 colors);

	/**
	 * \brief Render a menu option on screen.
	 * 
	 * \property draw calls this method to render the right page of menu options 
	 * on screen.
	 * 
	 * This method generates an assertion error if \a idx is out of range.
	 * 
	 * \param idx		0-based index of the menu option to render.
	 * \param window	Render window for the menu option to be drawn on.
	 */
	void drawOption(const size_t idx, sf::RenderWindow& window);

	/**
	 * \brief Render the page numbers and navigator arrow indicators on screen.
	 * 
	 * \property draw calls this method if there are multiple pages. If there is 
	 * only one when this is called, the arrows are excluded.
	 * 
	 * \param window	Render window for the page navigation references to be 
	 * drawn on.
	 */
	void drawPageRef(sf::RenderWindow& window) const;

	/**
	 * \brief Search for an option
	 * 
	 * This method finds the option that matches \a id. It SHOULDN'T find more 
	 * than one since \property add disallows multiple menu options having 
	 * the same ID, but if for some reason there is, it grabs the first 
	 * occurrence.
	 * 
	 * \param id		ID of the menu option to find.
	 * 
	 * \return Non-const iterator to the menu option found. If there is no match, 
	 * \var options_.end() is returned.
	 */
	auto find(const int id) -> decltype(options_.begin());

	/**
	 * \brief Parse a json file that contains arguments for constructing a Menu 
	 * object.
	 * 
	 * This method works in tandem with the private constructor to implement the 
	 * second public constructor, which takes in a json file.
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