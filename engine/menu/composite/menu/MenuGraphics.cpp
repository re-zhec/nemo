#include <fstream>
#include <iostream>
#include <boost/assert.hpp>

#include "MenuGraphics.hpp"
#include "../MenuEntry.hpp"
#include "nlohmann/json.hpp"
#include "utility/wrapper/sfVector2.hpp"
#include "utility/wrapper/sfMakeColor.hpp"

namespace nemo
{

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
 
MenuGraphics::MenuGraphics(
	const std::string&              title,
	const XYPair&                   pos,
	const XYPair&                   dim,
	const XYPair&                   margins,
	const TextBoxColors             colors,
	const std::shared_ptr<sf::Font> font,
	const int                       font_sz,
	const AlignConfig               align,
	const Row&                      rows,
	const Column&                   cols
)
	: MenuEntryGraphics(
		title, 
		pos, 
		dim, 
		margins, 
		colors, 
		font, 
		font_sz, 
		align
	)
	, rows_(rows)
	, cols_(cols)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuGraphics::MenuGraphics(
	const std::string&              title,
	const std::string&              file,
	const std::shared_ptr<sf::Font> font
)
	: MenuGraphics(title, parse(file), font)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void 
MenuGraphics::update(MenuEntry& entry, sf::RenderWindow& window)
{
	drawOn(window);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

MenuGraphics::MenuGraphics(
	const std::string&              title,
	const ParseInfo&                info,
	const std::shared_ptr<sf::Font> font
)
	: MenuGraphics(
		title,
		info.pos_,
		info.dim_,
		info.margins_,
		info.colors_,
		font,
		info.font_sz_,
		info.align_,
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

		const auto align = AlignConfig(
			std::string(js.at("alignment").at("horizontal")),
			std::string(js.at("alignment").at("vertical"))
		);

		const auto colors = TextBoxColors(
			sfMakeColor(js.at("colors").at("text")),
			sfMakeColor(js.at("colors").at("background")),
			sfMakeColor(js.at("colors").at("border"))
		);

		const auto font_sz = js.at("font").at("size");
		const auto rows    = Row(js.at("rows"));
		const auto cols    = Column(js.at("columns"));

		return ParseInfo(
			pos,
			dim,
			margins,
			colors,
			font_sz,
			align,
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
	const XYPair&       pos,
	const XYPair&       dim,
	const XYPair&       margins,
	const TextBoxColors colors,
	const int           font_sz,
	const AlignConfig   align,
	const Row&          rows,
	const Column&       cols
)
	: pos_    (pos)
	, dim_    (dim)
	, margins_(margins)
	, colors_ (colors)
	, font_sz_(font_sz)
	, align_  (align)
	, rows_   (rows)
	, cols_   (cols)
{
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

}