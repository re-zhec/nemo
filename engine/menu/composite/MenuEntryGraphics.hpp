#pragma once

#include <memory>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include "utility/type/XY.hpp"
#include "utility/type/Color.hpp"
#include "MenuEntry.hpp"

namespace nemo
{

class MenuEntryGraphics
{
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
public:
	virtual
	~MenuEntryGraphics() {}

	virtual void
	update(sf::RenderWindow& window) = 0;

	void 
	setText(const std::string& text);

	void
	setColors(const TextBoxColors colors);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
protected:

	sf::RectangleShape cell_;
	sf::Text           text_;

	/////////////////////////////////////////////////////
	//                                                 //
	/////////////////////////////////////////////////////

	MenuEntryGraphics(
		const XYPair&                   pos,
		const XYPair&                   dim,
		const XYPair&                   margins,
		const TextBoxColors             colors,
		const std::shared_ptr<sf::Font> font,
		const int                       font_sz,
		const bool                      center
	);

	MenuEntryGraphics(
		const std::string&              file, 
		const std::shared_ptr<sf::Font> font
	);

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
private:

	std::shared_ptr<sf::Font> font_;
	int                       font_sz_;
	bool                      center_;
};

}