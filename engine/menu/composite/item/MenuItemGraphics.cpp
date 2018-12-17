#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <SFML/Graphics/Text.hpp>

#include "MenuItemGraphics.hpp"
#include "../MenuEntry.hpp"
#include "nlohmann/json.hpp"
#include "utility/wrapper/sfMakeColor.hpp"
#include "utility/wrapper/sfVector2.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
MenuItemGraphics::update(MenuEntry& entry, sf::RenderWindow& window)
{
	drawOn(window);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuItemGraphics::MenuItemGraphics(
	const std::string&              text,
	const std::string&              file,
	const XYPair&                   pos,
	const XYPair&                   dim,
	const std::shared_ptr<sf::Font> font
)
	: MenuItemGraphics(text, parse(file), pos, dim, font)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuItemGraphics::MenuItemGraphics(
	const std::string&              text,
	const ParseInfo&                info,
	const XYPair&                   pos,
	const XYPair&                   dim,
	const std::shared_ptr<sf::Font> font
)
	: MenuItemGraphics(
		text,
		pos,
		dim,
		info.margins_,
		info.colors_,
		font,
		info.font_sz_,
		info.align_
	)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuItemGraphics::ParseInfo
MenuItemGraphics::parse(const std::string& file)
const
{
	std::ifstream ifs(file);
	BOOST_ASSERT(ifs);
	nlohmann::json js;
	ifs >> js;

	try {
		const auto margins = XYPair(
			XValue(js.at("margins").at("horizontal")),
			YValue(js.at("margins").at("vertical"))
		);

		const auto colors = TextBoxColors(
			sfMakeColor(js.at("colors").at("text")),
			sfMakeColor(js.at("colors").at("background")),
			sfMakeColor(js.at("colors").at("border"))
		);

		const auto align = AlignConfig(
			std::string(js.at("alignment").at("horizontal")),
			std::string(js.at("alignment").at("vertical"))
		);

		const auto font_sz = js.at("font").at("size");

		return ParseInfo(
			margins,
			colors,
			font_sz,
			align
		);
	}
	catch (const nlohmann::json::out_of_range& e) {
		// BOOST_LOG_TRIVIAL(error) << file << " parsing failed. " << e.what();
		return ParseInfo();
	}
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuItemGraphics::ParseInfo::ParseInfo(
	const XYPair&        margins,
	const TextBoxColors  colors,
	const int            font_sz,
	const AlignConfig    align
)
	: margins_(margins)
	, colors_ (colors)
	, font_sz_(font_sz)
	, align_  (align)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}