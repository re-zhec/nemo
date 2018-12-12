#include <fstream>
#include <boost/assert.hpp>

#include "MenuItemGraphics.hpp"
#include "nlohmann/json.hpp"
#include "utility/wrapper/sfMakeColor.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
MenuItemGraphics::update(sf::RenderWindow& window)
{
	window.draw(cell_);
	window.draw(text_);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuItemGraphics::MenuItemGraphics(
	const std::string&              file,
	const XYPair&                   pos,
	const XYPair&                   dim,
	const std::shared_ptr<sf::Font> font
)
	: MenuItemGraphics(parse(file), pos, dim, font)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuItemGraphics::MenuItemGraphics(
	const ParseInfo&                info, 
	const XYPair&                   pos,
	const XYPair&                   dim,
	const std::shared_ptr<sf::Font> font
)
	: MenuItemGraphics(
		pos,
		dim,
		info.margins_,
		info.colors_,
		font,
		info.font_sz_,
		info.center_
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

		const auto font_sz = js.at("font").at("size");
		const auto center  = js.at("center");

		return ParseInfo(
			margins,
			colors,
			font_sz,
			center
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
	const bool           center
)
	: margins_ (margins)
	, colors_  (colors)
	, font_sz_ (font_sz)
	, center_  (center)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}