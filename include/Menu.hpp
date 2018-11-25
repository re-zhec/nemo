#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include <optional>
#include <SFML/Graphics.hpp>
#include <pugixml.hpp>

namespace rp
{

/**
 * \brief Graphical menu.
 *
 * Calling the constructor creates an empty menu. The client must add options 
 * to the menu via \property add. The menu isn't displayed on the render window
 * until \property draw is called.
 * 
 * Menu options's IDs are templatized. This design is to help with compiler 
 * warnings/errors if the client accidentally two options from two different 
 * menus. IDs can be scoped enums, classes with a defined == operator, etc..
 */
template <typename T> 
class Menu
{
public:
	/////////////////////////////////////////////////////////
	// Type alias
	/////////////////////////////////////////////////////////
	using float2 = std::pair<float, float>;
	using sf_color2 = std::pair<sf::Color, sf::Color>;
	using sf_color3 = std::tuple<sf::Color, sf::Color, sf::Color>;
	
	/////////////////////////////////////////////////////////
	// Methods
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
		const float2 pos,
		const float2 dim,
		const size_t rows, 
		const size_t cols, 

		const float2 outer_margins = {10.f, 10.f},
		const float2 inner_margins = {10.f, 10.f},
		const size_t char_sz = 20,

		const sf_color3 option_color = {{43,7,0}, {249,231,228}, {229,197,191}},
		const sf_color3 cursor_color = {{244,50,116}, {250,250,250}, {229,197,191}},
		const sf_color2 box_color = {{251,245,240}, {243,200,214}},
		
		const std::string& font_file = "font/Montserrat/Montserrat-Regular.ttf"
	);

	/**
	 * \brief Constructs an empty menu.
	 * 
	 * This constructor reads an XML file containing the arguments for the first 
	 * constructor, extracts them, and calls the above constructor with them.
	 * 
	 * \param file		Path to the file containing menu arguments.
	 */
	Menu(const std::string& xmlfile);

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
	 * setOptionText, \property setOptionColor, etc..
	 */
	Menu& add(const T id, const std::string& txt);

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
	 * setOptionText, \property setOptionColor, etc..
	 */
	Menu& remove(const T id);

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
	 * setOptionText, \property setOptionColor, etc..
	 */
	Menu& setOptionText(const T id, const std::string& txt);

	/**
	 * \brief Change an option's colors.
	 * 
	 * The public version of an overloaded method.
	 * 
	 * It finds the option based on its ID. If no options in the menu has \a id, 
	 * this method generates an assertion error.
	 * 
	 * \param key		ID of the menu option to change the colors of.
	 * \param colors	New color set.
	 * 
	 * \return A reference to the object itself. This allows for chaining 
	 * multiples and combinations of \property add, \property remove, \property 
	 * setOptionText, \property setOptionColor, etc..
	 */
	Menu& setOptionColor(const T id, const sf_color3 color);

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
	std::optional<T> cursorAt() const;

private:
	/////////////////////////////////////////////////////////
	// Additional types
	/////////////////////////////////////////////////////////
	/**
	 * \brief Struct containing all the arguments needed to construct a Menu
	 * object, listed in order of the parameters of the first constructor. The 
	 * second constructor creates one from reading the XML file.
	 */
	using ctor_args = struct ctor_args {
		float2 pos;
		float2 dim;
		size_t rows;
		size_t cols;
		float2 outer_margins;
		float2 inner_margins;
		size_t char_sz;
		sf_color3 option_color;
		sf_color3 cursor_color;
		sf_color2 box_color;
		std::string font_file;
	
		/**
		 * \brief Extract the color from an XML element that contains RGB 
		 * attributes.
		 * 
		 * \param color 		An XML element node containing attributes named 
		 * 						"red", "green", and "blue", each paired with a value.
		 * 
		 * \return An SFML Color object
		 */
		sf::Color getXMLColor(const pugi::xml_node& color);

		/**
		 * \brief Extract the horizontal and vertical margins from an XML that
		 * contains these attributes.
		 * 
		 * \param margins 	An XML element node containing attributes named 
		 * 						"hz" and "vt", each paired with a value.
		 * 
		 * \return Horizontal and vertical margins, in that order.
		 */
		float2 getXMLMargins(const pugi::xml_node& margins);
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
	// Variables
	/////////////////////////////////////////////////////////
	///< Starting xy-coordinate relative to the render window.
	//		1. X-coordinate.
	//		2. Y-coordinate.
	float2 m_pos;
	///< Size of the entire menu. This doesn't include the page number and 
	// navigation arrow indicators that may be added directly below the menu.
	//		1. Width.
	//		2. Height.
	float2 m_dim;

	///< Margins between the sides of the menu and of the outer menu options.
	//		1. Horizontal.
	//		2. Vertical.
	float2 m_page_margins;

	///< Maximum rows of menu options per page.
	size_t m_rows;

	///< Maximum columns of menu options per page.
	size_t m_cols;

	///< All menu options are stored in this vector. Each element consists of 
	// the menu option's:
	//		1. ID.
	//		2. graphical text.
	//		3. colors, in this order:
	//			a. text color.
	//			b. background color.
	//			c. border color.
	std::vector<std::tuple<T, sf::Text, sf_color3>> m_options;

	///< Default colors of each menu option other than the one the cursor is
	// over, in this order:
	//		1. Text color.
	//		2. Background color.
	//		3. Border color.
	sf_color3 m_option_color;

	///< 0-based row and column indices of the menu's cursor.
	//		1. Row index.
	//		2. Column index.
	std::pair<size_t, size_t> m_cursor_rc;

	///< Color of the menu option the cursor is over.
	//		1. Text color.
	//		2. Background color.
	//		3. Border color.
	sf_color3 m_cursor_color;

	///< Character size for all menu options' text.
	size_t m_char_sz;

	///< Menu options' cells.
	std::vector<sf::RectangleShape> m_cells;

	///< Menu box.
	sf::RectangleShape m_box;
	
	///< Font.
	sf::Font m_font;

	/////////////////////////////////////////////////////////
	// Methods
	/////////////////////////////////////////////////////////
	/**
	 * \brief Construct an empty menu from a struct containing arguments for the
	 * first public constructor.
	 * 
	 * This constructor works in tandem with \property parseXML to implement 
	 * the second public constructor, which takes in an XML file containing the 
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
	 * A private version of the overloaded method.
	 * 
	 * If \a idx is outside the range of menu options, this method generates an
	 * assertion error.
	 * 
	 * \param idx		0-based index of the menu option to change the colors of 
	 * 					from the vector of menu options.
	 * \param color	New color set.
	 */
	void setOptionColor(const size_t idx, const sf_color3 color);

	/**
	 * \brief Change an option's colors.
	 * 
	 * A private version of the overloaded method.
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
	void setOptionColor(const size_t r, const size_t c, const sf_color3 colors);

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
	 * \var m_options.end() is returned.
	 */
	auto find(const T id);

	/**
	 * \brief Parse an XML file that contains arguments for constructing a Menu 
	 * object.
	 * 
	 * This method works in tandem with the private constructor to implement the 
	 * second public constructor, which takes in an XML file.
	 * 
	 * It generates an assertion error if the XML parsing fails for any 
	 * reason.
	 * 
	 * \return Struct containing arguments needed by the first public 
	 * constructor.
	 */
	ctor_args parseXML(const std::string& xmlfile);
};

}