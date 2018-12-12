#include <fstream>
#include <boost/assert.hpp>

#include "MenuGraphics.hpp"
#include "nlohmann/json.hpp"
#include "utility/wrapper/sfVector2.hpp"
#include "utility/wrapper/sfMakeColor.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
 
MenuGraphics::MenuGraphics(
	const XYPair&                   pos,
	const XYPair&                   dim,
	const XYPair&                   margins,
	const TextBoxColors             colors,
	const std::shared_ptr<sf::Font> font,
	const int                       font_sz,
	const bool                      center,
	const Row&                      rows,
	const Column&                   cols
)
	: MenuEntryGraphics(pos, dim, margins, colors, font, font_sz, center)
	, rows_(rows)
	, cols_(cols)
{
	BOOST_ASSERT(rows > 0);
	BOOST_ASSERT(cols > 0);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuGraphics::MenuGraphics(
	const std::string&              file,
	const std::shared_ptr<sf::Font> font
)
	: MenuGraphics(parse(file), font)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
MenuGraphics::update(sf::RenderWindow& window)
{
	// Draw the menu box.
	window.draw(cell_);
	window.draw(text_);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuGraphics::MenuGraphics(
	const ParseInfo&                info,
	const std::shared_ptr<sf::Font> font
)
	: MenuGraphics(
		info.pos_,
		info.dim_,
		info.margins_,
		info.colors_,
		font,
		info.font_sz_,
		info.center_,
		info.rows_,
		info.cols_
	)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuGraphics::ParseInfo
MenuGraphics::parse(const std::string& file)
const
{
	std::ifstream ifs(file);
	BOOST_ASSERT(ifs);
	nlohmann::json js;
	ifs >> js;

	try {
		const auto pos = XYPair(
			XValue(js.at("position").at("x")),
			YValue(js.at("position").at("y"))
		);

		const auto dim = XYPair(
			XValue(js.at("dimensions").at("width")),
			YValue(js.at("dimensions").at("height"))
		);

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
		const auto rows    = Row(js.at("rows"));
		const auto cols    = Column(js.at("columns"));

		return ParseInfo(
			pos,
			dim,
			margins,
			colors,
			font_sz,
			center,
			rows,
			cols
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

MenuGraphics::ParseInfo::ParseInfo(
	const XYPair&        pos,
	const XYPair&        dim,
	const XYPair&        margins,
	const TextBoxColors  colors,
	const int            font_sz,
	const bool           center,
	const Row&           rows,
	const Column&        cols
)
	: pos_     (pos)
	, dim_     (dim)
	, margins_ (margins)
	, colors_  (colors)
	, font_sz_ (font_sz)
	, center_  (center)
	, rows_    (rows)
	, cols_    (cols)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}